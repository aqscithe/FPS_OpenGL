#pragma once

#include "Maths.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace Core::Maths;

namespace Resources
{
	class Character
	{
	public:
		unsigned int	texture;
		vec2			size;		// h = x = rows, w = y = width
		vec2			bearing;	// offset from the world axis
		unsigned int	advance;
	};

	class Font
	{
	public:
		Character ch[128];

		int init();
	};
}