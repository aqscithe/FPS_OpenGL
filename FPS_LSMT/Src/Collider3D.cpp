#include "Collider3D.h"
#include "ToolBox3D.h"

using namespace Core::Maths::ToolBox3D;

PlaneCollider::PlaneCollider(const vec3& Normal, const float& Distance)
{
	this->Normal = Normalize(Normal); 
	d = Distance; 
}


PlaneCollider::PlaneCollider(const vec3& Normal, const vec3& point)
{
	this->Normal = Normalize(Normal);
	d = Dot(point, Normal);
}

PlaneCollider::PlaneCollider(const vec3& a, const vec3& b, const vec3& c)
{
	Normal = Normalize(Cross(b - a, c - a));
	d = Dot(a, Normal);
}

CylinderCollider::CylinderCollider(const vec3& a, const Quaternion& q1, const float& radius, const float& height, const int finite)
{
	pt1 = a;
	q = q1;
	r = radius;
	h = height;	
	pt2 = { 0, height, 0 };
	pt2 = Vector3RotateByQuaternion(pt2, q);
	pt2 = pt1 + pt2;
	infinite = finite;
}

QuadCollider::QuadCollider(const vec3& position, const Quaternion& q1, const vec2& _extensions)
{
	pos = position;
	q = q1;
	extensions = _extensions;
}

BoxCollider::BoxCollider(const vec3& position, const Quaternion& q1, const vec3& _extensions)
{
	center = position;
	q = q1;
	extensions = _extensions;

	unitX = Vector3RotateByQuaternion({ 1,0,0 }, q);
	unitY = Vector3RotateByQuaternion({ 0,1,0 }, q);
	unitZ = Vector3RotateByQuaternion({ 0,0,1 }, q);
}

BoxCollider::BoxCollider(const vec3& position, const Quaternion& q1, const vec3& _extensions, const float& r)
{
	center = position;
	q = q1;
	extensions = _extensions;
	radius = r;

	unitX = Vector3RotateByQuaternion({ 1,0,0 }, q);
	unitY = Vector3RotateByQuaternion({ 0,1,0 }, q);
	unitZ = Vector3RotateByQuaternion({ 0,0,1 }, q);
}

SphereCollider::SphereCollider(const vec3& point, const Quaternion& q1, const float& radius, const float& mass)
{
	origin = point;
	q = q1;
	r = radius;
	m = mass;
}

SphereCollider::SphereCollider(const vec3& point, const Quaternion& q1, const float& radius)
{
	origin = point;
	q = q1;
	r = radius;
}

CapsuleCollider::CapsuleCollider(const vec3& a, const Quaternion& q1, const float& radius, const float& height)
{
	pt1 = a;
	r = radius;
	h = height;
	q = q1;
	pt2 = pt1 + Vector3RotateByQuaternion({ 0.f, 1.f, 0.f }, q) * h;
}

Core::Maths::ToolBox3D::Segment::Segment(const vec3& s, const vec3& e)
{
	start = s;
	end = e;
	dir = e - s;
}

Core::Maths::ToolBox3D::Quad::Quad(const vec3& center, const vec2& extensions, const Quaternion& q, const bool reverse)
	:center(center), extensions(extensions), q(q), reverse(reverse)
{

}

vec3 Core::Maths::ToolBox3D::Quad::getNormal() const
{
	//get quad's normal through its quaternion
	vec3 normal = { 0.f, 1.f, 0.f };
	normal = Vector3RotateByQuaternion(normal, q);

	if (reverse)
		normal = -normal;

	return normal;
}
