#include "PhysicsManager.h"
#include "CheckCollision3D.h"
#include "ToolBox3D.h"
#include "Transform.h"
#include "Application.h"

using namespace Physics;
using namespace Core::Maths::ToolBox3D;

PhysicsManager* PhysicsManager::instance = 0;

PhysicsManager* PhysicsManager::getInstance()
{
	if (instance == 0)
	{
		instance = new PhysicsManager();
	}

	return instance;
}

void PhysicsManager::addBoxCollider(BoxCollider* boxCollider)
{
	boxColliders.push_back(boxCollider);
}

void  PhysicsManager::addSphereCollider(SphereCollider* sphereCollider)
{
	sphereColliders.push_back(sphereCollider);
}

void PhysicsManager::deleteBoxCollider(BoxCollider* boxCollider)
{
	bool colliderFound = false;
	int i;
	for (i = 0; i < boxColliders.size(); i++)
	{
		if (boxColliders[i] == boxCollider)
		{
			colliderFound = true;
			break;
		}
	}

	if (colliderFound)
	{
		//No delete !
		boxColliders[i] = boxColliders[boxColliders.size() - 1];
		boxColliders.pop_back();
	}
}

void PhysicsManager::deleteSphereCollider(SphereCollider* sphereCollider)
{
	bool colliderFound = false;
	int i;
	for (i = 0; i < sphereColliders.size(); i++)
	{
		if (sphereColliders[i] == sphereCollider)
		{
			colliderFound = true;
			break;
		}
	}

	if (colliderFound)
	{
		//No delete !
		sphereColliders[i] = sphereColliders[sphereColliders.size() - 1];
		sphereColliders.pop_back();
	}
}

vec3 SqDistPointAABB(const vec3& p, const BoxCollider& b)
{
	vec3 min = b.center;
	vec3 max = b.center;

	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				vec3 pos = b.center + Vector3RotateByQuaternion({ b.extensions.x * i, b.extensions.y * j, b.extensions.z * k }, b.q);
				min = { Core::Maths::min(min.x, pos.x), Core::Maths::min(min.y, pos.y), Core::Maths::min(min.z, pos.z) };
				max = { Core::Maths::max(max.x, pos.x), Core::Maths::max(max.y, pos.y), Core::Maths::max(max.z, pos.z) };
			}
		}
	}

	vec3 q;
	// For each coordinate axis, if the point coordinate value is
	// outside box, clamp it to the box, else keep it as is
	for (int i = 0; i < 3; i++) {
		float v = p.e[i];
		if (v < min.e[i]) v = min.e[i]; // v = max( v, b.min[i] )
		if (v > max.e[i]) v = max.e[i]; // v = min( v, b.max[i] )
		q.e[i] = v;
	}
	return q;
}

bool Physics::PhysicsManager::CollisionBoxesSphere(const SphereCollider& sphere,
												   std::vector<vec3>& normals,
												   const Segment& seg,
												   std::vector<BoxCollider*>& boxes,
												   std::vector<BoxCollider*>& nonCollisions,
                                                   std::vector<float>& frictions)
{
	vec3 interPt;
	vec3 normal;

	for (int i = 0; i < boxColliders.size(); i++)
	{
		//if (Collision::IntersectionSegmentBox(start, end, *boxColliders[i], interPt, normals))
		//	return true;

		BoxCollider b(boxColliders[i]->center, boxColliders[i]->q, boxColliders[i]->extensions + sphere.r * 2.f);

		if (Collision::IntersectionSegmentBox(seg.start, seg.end, b, interPt, normal))
		//if (Collision::cSegmentBox(Segment(seg.start, seg.end), b, interPt, normal))
		{
			boxes.push_back(boxColliders[i]);
			normals.push_back({ normal.x, normal.y, normal.z });
			frictions.push_back(boxColliders[i]->friction);
		}
		else
		{
			nonCollisions.push_back(boxColliders[i]);
		}
	}

	if (boxes.size() > 0)
		return true;

	return false;
}

bool Physics::PhysicsManager::CollisionSphereSphere(const SphereCollider* s1, std::vector<SphereCollider*>& spheres, std::vector<SphereCollider*>& nonCollisions)
{
	for (int i = 0; i < sphereColliders.size(); ++i)
	{
		SphereCollider* s2 = sphereColliders[i];
		if (s1 != s2)
		{
			//vector between each center
			vec3 OP = pointVector(s1->origin, s2->origin);

			float sqrRadiuses = (s1->r + s2->r) * (s1->r + s2->r);
			float magOP = sqrMag(OP);

			if (magOP > sqrRadiuses)
			{
				nonCollisions.push_back(sphereColliders[i]);
				continue;
			}
			
			spheres.push_back(s2);
		}
		else
			continue;
	}

	if (spheres.size() > 0)
		return true;

	return false;
}

bool Physics::PhysicsManager::CollisionSegmentBox(const Segment* segment, vec3& normal)
{
	vec3 point;
	for (auto& box : boxColliders)
	{
		if (Collision::IntersectionSegmentBox(segment->start, segment->end, *box, point, normal))
			return true;
	}
	return false;
}

bool PhysicsManager::RaycastHit(vec3 origin, vec3 dir, float dist, Hit& hit)
{
	bool intersect = false;

	dir *= dist;
	Quaternion exitQ;

	for (int i = 0; i < boxColliders.size(); i++)
	{
		if (boxColliders[i]->isTrigger)
			continue;

		vec3 interPt;
		vec3 interNormal;
		if (Collision::IntersectionSegmentBox(origin, dir, *boxColliders[i], interPt, interNormal, &exitQ))
		{
			if (!intersect || Distance(origin, interPt) < Distance(origin, hit.interPt))
			{
				hit.interPt = interPt;
				hit.interNormal = interNormal;
				hit.gameObject = boxColliders[i]->getGameObject<GameObject>();
				hit.exitQ = exitQ;
				intersect = true;
			}
		}
	}

	for (int i = 0; i < sphereColliders.size(); i++)
	{
		vec3 interPt;
		vec3 interNormal;
		if (Collision::IntersectionSegmentSphere(origin, dir, *sphereColliders[i], interPt, interNormal))
		{
			if (sphereColliders[i]->isTrigger)
				continue;

			if (!intersect || Distance(origin, interPt) < Distance(origin, hit.interPt))
			{
				hit.interPt = interPt;
				hit.interNormal = interNormal;
				hit.gameObject = sphereColliders[i]->getGameObject<GameObject>();
				intersect = true;
			}
		}
	}
	if (intersect)
		hit.interNormal = Normalize(hit.interNormal);

	return intersect;
}
