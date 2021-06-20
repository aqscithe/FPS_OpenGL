#include "Skybox.h"

using namespace Gameplay::Entity;

void Skybox::start()
{
	shader = ResourcesManager::getInstance()->addResource<Shader>("shaderCubemap");
	cameraManager = CameraManager::getInstance();

	unsigned int VBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

	glBindVertexArray(0);
}

void Skybox::update()
{
	if (cubemap == nullptr)
		return;

	Camera* activeCamera(cameraManager->getActiveCamera());


	glDepthMask(GL_FALSE);
	shader->use();

	shader->setMatrix4("viewProj", activeCamera->getViewProj());
	
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->data);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void Skybox::loadCubemap(std::string path)
{
	cubemap = Resources::ResourcesManager::getInstance()->addResource<Cubemap>("Resources//assets//" + path);
}

bool Skybox::updateIMGUI()
{
	if (ImGui::TreeNode("Skybox"))
	{
		static char texturePath[150];
		std::string path = texturePath;

		ImGui::Text("Texture Path : Resources\\assets\\");
		ImGui::InputText("Texture", texturePath, IM_ARRAYSIZE(texturePath));

		if (ImGui::Button("change cubemap"))
		{
			loadCubemap(path);
		}

		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void Skybox::save(std::ofstream& sceneFile)
{
	sceneFile << "Skybox" << "\n";
	sceneFile << "cubemap " << cubemap->getResourcePath() << "\n";
}