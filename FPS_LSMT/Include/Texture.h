#pragma once
#include "ResourcesManager.h"

namespace Resources
{
	class Texture : public Resource
	{
	public:
		void loadResource() override;
		void unloadResource() override {}

		unsigned int data = 0;
	};
}