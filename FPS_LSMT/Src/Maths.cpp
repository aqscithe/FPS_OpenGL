#include <Maths.h>

using namespace Core;
/*
float Maths::toDegrees(const float& x)
{
	return x * 180.f / (TAU / 2);
}

float Maths::toRadians(const float& x)
{
	return x * TAU / 360.f;
}*/

void Core::Maths::translate(vec3& dest, const vec3& t)
{
	dest.x += t.x;
	dest.y += t.y;
	dest.z += t.z;
}
