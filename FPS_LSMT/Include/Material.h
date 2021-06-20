#pragma once

#include "Maths.h"
#include "ResourcesManager.h"

using namespace Core::Maths;

namespace Resources
{
	class Material : public Resource
	{
	public:
		void loadResource() override {}
		void unloadResource() override {}

		vec3 ambient = {}; 
		vec3 diffuse = {};
		vec3 specular = {};
		float shininess = 0;
	};
}