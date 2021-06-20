#include "Toolbox2D.h"

using namespace Core::Maths;

vec2 Toolbox2D::Normalize(const vec2& v)
{
    vec2 result = v;

    float length, ilength;
    length = Magnitude(v);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f / length;

    result.x *= ilength;
    result.y *= ilength;

    return result;
}

float Toolbox2D::Magnitude(const vec2& v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

bool Toolbox2D::pointBoxIntersection(const vec2& pt, const Toolbox2D::Box& box)
{
    if (pt.x >= box.center.x - box.w / 2.f && pt.x < box.center.x + box.w / 2.f
        && pt.y >= box.center.y - box.h / 2.f && pt.y < box.center.y + box.h / 2.f)
        return true;

    return false;
}