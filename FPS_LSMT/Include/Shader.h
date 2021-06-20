#pragma once
#include <string>
#include "ResourcesManager.h"
#include "Maths.h"

using namespace Core::Maths;

namespace Resources
{
	class Shader : public Resource
	{
	public:
		Shader();

		void loadResource() override;
		void unloadResource() override {}

		std::string parseShaderFile(std::string filename);
		unsigned int shaderProgram;

		void use();
		void setVec3(const char* name, const vec3& v);
		void setInt(const char* name, const int i);
		void setFloat(const char* name, const float f);
		void setMatrix4(const char* name, const mat4& m);
	};
}