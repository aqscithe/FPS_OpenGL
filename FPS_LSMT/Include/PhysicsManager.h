#pragma once
#include "Maths.h"
#include <vector>
#include "Collider3D.h"
#include "GameObject.h"

#include "ToolBox3D.h"

#define G -9.81f

using namespace Core::Maths;
using namespace Core::Maths::ToolBox3D;

namespace Physics
{
	class Hit
	{
	public :
		vec3		interPt;
		vec3		interNormal;
		GameObject* gameObject;
		Quaternion	exitQ = QuaternionIdentity();
	};

	class PhysicsManager //singleton
	{
	public :
		static PhysicsManager* getInstance();

		vec3 gravity = { 0, G, 0 };

		void addBoxCollider(BoxCollider* boxCollider);
		void addSphereCollider(SphereCollider* sphereCollider);
		void deleteBoxCollider(BoxCollider* boxCollider);
		void deleteSphereCollider(SphereCollider* sphereCollider);
		bool CollisionBoxesSphere(const SphereCollider& sphere,
								  std::vector<vec3>& normals,
								  const Segment& seg,
								  std::vector<BoxCollider*>& boxes,
								  std::vector<BoxCollider*>& nonCollisions,
								  std::vector<float>& frictions);
		bool RaycastHit(vec3 origin, vec3 dir, float dist, Hit& hit);
        bool CollisionSphereSphere(const SphereCollider* s1, std::vector<SphereCollider*>& spheres, std::vector<SphereCollider*>& nonCollisions);
		bool CollisionSegmentBox(const Segment* segment, vec3& normal);

	private:
		//Debug
		std::vector<BoxCollider*> boxColliders;
		int iDraw = 0;

		static PhysicsManager* instance;
		std::vector<SphereCollider*> sphereColliders;
	};
}
