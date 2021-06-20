#include "Mesh.h"
#include "Debug.h"
#include "Material.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace Resources;
using namespace Core::Debug;

void loadMtl(std::string mtlPath, std::unordered_map<std::string, std::string>& texturePaths, ResourcesManager* resourcesManager)
{
	std::ifstream in("Resources\\assets\\" + mtlPath, std::ios::in);
	if (!in)
	{
		Log::getInstance()->print("mtl file cannot open " + mtlPath, LogType::ERROR);
	}
	Log::getInstance()->print("mtl file open " + mtlPath, LogType::DEBUG);


	std::string line;

	int indexSubMesh = 0;

	std::string nameMtl;
	std::string texturePath;

	std::shared_ptr<Material> material = nullptr;

	while (std::getline(in, line))
	{
		if (line.substr(0, 7) == "newmtl ")
		{

			std::istringstream mtl(line.substr(7));
			mtl >> nameMtl;
			material = resourcesManager->addResource<Material>(nameMtl);
		}
		else if (line.substr(0, 7) == "map_Kd ")
		{
			std::istringstream map(line.substr(7));
			map >> texturePath;
			texturePaths.emplace(nameMtl, texturePath);
		}
		else if (line.substr(0, 3) == "Ns ")
		{
			std::istringstream ns(line.substr(3));
			float s;
			ns >> s;
			material->shininess = s;
		}
		else if (line.substr(0, 3) == "Ka ")
		{
			std::istringstream rgb(line.substr(3));
			float r, g, b;
			rgb >> r;
			rgb >> g;
			rgb >> b;
			material->ambient = { r,g,b };
		}
		else if (line.substr(0, 3) == "Kd ")
		{
			std::istringstream rgb(line.substr(3));
			float r, g, b;
			rgb >> r;
			rgb >> g;
			rgb >> b;
			material->diffuse = { r,g,b };
		}
		else if (line.substr(0, 3) == "Ks ")
		{
			std::istringstream rgb(line.substr(3));
			float r, g, b;
			rgb >> r;
			rgb >> g;
			rgb >> b;
			material->specular = { r,g,b };
		}
	}
}

void setSettings(const std::string& meshPath, const std::vector<Vertex>& vertexs, const std::string& texturePath, const std::string& meshName,
	const std::string& mtlName, ResourcesManager* resourcesManager, std::vector<std::shared_ptr<Mesh>>& meshs)
{
	std::shared_ptr<Mesh> mesh = resourcesManager->addResource<Mesh>(meshPath + "_" + meshName);

	mesh->vertexs = vertexs;
	mesh->texturePath = texturePath;
	mesh->meshName = meshName;
	mesh->mtlName = mtlName;

	mesh->loadVAO();

	meshs.push_back(mesh);
}

void orderVertex(std::vector<Vertex>& vertexs, const std::vector<int3>& indices, const std::vector<vec3>& vertices,
	const std::vector<vec2>& uvs, const std::vector<vec3>& normals)
{
	for (int i = 0; i < indices.size(); i++)
	{
		vertexs.push_back(Vertex{ vertices[indices[i].x], uvs[indices[i].y], normals[indices[i].z] });
	}
}


void Resources::loadMesh(const std::string& meshPath, std::vector< std::shared_ptr<Mesh>>& meshs)
{
	ResourcesManager* resourcesManager(ResourcesManager::getInstance());

	std::unordered_map<std::string, std::string> texturePaths;

	std::vector<int3> indices;
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;

	std::string meshName;
	std::string mtlName;

	//Log
	std::ifstream in("Resources\\assets\\" + meshPath, std::ios::in);
	if (!in)
		Log::getInstance()->print("Can not open mesh file: " + meshPath, LogType::ERROR);
	
	Log::getInstance()->print("Opening mesh file: " + meshPath, LogType::INFO);

	std::string line;

	bool firstMesh = true;

	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "o ")
		{
			if (!firstMesh)
			{
				std::vector<Vertex> vertexs;
				orderVertex(vertexs, indices, vertices, uvs, normals);
				setSettings(meshPath, vertexs, texturePaths.find(mtlName)->second, meshName, mtlName, resourcesManager, meshs);
				indices.clear();
			}

			std::istringstream o(line.substr(2));
			o >> meshName;
			firstMesh = false;
		}

		//Mtl 
		if (line.substr(0, 7) == "mtllib ")
		{
			std::string mtlPath = meshPath;
			mtlPath.resize(meshPath.length() - 3);
			mtlPath += "mtl";
			loadMtl(mtlPath, texturePaths, resourcesManager);
		}
		//Vertices
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream v(line.substr(2));

			float x, y, z;

			v >> x;
			v >> y;
			v >> z;

			vertices.push_back(vec3{ x, y, z });
		}

		//UVs
		if (line.substr(0, 2) == "vt")
		{
			std::istringstream vt(line.substr(3));

			float u, v;
			vt >> u;
			vt >> v;

			uvs.push_back(vec2{ u, v });
		}

		//Normas
		if (line.substr(0, 2) == "vn")
		{
			std::istringstream vn(line.substr(3));

			float x, y, z;
			vn >> x;
			vn >> y;
			vn >> z;

			normals.push_back(vec3{ x, y, z });
		}

		//Mtl use by mesh
		if (line.substr(0, 7) == "usemtl ")
		{
			std::istringstream usemtl(line.substr(7));
			usemtl >> mtlName;
		}

		//Indices Faces
		if (line.substr(0, 2) == "f ")
		{
			std::istringstream f(line.substr(2));

			for (int i = 0; i < 3; i++)
			{
				int index[3];

				for (int j = 0; j < 3; j++)
				{
					f >> index[j];
					f.ignore();
				}

				indices.push_back(int3(index[0] - 1, index[1] - 1, index[2] - 1));
			}

		}
	}

	std::vector<Vertex> vertexs;
	orderVertex(vertexs, indices, vertices, uvs, normals);
	setSettings(meshPath, vertexs, texturePaths.find(mtlName)->second, meshName, mtlName, resourcesManager, meshs);

	in.close();
}


void Mesh::loadVAO()
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexs.size(), vertexs.data(), GL_STATIC_DRAW);

	glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
}