#pragma once
#include "ResourcesManager.h"
#include <array>
using namespace Resources;

namespace Resources
{
	class Cubemap : public Resource
	{
	private:
		void loadResource() override;
		void unloadResource() override {}

		std::array<std::string, 6> faces
		{
			"posx.jpg",
			"negx.jpg",
			"negy.jpg",
			"posy.jpg",
			"posz.jpg",
			"negz.jpg"
		};
		

	public :
		unsigned int data = 0;
	};
}
