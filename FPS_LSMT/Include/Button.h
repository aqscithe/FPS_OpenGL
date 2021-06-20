#pragma once
#include "Box2D.h"
#include "Texture.h"

namespace Gameplay
{
	namespace UI
	{
		class Button
		{
		public:
			Core::Maths::Toolbox2D::Box* box;
			Resources::Texture* texture;
		};
	}
}