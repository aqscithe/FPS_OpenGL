#pragma once
#include "Maths.h"
#include "ResourcesManager.h"	

using namespace Core::Maths;

namespace Resources
{
	class Vertex
	{
	public :
		vec3 vertices;
		vec2 uvs;
		vec3 normals;
	};


	class Mesh : public Resource
	{
		void loadResource() override {}
		void unloadResource() override {}
	public :
		std::vector<Vertex> vertexs;
		GLuint VAO = 0;

		void loadVAO();

		std::string meshName;
		std::string texturePath;
		std::string mtlName;
	};

	void loadMesh(const std::string& meshPath, std::vector< std::shared_ptr<Mesh>>& meshs);
}