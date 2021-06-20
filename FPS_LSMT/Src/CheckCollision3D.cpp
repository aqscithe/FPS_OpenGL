#include "CheckCollision3D.h"
#include "ToolBox3D.h"

using namespace Core::Maths;
using namespace Core::Maths::ToolBox3D;

#define epsilon 0.000001

bool Collision::IntersectionSegmentPlane(const vec3& a, const vec3& b, const PlaneCollider& plane, vec3& interPt, vec3& interNormal)
{
	float abN = Dot(plane.Normal, b - a);

	if (fabs(abN) <= epsilon)
	{
		return false;
	}

	float t = (plane.d - Dot(plane.Normal, a)) / abN;

	if (t < 0 || t > 1)
	{

		return false;
	}

	interPt = a + ((b - a) * t);
	interNormal = plane.Normal;

	return true;
}

bool Collision::IntersectionSegmentSphere(const vec3& a, const vec3& b, const SphereCollider& sphere, vec3& interPt, vec3& interNormal)
{
	vec3 d = (b - a) / Magnitude(b - a);
	vec3 m = a - sphere.origin;

	float c = Dot(m, m) - sphere.r * sphere.r;
	float b_ = Dot(m, d);

	if (c > 0 && b_ > 0)
		return false;

	float delta = b_ * b_ - c;

	if (delta < 0)
		return false;

	float t = -b_ - sqrt(delta);

	if (t < 0.f)
		t = 0;

	interPt = a + d * t;
	interNormal = (interPt - sphere.origin) / sphere.r;

	return true;
}

bool IntersectionSegmentCylinderInfinite(const vec3& a, const vec3& b, const CylinderCollider& cylinder, vec3& interPt, vec3& interNormal)
{
	vec3 AB = b - a;
	vec3 PA = a - cylinder.pt1;
	vec3 PQ = cylinder.pt2 - cylinder.pt1;

	float A = Dot(PQ, PQ) * Dot(AB, AB) - Dot(PQ, AB) * Dot(PQ, AB);
	if (A < epsilon)
		return false;

	float C = Dot(PQ, PQ) * (Dot(PA, PA) - cylinder.r * cylinder.r) - Dot(PA, PQ) * Dot(PA, PQ);
	if (C < 0)
		return false;

	float B = Dot(PQ, PQ) * Dot(PA, AB) - Dot(PQ, AB) * Dot(PA, PQ);

	float delta = B * B - A * C;
	if (delta < epsilon)
		return false;

	float t = (-B - sqrtf(delta)) / A;

	if (t < 0 || t > 1)
		return false;

	interPt = a + t * AB;
	interNormal = Normalize(interPt);

	vec3 tiPt;
	vec3 tiNm;
	PlaneCollider plan(Normalize(PQ), interPt);

	if (Collision::IntersectionSegmentPlane(cylinder.pt1, cylinder.pt2, plan, tiPt, tiNm))
		interNormal = interPt - tiPt;

	return true;
}

bool IntersectionSegmentCylinderFinite(const vec3& a, const vec3& b, const CylinderCollider& cylinder, vec3& interPt, vec3& interNormal)
{
	vec3 PA = a - cylinder.pt1;
	vec3 PQ = cylinder.pt2 - cylinder.pt1;
	vec3 AB = b - a;
	
	float PA_PQ = Dot(PA, PQ);
	float PQ_PQ = Dot(PQ, PQ);
	float PA_PA = Dot(PA, PQ);
	float PQ_AB = Dot(PQ, AB);
	
	float A = PQ_PQ * Dot(AB, AB) - PQ_AB * PQ_AB;
	if (A < epsilon)
		return false;
	
	float C = PQ_PQ * (Dot(PA, PA) - cylinder.r * cylinder.r) - PA_PQ * PA_PQ;
	if (C < 0)
	{
		if (PA_PQ > 0 && PA_PQ < PQ_PQ)
			return false;
	
		if (PA_PQ < 0)
			return Collision::IntersectionSegmentDisk(a, b, { -PQ, cylinder.pt1, cylinder.r }, interPt, interNormal);
		if (PA_PQ > PQ_PQ)
			return Collision::IntersectionSegmentDisk(a, b, { PQ, cylinder.pt2, cylinder.r }, interPt, interNormal);
	}
	
	float B = PQ_PQ * Dot(PA, AB) - PQ_AB * PA_PQ;
	
	float delta = B * B - A * C;
	if (delta < epsilon)
		return false;
	
	float t = (-B - sqrtf(delta)) / A;
	
	if (t < 0 || t > 1)
		return false;
	
	interPt = a + t * AB;
	vec3 pi = interPt - cylinder.pt1;
	float pi_PQ = Dot(pi, PQ);
	
	if (pi_PQ < 0)
		return Collision::IntersectionSegmentDisk(interPt, cylinder.pt2, { -PQ, cylinder.pt1, cylinder.r }, interPt, interNormal);
	if (pi_PQ > PQ_PQ)
		return Collision::IntersectionSegmentDisk(interPt, cylinder.pt2, { PQ, cylinder.pt2, cylinder.r }, interPt, interNormal);
	
	vec3 tiPt;
	vec3 tiNm;
	PlaneCollider plan(Normalize(PQ), interPt);
	
	if (Collision::IntersectionSegmentPlane(cylinder.pt1, cylinder.pt2, plan, tiPt, tiNm))
		interNormal = interPt - tiPt;
	
	return true;
}

bool Collision::IntersectionSegmentDisk(const vec3& a, const vec3& b, const DiskCollider& disk, vec3& interPt, vec3& interNormal)
{
	if (!IntersectionSegmentPlane(a, b, { disk.origin, disk.r }, interPt, interNormal))
		return false;

	vec3 OI = interPt - disk.origin;

	if (Dot(OI, OI) < disk.r * disk.r)
		return true;

	return false;
}

bool Collision::IntersectionSegmentCylinder(const vec3& a, const vec3& b, const CylinderCollider& cylinder, vec3& interPt, vec3& interNormal)
{
	if (cylinder.infinite)
		return IntersectionSegmentCylinderInfinite(a, b, cylinder, interPt, interNormal);
	else
		return IntersectionSegmentCylinderFinite(a, b, cylinder, interPt, interNormal);
}

bool Collision::IntersectionSegmentCapsule(const vec3& a, const vec3& b, CapsuleCollider& capsule, vec3& interPt, vec3& interNormal)
{
	SphereCollider s1;
	SphereCollider s2;
	CylinderCollider cylinder;
	capsule.getCollider(s1, s2, cylinder);

	vec3 PA = a - cylinder.pt1;
	vec3 PQ = cylinder.pt2 - cylinder.pt1;
	vec3 AB = b - a;

	float PA_PQ = Dot(PA, PQ);
	float PQ_PQ = Dot(PQ, PQ);
	float PA_PA = Dot(PA, PQ);
	float PQ_AB = Dot(PQ, AB);

	float A = PQ_PQ * Dot(AB, AB) - PQ_AB * PQ_AB;
	if (A < epsilon)
		return false;

	float C = PQ_PQ * (Dot(PA, PA) - cylinder.r * cylinder.r) - PA_PQ * PA_PQ;
	if (C < 0)
	{
		if (PA_PQ > 0 && PA_PQ < PQ_PQ)
			return false;

		if (PA_PQ < 0)
			return IntersectionSegmentSphere(a, b, s1, interPt, interNormal);
		if (PA_PQ > PQ_PQ)
			return IntersectionSegmentSphere(a, b, s2, interPt, interNormal);
	}

	float B = PQ_PQ * Dot(PA, AB) - PQ_AB * PA_PQ;

	float delta = B * B - A * C;
	if (delta < epsilon)
		return false;

	float t = (-B - sqrtf(delta)) / A;

	if (t < 0 || t > 1)
		return false;

	interPt = a + t * AB;
	vec3 pi = interPt - cylinder.pt1;
	float pi_PQ = Dot(pi, PQ);

	if (pi_PQ < 0)
		return IntersectionSegmentSphere(interPt, cylinder.pt2, s1, interPt, interNormal);
	if (pi_PQ > PQ_PQ)
		return IntersectionSegmentSphere(interPt, cylinder.pt2, s2, interPt, interNormal);

	vec3 tiPt;
	vec3 tiNm;
	PlaneCollider plan(Normalize(PQ), interPt);

	if (IntersectionSegmentPlane(cylinder.pt1, cylinder.pt2, plan, tiPt, tiNm))
		interNormal = interPt - tiPt;

	return true;
}

void CapsuleCollider::getCollider(SphereCollider& s1, SphereCollider& s2, CylinderCollider& c1)
{
	vec3 axis = Vector3RotateByQuaternion({ 1, 1, 1 }, q);

	c1 = CylinderCollider(pt1, q, r, h, true);
	s1 = SphereCollider(pt1, q, r);
	s2 = SphereCollider(pt2, q, r);
}

bool Collision::IntersectionSegmentQuad(const vec3& a, const vec3& b, const QuadCollider& quad, vec3& interPt, vec3& interNormal)
{
	vec3 Normal = Normalize(Vector3RotateByQuaternion({ 0.0f, 1.0f, 0.0f }, quad.q));
	
	PlaneCollider plane(Normal, quad.pos);
	
	if (!IntersectionSegmentPlane(a, b, plane, interPt, interNormal))
		return false;
	
	vec3 i = Normalize(Vector3RotateByQuaternion({ 1.0f, 0.0f, 0.0f }, quad.q));
	vec3 j = Normalize(Vector3RotateByQuaternion({ 0.0f, 0.0f, 1.0f }, quad.q));
	vec3 vect = interPt - quad.pos;
	
	if (fabs(Dot(i, vect)) > quad.extensions.x || fabs(Dot(j, vect)) > quad.extensions.y)
		return false;
	
	return true;

	//vec3 Normal = Vector3RotateByQuaternion({ 0,1,0 }, quad.q);
	//
	//PlaneCollider plane(Normal, quad.pos);
	//
	//if (!IntersectionSegmentPlane(a, b, plane, interPt, interNormal))
	//	return false;
	//
	//vec3 i = Vector3RotateByQuaternion({ 1,0,0 }, quad.q);
	//vec3 j = Vector3RotateByQuaternion({ 0,0,1 }, quad.q);
	//
	//float projX = Dot(interPt, i);
	//float projY = Dot(interPt, j);
	//
	//if (fabs(projX) > quad.extensions.x || fabs(projY) > quad.extensions.y)
	//	return false;
	//
	////interNormal = a - interPt;
	//return true;
}

bool Collision::IntersectionSegmentBox(const vec3& ptA, const vec3& ptB, const BoxCollider& box, vec3& interPt, vec3& interNormal, Quaternion* exitQ)
{
	bool intersect = false;

	vec3 interPtQuad = {};
	{
		//Quad Ox
		{
			vec3 interPtBuffer = {};
			vec3 interNormalBuffer = {};
			QuadCollider quad(box.center + (box.unitX * box.extensions.x/2.f), QuaternionMultiply(box.q, QuaternionFromAxisAngle({ 0,0,1 }, PI / 2.f)), { box.extensions.y/2.f , box.extensions.z/2.f });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
			{
				interPtQuad = interPtBuffer;
				interNormal = interNormalBuffer;
				intersect = true;
				if (exitQ)
					*exitQ = quad.q;
			}
		}

		{
			vec3 interPtBuffer = {};
			vec3 interNormalBuffer = {};
			QuadCollider quad(box.center - (box.unitX * box.extensions.x/2.f), QuaternionMultiply(box.q, QuaternionFromAxisAngle({ 0,0,1 }, PI / 2)), { box.extensions.y/2.f , box.extensions.z/2.f });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interPtQuad = interPtBuffer;
					interNormal = interNormalBuffer;
					intersect = true;
					if (exitQ)
					*exitQ = quad.q;
				}
		}

		//Quad Oy
		{
			vec3 interPtBuffer = {};
			vec3 interNormalBuffer = {};
			QuadCollider quad(box.center + (box.unitY * box.extensions.y/2.f), QuaternionMultiply(box.q, QuaternionFromAxisAngle({ 0,0,1 }, 0)), { box.extensions.x/2.f , box.extensions.z/2.f });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interPtQuad = interPtBuffer;
					interNormal = interNormalBuffer;
					intersect = true;
					if (exitQ)
					*exitQ = quad.q;
				}
		}

		{
			vec3 interPtBuffer = {};
			vec3 interNormalBuffer = {};
			QuadCollider quad(box.center - (box.unitY * box.extensions.y/2.f), QuaternionMultiply(box.q, QuaternionFromAxisAngle({ 0,0,1 }, PI)), { box.extensions.x/2.f , box.extensions.z/2.f});
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interPtQuad = interPtBuffer;
					interNormal = interNormalBuffer;
					intersect = true;
					if (exitQ)
					*exitQ = quad.q;
				}
		}

		//Quad Oz
		{
			vec3 interPtBuffer = {};
			vec3 interNormalBuffer = {};
			QuadCollider quad(box.center + (box.unitZ * box.extensions.z/2.f), QuaternionMultiply(box.q, QuaternionFromAxisAngle({ 1,0,0 }, PI / 2)), { box.extensions.x/2.f, box.extensions.y/2.f });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interPtQuad = interPtBuffer;
					interNormal = interNormalBuffer;
					intersect = true;
					if (exitQ)
						*exitQ = quad.q;
				}
		}

		{
			vec3 interPtBuffer = {};
			vec3 interNormalBuffer = {};
			QuadCollider quad(box.center - (box.unitZ * box.extensions.z/2.f), QuaternionMultiply(box.q, QuaternionFromAxisAngle({ 1,0,0 }, PI / 2)), { box.extensions.x/2.f , box.extensions.y/2.f });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interPtQuad = interPtBuffer;
					interNormal = interNormalBuffer;
					intersect = true;
					if (exitQ)
						*exitQ = quad.q;
				}
		}
	}

	if (!intersect)
		return false;

	interPt = interPtQuad;
	return true;
}

bool Collision::IntersectionSegmentRoundedBox(const vec3& ptA, const vec3& ptB, const BoxCollider& roundedBox, vec3& interPt, vec3& interNormal)
{
	bool intersect = false;
	vec3 facesExtensions;

	vec3 interPtQuad = {};
	{
		//Quad
		{
			vec3 interPtBuffer = {};
			vec3 interNormalBuffer = {};
			facesExtensions = (roundedBox.extensions/2 + vec3{ roundedBox.radius, roundedBox.radius ,roundedBox.radius });
			QuadCollider quad(roundedBox.center + (roundedBox.unitX * facesExtensions.x), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 0,0,1 }, PI / 2)), { roundedBox.extensions.y/2 , roundedBox.extensions.z/2 });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
			{
				interPtQuad = interPtBuffer;
				interNormal = interNormalBuffer;
				intersect = true;
			}
		}

		{
			vec3 interPtBuffer = {};
			vec3 interNormalBuffer = {};
			QuadCollider quad(roundedBox.center + (roundedBox.unitX * -facesExtensions.x), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 0,0,1 }, PI / 2)), { roundedBox.extensions.y/2 , roundedBox.extensions.z/2 });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
		}

		{
			//facesExtensions = (roundedBox.extensions + vec3{ roundedBox.radius, 0 ,roundedBox.radius });
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			QuadCollider quad((roundedBox.center + (roundedBox.unitY * facesExtensions.y)), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 0,0,1 }, 0)), { roundedBox.extensions.x/2 , roundedBox.extensions.z/2 });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			QuadCollider quad((roundedBox.center + (roundedBox.unitY * -facesExtensions.y)), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 0,0,1 }, PI)), { roundedBox.extensions.x/2 , roundedBox.extensions.z/2 });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			QuadCollider quad((roundedBox.center + (roundedBox.unitZ * facesExtensions.z)), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 1,0,0 }, PI / 2)), { roundedBox.extensions.x/2 , roundedBox.extensions.y/2 });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			QuadCollider quad((roundedBox.center + (roundedBox.unitZ * -facesExtensions.z)), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 1,0,0 }, PI / 2)), { roundedBox.extensions.x/2 , roundedBox.extensions.y/2 });
			if (IntersectionSegmentQuad(ptA, ptB, quad, interPtBuffer, interNormalBuffer))
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
		}

		//Cylinder Oz
		{
			facesExtensions = (roundedBox.extensions);
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) +
				(roundedBox.unitZ * -facesExtensions.z))), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 1,0,0 }, PI / 2)), roundedBox.radius, facesExtensions.z * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) +
				(roundedBox.unitZ * -facesExtensions.z))), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 1,0,0 }, PI / 2)), roundedBox.radius, facesExtensions.z * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * facesExtensions.x) + (roundedBox.unitY * facesExtensions.y)) +
				(roundedBox.unitZ * -facesExtensions.z))), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 1,0,0 }, PI / 2)), roundedBox.radius, facesExtensions.z * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * facesExtensions.y)) +
				(roundedBox.unitZ * -facesExtensions.z))), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 1,0,0 }, PI / 2)), roundedBox.radius, facesExtensions.z * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		//Cylinder Oy
		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center, (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)),
				(roundedBox.unitZ * -facesExtensions.z))), roundedBox.q, roundedBox.radius, facesExtensions.y * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) +
				(roundedBox.unitZ * facesExtensions.z))), roundedBox.q, roundedBox.radius, facesExtensions.y * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) +
				(roundedBox.unitZ * -facesExtensions.z))), roundedBox.q, roundedBox.radius, facesExtensions.y * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) +
				(roundedBox.unitZ * facesExtensions.z))), roundedBox.q, roundedBox.radius, facesExtensions.y * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		//Cylinder Ox
		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) +
				(roundedBox.unitZ * -facesExtensions.z))), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 0,0,1 }, -PI / 2)), roundedBox.radius, facesExtensions.z * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) +
				(roundedBox.unitZ * facesExtensions.z))), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 0,0,1 }, -PI / 2)), roundedBox.radius, facesExtensions.z * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * facesExtensions.y)) +
				(roundedBox.unitZ * -facesExtensions.z))), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 0,0,1 }, -PI / 2)), roundedBox.radius, facesExtensions.z * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			CylinderCollider cylinder((roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * facesExtensions.y)) +
				(roundedBox.unitZ * facesExtensions.z))), QuaternionMultiply(roundedBox.q, QuaternionFromAxisAngle({ 0,0,1 }, -PI / 2)), roundedBox.radius, facesExtensions.z * 2, false);
			if (IntersectionSegmentCylinder(ptA, ptB, cylinder, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		//Sphere
		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			if (IntersectionSegmentSphere(ptA, ptB, { (roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) + (roundedBox.unitZ * -facesExtensions.z)))
				, roundedBox.radius }, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			if (IntersectionSegmentSphere(ptA, ptB, { (roundedBox.center + (((roundedBox.unitX * -facesExtensions.x), (roundedBox.unitY * facesExtensions.y)) + (roundedBox.unitZ * -facesExtensions.z)))
				, roundedBox.radius }, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			if (IntersectionSegmentSphere(ptA, ptB, { (roundedBox.center + (((roundedBox.unitX * facesExtensions.x) + (roundedBox.unitY * facesExtensions.y)) + (roundedBox.unitZ * -facesExtensions.z)))
				, roundedBox.radius }, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			if (IntersectionSegmentSphere(ptA, ptB, { (roundedBox.center + (((roundedBox.unitX * facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) + (roundedBox.unitZ * -facesExtensions.z)))
				, roundedBox.radius }, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			if (IntersectionSegmentSphere(ptA, ptB, { (roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) + (roundedBox.unitZ * facesExtensions.z)))
				, roundedBox.radius }, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			if (IntersectionSegmentSphere(ptA, ptB, { (roundedBox.center + (((roundedBox.unitX * -facesExtensions.x) + (roundedBox.unitY * facesExtensions.y)) + (roundedBox.unitZ * facesExtensions.z)))
				, roundedBox.radius }, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			if (IntersectionSegmentSphere(ptA, ptB, { (roundedBox.center + (((roundedBox.unitX * facesExtensions.x) + (roundedBox.unitY * facesExtensions.y)) + (roundedBox.unitZ * facesExtensions.z)))
				, roundedBox.radius }, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}

		{
			vec3 interNormalBuffer = {};
			vec3 interPtBuffer = {};
			if (IntersectionSegmentSphere(ptA, ptB, { (roundedBox.center + (((roundedBox.unitX * facesExtensions.x) + (roundedBox.unitY * -facesExtensions.y)) + (roundedBox.unitZ * facesExtensions.z)))
				, roundedBox.radius }, interPtBuffer, interNormalBuffer))
			{
				if (Distance(ptA, interPtBuffer) < Distance(ptA, interPtQuad))
				{
					interNormal = interNormalBuffer;
					interPtQuad = interPtBuffer;
					intersect = true;
				}
			}
		}
	}

	if (!intersect)
		return false;

	interPt = interPtQuad;
	return true;
}

bool Collision::IntersectionBoxSphere(const SphereCollider& sphere, const BoxCollider& box)
{
	vec3 i = Vector3RotateByQuaternion({ 1.f, 0.f, 0.f }, box.q);
	vec3 j = Vector3RotateByQuaternion({ 0.f, 1.f, 0.f }, box.q);
	vec3 k = Vector3RotateByQuaternion({ 0.f, 0.f, 1.f }, box.q);

	vec3 localSphereOrigin = GlobalToLocalPosition(box.center, i, j, k, sphere.origin);

	if (fabs(localSphereOrigin.x) > box.extensions.x + sphere.r || fabs(localSphereOrigin.y) > box.extensions.y + sphere.r || fabs(localSphereOrigin.z) > box.extensions.z + sphere.r)
		return false;

	vec3 pt = { Clamp(localSphereOrigin.x, -box.extensions.x, box.extensions.x), Clamp(localSphereOrigin.y, -box.extensions.y, box.extensions.y), Clamp(localSphereOrigin.z, -box.extensions.z, box.extensions.z) };

	if (Magnitude(pt - localSphereOrigin) > sphere.r)
		return false;

	return true;
}

BoxCollider GetAABB(const BoxCollider& box)
{
	vec3 min = box.center;
	vec3 max = box.center;

	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				vec3 pos = box.center + Vector3RotateByQuaternion({ box.extensions.x * i, box.extensions.y * j, box.extensions.z * k }, box.q);
				min = { Core::Maths::min(min.x, pos.x), Core::Maths::min(min.y, pos.y), Core::Maths::min(min.z, pos.z) };
				max = { Core::Maths::max(max.x, pos.x), Core::Maths::max(max.y, pos.y), Core::Maths::max(max.z, pos.z) };
			}
		}
	}

	vec3 extensions = { (max.x - min.x) * 0.5f, (max.y - min.y) * 0.5f, (max.z - min.z) * 0.5f };
	vec3 AABB = { (min.x + extensions.x), (min.y + extensions.y), (min.z + extensions.z) };

	return { AABB, QuaternionIdentity(), extensions };
}

bool Collision::IntersectionBoxBox(const BoxCollider& box1, const BoxCollider& box2)
{
	BoxCollider AABB1 = GetAABB(box1);
	BoxCollider AABB2 = GetAABB(box2);

	vec3 localAABB = GlobalToLocalPosition(AABB2.center, { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, AABB1.center);

	if (fabs(localAABB.x) > AABB1.extensions.x + box2.extensions.x || fabs(localAABB.y) > AABB1.extensions.y + box2.extensions.y || fabs(localAABB.z) > AABB1.extensions.z + box2.extensions.z)
		return false;

	return true;
}

/*
bool SATBoxOnAxis(const vec3& axis, const std::vector<vec3> corners1, const std::vector<vec3> corners2)
{
	Range range1 = { Dot(axis, corners1[0]), Dot(axis, corners1[0]) };
	Range range2 = { Dot(axis, corners2[0]), Dot(axis, corners2[0]) };

	for (int i = 1; i < corners1.size(); i++)
	{
		range1 = RangeUpdate(range1, Dot(axis, corners1[i]));
		range2 = RangeUpdate(range2, Dot(axis, corners2[i]));
	}
}*/

//TODO : IntersetionOrientedBoxBox
bool Collision::IntersectionOrientedBoxBox(const BoxCollider& box1, const BoxCollider& box2)
{
	if (!IntersectionBoxBox(box1, box2))
		return false;
	return true;
}

bool Collision::CollisionBoxSphere(const SphereCollider& sphere, const BoxCollider& box, vec3& interPt, vec3& interNormal)
{
	Quaternion quaternion = QuaternionFromEuler(box.q.x, box.q.y, box.q.z);

	vec3 i = Vector3RotateByQuaternion({ 1.0f, 0.0f, 0.0f }, quaternion);
	vec3 j = Vector3RotateByQuaternion({ 0.0f, 1.0f, 0.0f }, quaternion);
	vec3 k = Vector3RotateByQuaternion({ 0.0f, 0.0f, 1.0f }, quaternion);

	vec3 boxToSphere = sphere.origin - box.center;
	SphereCollider localSphere = { {Dot(boxToSphere, i), Dot(boxToSphere, j), Dot(boxToSphere, k)}, sphere.r };

	if (!((localSphere.origin.x) > ((box.extensions.x / 2) + localSphere.r))
		|| !((localSphere.origin.y) > ((box.extensions.y / 2) + localSphere.r))
		|| !((localSphere.origin.z) > ((box.extensions.z / 2) + localSphere.r)))
		return false;

	vec3 closestPoint = { Clamp(localSphere.origin.x, -box.extensions.x / 2, box.extensions.x / 2),
						  Clamp(localSphere.origin.y, -box.extensions.y / 2, box.extensions.y / 2),
						  Clamp(localSphere.origin.z, -box.extensions.z / 2, box.extensions.z / 2) };

	boxToSphere = localSphere.origin - closestPoint;

	if (Dot(boxToSphere, boxToSphere) > sphere.r * sphere.r)
		return false;

	interPt = closestPoint;
	interNormal = (interPt - sphere.origin) / sphere.r;
	return true;
}

bool Core::Maths::ToolBox3D::Collision::cSegmentPlane(const Segment& s, const PlaneCollider& p, vec3& intersection, vec3& Normal)
{
	float segPlan = Dot(s.dir, p.Normal);
	if (segPlan == 0)
		return false;

	vec3 P = p.Normal * p.d;
	float t = Dot(P - s.start, p.Normal) / segPlan;

	if (t < 0 || t > 1)
		return false;

	//drawing intersection point
	intersection = (s.start + s.dir * t);
	//DrawSphere(intersection.rlVec, 0.1f, RED);
	//DrawLine3D(intersection.rlVec, (intersection + p.Normal).rlVec, BLUE);

	Normal = p.Normal;
	return true;
}

bool Core::Maths::ToolBox3D::Collision::cSegmentSphere(const Segment& seg, const SphereCollider& sph, vec3& intersection, vec3& Normal)
{
	float segMag = Magnitude(seg.dir);
	vec3 segSph = seg.start - sph.origin;
	float segSphMag = Magnitude(segSph);

	float a = segMag * segMag;
	float b = 2 * Dot(segSph, seg.dir);
	float c = segSphMag * segSphMag - sph.r * sph.r;

	float delta = b * b - 4 * a * c;

	if (delta < 0)
		return false;

	float t = (-b - sqrtf(delta)) / (2 * a);

	if (t < 0 || t > 1)
		return false;

	//drawing intersection point
	intersection = (segSph + seg.dir * t) + sph.origin;
	//DrawSphere(intersection.rlVec, 0.1f, RED);
	Normal = Normalize(intersection - sph.origin);
	//DrawLine3D(intersection.rlVec, (intersection + Normal).rlVec, BLUE);
	return true;
}

bool Core::Maths::ToolBox3D::Collision::cSegmentCylinderInf(const Segment& s, const CylinderCollider& cy, vec3& intersection, vec3& Normal)
{
	vec3 A = { 0.f + cy.h / 2, 0.f, 0.f };
	vec3 B = { 0.f - cy.h / 2, 0.f, 0.f };

	A = Vector3RotateByQuaternion(A, cy.q);
	B = Vector3RotateByQuaternion(B, cy.q);

	translate(A, (cy.pt1 + cy.pt2) / 2);
	translate(B, (cy.pt1 + cy.pt2) / 2);

	vec3 PQ = B - A;
	vec3 AB = s.dir;
	vec3 PA = s.start - A;

	float a = Dot(PQ, PQ) * Dot(AB, AB) - Dot(PQ, AB) * Dot(PQ, AB);
	float b = Dot(PQ, PQ) * Dot(PA, AB) - Dot(PQ, AB) * Dot(PA, PQ);
	float c = Dot(PQ, PQ) * (Dot(PA, PA) - cy.r * cy.r) - Dot(PA, PQ) * Dot(PA, PQ);

	float delta = b * b - a * c;

	if (delta < 0)
		return false;

	float t = (-b - sqrtf(delta)) / a;

	if (t < 0 || t > 1)
		return false;

	//drawing intersection point
	intersection = s.start + s.dir * t;
	//DrawSphere(intersection.rlVec, 0.1f, RED);
	vec3 PM = intersection - A;
	vec3 HM = (PQ * (Dot(PM, PQ))) / Dot(PQ, PQ) - PM;
	HM = Normalize(HM);
	Normal = HM;
	//DrawLine3D(intersection.rlVec, (intersection - HM).rlVec, BLUE);
	return true;
}

bool Core::Maths::ToolBox3D::Collision::cSegmentCylinder(const Segment& s, const CylinderCollider& c, vec3& intersection, vec3& Normal)
{
	if (!cSegmentCylinderInf(s, c, intersection, Normal))
		return false;

	vec3 A = { 0.f + c.h / 2, 0.f, 0.f };
	vec3 B = { 0.f - c.h / 2, 0.f, 0.f };

	A = Vector3RotateByQuaternion(A, c.q);
	B = Vector3RotateByQuaternion(B, c.q);

	translate(A, (c.pt1 + c.pt2) / 2);
	translate(B, (c.pt1 + c.pt2) / 2);

	vec3 PM = intersection - A;
	vec3 PQ = B - A;

	if (!(Dot(PM, PQ) < 0 || Dot(PM, PQ) > Dot(PQ, PQ)))
	{
		vec3 HM = PM - (PQ * (Dot(PM, PQ))) / Dot(PQ, PQ);
		HM = Normalize(HM);
		Normal = HM;

		return true;
	}

	PlaneCollider discA(-PQ, A);
	PlaneCollider discB(PQ, B);

	if (cSegmentPlane(s, discA, intersection, Normal))
	{
		PM = intersection - A;
		if (Dot(PM, PM) < c.r * c.r)
			return true;
	}
	else if (cSegmentPlane(s, discB, intersection, Normal))
	{
		PM = intersection - B;
		if (Dot(PM, PM) < c.r * c.r)
			return true;
	}

	return false;
}

bool Core::Maths::ToolBox3D::Collision::cSegmentQuad(const Segment& s, const Quad& q, vec3& intersection, vec3& Normal)
{
	PlaneCollider p(q.getNormal(), q.center);
	if (!cSegmentPlane(s, p, intersection, Normal))
		return false;

	if (intersection.x > q.center.x + q.extensions.x / 2.f || intersection.x < q.center.x - q.extensions.x / 2.f)
		return false;
	if (intersection.y > q.center.y + q.extensions.y / 2.f || intersection.y < q.center.y - q.extensions.y / 2.f)
		return false;
	if (intersection.z > q.center.z + q.extensions.y / 2.f || intersection.z < q.center.z - q.extensions.y / 2.f)
		return false;

	return true;
}

bool Core::Maths::ToolBox3D::Collision::cSegmentBox(const Segment& s, const BoxCollider& b, vec3& intersection, vec3& Normal)
{
	vec3 A = { 0.f + b.extensions.x / 2, 0.f, 0.f };
	vec3 B = { 0.f - b.extensions.x / 2, 0.f, 0.f };
	vec3 C = { 0.f, 0.f + b.extensions.y / 2, 0.f };
	vec3 D = { 0.f, 0.f - b.extensions.y / 2, 0.f };
	vec3 E = { 0.f, 0.f, 0.f + b.extensions.z / 2 };
	vec3 F = { 0.f, 0.f, 0.f - b.extensions.z / 2 };

	A = Vector3RotateByQuaternion(A, b.q);
	B = Vector3RotateByQuaternion(B, b.q);
	C = Vector3RotateByQuaternion(C, b.q);
	D = Vector3RotateByQuaternion(D, b.q);
	E = Vector3RotateByQuaternion(E, b.q);
	F = Vector3RotateByQuaternion(F, b.q);

	translate(A, b.center);
	translate(B, b.center);
	translate(C, b.center);
	translate(D, b.center);
	translate(E, b.center);
	translate(F, b.center);

	Quad quads[6];
	quads[0] = { A,
				 { b.extensions.y, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90 * DEG2RAD)), true };

	quads[1] = { B,
				 { b.extensions.y, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90 * DEG2RAD)), false };

	quads[2] = { C,
				 { b.extensions.x, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 0 * DEG2RAD)), false };

	quads[3] = { D,
				 { b.extensions.x, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 0 * DEG2RAD)), true };

	quads[4] = { E,
				 { b.extensions.x, b.extensions.y },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 1.f, 0.f, 0.f }, -90 * DEG2RAD)), true };

	quads[5] = { F,
				 { b.extensions.x, b.extensions.y },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 1.f, 0.f, 0.f }, -90 * DEG2RAD)), false };

	for (int i = 0; i < 6; ++i)
	{
		if (cSegmentQuad(s, quads[i], intersection, Normal))
			return true;
	}

	return false;
}

bool Core::Maths::ToolBox3D::Collision::cSegmentBox(const Segment& s, const BoxCollider& b, float radius, vec3& intersection, vec3& Normal)
{
	vec3 A = { 0.f + b.extensions.x / 2.f - radius, 0.f, 0.f };
	vec3 B = { 0.f - b.extensions.x / 2.f + radius, 0.f, 0.f };
	vec3 C = { 0.f, 0.f + b.extensions.y / 2.f - radius, 0.f };
	vec3 D = { 0.f, 0.f - b.extensions.y / 2.f + radius, 0.f };
	vec3 E = { 0.f, 0.f, 0.f + b.extensions.z / 2.f - radius };
	vec3 F = { 0.f, 0.f, 0.f - b.extensions.z / 2.f + radius };

	A = Vector3RotateByQuaternion(A, b.q);
	B = Vector3RotateByQuaternion(B, b.q);
	C = Vector3RotateByQuaternion(C, b.q);
	D = Vector3RotateByQuaternion(D, b.q);
	E = Vector3RotateByQuaternion(E, b.q);
	F = Vector3RotateByQuaternion(F, b.q);

	translate(A, b.center);
	translate(B, b.center);
	translate(C, b.center);
	translate(D, b.center);
	translate(E, b.center);
	translate(F, b.center);

	Quad quads[6];
	quads[0] = { A,
				 { b.extensions.y, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90.f * DEG2RAD)), true };

	quads[1] = { B,
				 { b.extensions.y, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 0.f, 1.f }, 90.f * DEG2RAD)), false };

	quads[2] = { C,
				 { b.extensions.x, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 0.f * DEG2RAD)), false };

	quads[3] = { D,
				 { b.extensions.x, b.extensions.z },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 0.f, 1.f, 0.f }, 0.f * DEG2RAD)), true };

	quads[4] = { E,
				 { b.extensions.x, b.extensions.y },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 1.f, 0.f, 0.f }, -90.f * DEG2RAD)), true };

	quads[5] = { F,
				 { b.extensions.x, b.extensions.y },
				 QuaternionMultiply(b.q, QuaternionFromAxisAngle({ 1.f, 0.f, 0.f }, -90.f * DEG2RAD)), false };

	for (int i = 0; i < 6; ++i)
	{
		if (cSegmentQuad(s, quads[i], intersection, Normal))
			return true;
	}

	return false;
}
//
//bool Core::Maths::ToolBox3D::Collision::cSegmentCapsule(const Segment& s, const CapsuleCollider& c, vec3& intersection, vec3& Normal)
//{
//	vec3 A = { 0.f + c.h / 2, 0.f, 0.f };
//	vec3 B = { 0.f - c.h / 2, 0.f, 0.f };
//
//	A = Vector3RotateByQuaternion(A, c.q);
//	B = Vector3RotateByQuaternion(B, c.q);
//
//	translate(A, (c.pt1 + c.pt2) / 2);
//	translate(B, (c.pt1 + c.pt2) / 2);
//
//	CylinderCollider cy((c.pt1 + c.pt2) / 2, c.q, c.r, c.h, false);
//
//	if (cSegmentCylinder(s, cy, intersection, Normal))
//		return true;
//	else
//	{
//		SphereCollider sph1 = { A, c.r };
//		SphereCollider sph2 = { B, c.r };
//
//		if (cSegmentSphere(s, sph1, intersection, Normal))
//			return true;
//		if (cSegmentSphere(s, sph2, intersection, Normal))
//			return true;
//	}
//	Normal = { 0,0,0 };
//
//	return false;
//}