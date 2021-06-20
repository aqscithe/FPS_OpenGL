#pragma once

#include "Collider3D.h"

namespace Core
{
	namespace Maths
	{
		namespace ToolBox3D
		{
			namespace Collision
			{
                bool IntersectionSegmentPlane(const vec3& a, const vec3& b, const PlaneCollider& plane, vec3& interPt, vec3& interNormal);
                bool IntersectionSegmentSphere(const vec3& a, const vec3& b, const SphereCollider& sphere, vec3& interPt, vec3& interNormal);
                bool IntersectionSegmentCylinder(const vec3& a, const vec3& b, const CylinderCollider& cylinder, vec3& interPt, vec3& interNormal);
                bool IntersectionSegmentCapsule(const vec3& a, const vec3& b, CapsuleCollider& capsule, vec3& interPt, vec3& interNormal);
                bool IntersectionSegmentDisk(const vec3& a, const vec3& b, const DiskCollider& disk, vec3& interPt, vec3& interNormal);
                bool IntersectionSegmentQuad(const vec3& a, const vec3& b, const QuadCollider& quad, vec3& interPt, vec3& interNormal);
				bool IntersectionSegmentBox(const vec3& a, const vec3& b, const BoxCollider& box, vec3& interPt, vec3& interNormal, Quaternion* exitQ = nullptr);
                bool IntersectionSegmentRoundedBox(const vec3& a, const vec3& b, const BoxCollider& box, vec3& interPt, vec3& interNormal);
                bool IntersectionBoxSphere(const SphereCollider& sphere, const BoxCollider& box);
                bool IntersectionBoxBox(const BoxCollider& box1, const BoxCollider& box2);

                bool IntersectionOrientedBoxBox(const BoxCollider& box1, const BoxCollider& box2);
                bool CollisionBoxSphere(const SphereCollider& sphere, const BoxCollider& box, vec3& interPt, vec3& interNormal);

				bool cSegmentPlane(const Segment& s, const PlaneCollider& p, vec3& intersection, vec3& normal);

				bool cSegmentSphere(const Segment& seg, const SphereCollider& sph, vec3& intersection, vec3& normal);

				bool cSegmentCylinderInf(const Segment& s, const CylinderCollider& cy, vec3& intersection, vec3& normal);
				bool cSegmentCylinder(const Segment& s, const CylinderCollider& c, vec3& intersection, vec3& normal);

				bool cSegmentQuad(const Segment& s, const Quad& q, vec3& intersection, vec3& normal);

				bool cSegmentBox(const Segment& s, const BoxCollider& b, vec3& intersection, vec3& normal);
				bool cSegmentBox(const Segment& s, const BoxCollider& b, float radius, vec3& intersection, vec3& normal);

				//bool cSegmentCapsule(const Segment& s, const CapsuleCollider& c, vec3& intersection, vec3& normal);

				//bool cSegmentRoundedBox(const Segment& s, const RoundedBoxCollider& rb, vec3& intersection, vec3& normal);

				//bool cBoxSphere(const BoxCollider& b, const SphereCollider& s, vec3& intersection);
				//bool cSphereSphere(const SphereCollider& s1, const SphereCollider& s2);
			}
		}
	}
}