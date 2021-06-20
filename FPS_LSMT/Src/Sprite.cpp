#include "Sprite.h"
#include <imgui.h>
#include "Debug.h"

using namespace Gameplay::UI;

void Sprite::start()
{
	transform = getComponent<Transform>();
	shader = Resources::ResourcesManager::getInstance()->addResource<Shader>("shaderSprite");
	cameraManager = LowRenderer::CameraManager::getInstance();
}

void Sprite::loadTexture(std::string path)
{
	texture = Resources::ResourcesManager::getInstance()->addResource<Texture>("Resources//assets//" + path);
}

void Sprite::update()
{
	vec3 scale = transform->scale;

	float vertices[] = {
		 scale.x,  scale.y, 0.f, 1.f, 1.f,
		 scale.x, -scale.y, 0.f, 1.f, 0.f,
		-scale.x, -scale.y, 0.f, 0.f, 0.f,
		-scale.x,  scale.y, 0.f, 0.f, 1.f,
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	unsigned int VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	if (m_VAO == 0)
		glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);

	Core::Debug::Assertion::assertShrdPtr(shader);
	shader->use();
	LowRenderer::Camera* activeCamera(cameraManager->getActiveCamera());

	//Matrix view * Projection matrixViewProj
	shader->setMatrix4("viewProj", activeCamera->getViewProj());

	//Camera position
	shader->setVec3("camPos", activeCamera->getPosition());

	//Matrix Model
	shader->setMatrix4("model", transform->matrixModel);

	//Aplha
	shader->setFloat("alpha", alpha);

	if (texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, texture->data);

	glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &EBO);
}

bool Sprite::updateIMGUI()
{
	if (ImGui::TreeNode("Sprite"))
	{
		static char texturePath[150];
		std::string path = texturePath;

		ImGui::Text("Texture Path : Resources\\assets\\");
		ImGui::InputText("Texture", texturePath, IM_ARRAYSIZE(texturePath));
		ImGui::SliderFloat("Alpha", &alpha, 0.f, 1.f);

		if (ImGui::Button("change texture"))
		{
			loadTexture(path);
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

void Sprite::save(std::ofstream& sceneFile)
{
	sceneFile << "Sprite" << "\n";
	sceneFile << "texture " << texture->getResourcePath() << "\n";
	sceneFile << "alpha" << alpha << "\n";
}
