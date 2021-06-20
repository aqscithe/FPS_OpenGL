#pragma once
#include "Box2D.h"

namespace Core
{
	namespace Maths
	{
		namespace Toolbox2D
		{
			template <class T>
			T map(const T& value, const T& min1, const T& max1, const T& min2, const T& max2)
			{
				return (value - min1) / (max1 - min1) * (max2 - min2) + min2;
			}

			bool pointBoxIntersection(const vec2& pt, const Box& box);
			vec2 Normalize(const vec2& v);
			float Magnitude(const vec2& v);
		}
	}
}