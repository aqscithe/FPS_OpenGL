#include "Model.h"
#include "Components.h"
#include "Debug.h"

using namespace LowRenderer;

Model::Model() : resourceManager(ResourcesManager::getInstance()), cameraManager(CameraManager::getInstance()), lightsManager(LightsManager::getInstance())
{
	texture = nullptr;
	mesh = nullptr;
	material = nullptr;
	shader = nullptr;
}

Model::~Model()
{
	texture = nullptr;
	mesh = nullptr;
	material = nullptr;
	shader = nullptr;
}

void Model::start()
{
	transform = getComponent<Physics::Transform>();
}

void Model::update()
{
	Camera* activeCamera(cameraManager->getActiveCamera());

	if (shader == nullptr || mesh == nullptr || activeCamera == nullptr)
		return;

	lightsManager->lightsUpdateOnModel(shader);

	Core::Debug::Assertion::assertShrdPtr(shader);
	shader->use();


	//Matrix view * Projection matrixViewProj
	shader->setMatrix4("viewProj", activeCamera->getViewProj());

	//Camera position
	shader->setVec3("camPos", activeCamera->getPosition());

	//Matrix Model
	shader->setMatrix4("model", transform->matrixModel);

	if (material != nullptr)
	{
		Core::Debug::Assertion::assertShrdPtr(material);
		shader->setVec3("material.ambient", material->ambient);
		shader->setVec3("material.diffuse", material->diffuse);
		shader->setVec3("material.specular", material->specular);
		shader->setFloat("material.shininess", material->shininess);
	}

	if (texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, texture->data);

	Core::Debug::Assertion::assertShrdPtr(mesh);
	glBindVertexArray(mesh->VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh->vertexs.size());
	glBindVertexArray(0);
}

bool Model::updateIMGUI()
{
	if (ImGui::TreeNode("Model"))
	{
		if (mesh != nullptr)
		{
			ImGui::Text((new std::string("Mesh path : " + mesh->getResourcePath()))->c_str());
		}
		else
		{
			ImGui::Text("No mesh");
		}

		if (shader != nullptr)
		{
			ImGui::Text((new std::string("Shader path : " + shader->getResourcePath()))->c_str());
		}
		else
		{
			ImGui::Text("No shader");
		}

		if (texture != nullptr)
		{
			ImGui::Text((new std::string("Texture path : " + texture->getResourcePath()))->c_str());
		}
		else
		{
			ImGui::Text("No Texture");
		}

		if (ImGui::TreeNode("Material setting"))
		{
			if (material != nullptr)
			{
				ImGui::Text((new std::string("Material name : " + material->getResourcePath()))->c_str());
				ImGui::ColorEdit3("Ambient", material->ambient.e);
				ImGui::ColorEdit3("diffuse", material->diffuse.e);
				ImGui::ColorEdit3("specular", material->specular.e);
				ImGui::SliderFloat("shininess", &material->shininess, 0, 100);
			}
			else
			{
				ImGui::Text("No material");
			}

			ImGui::TreePop();
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

void Model::save(std::ofstream& sceneFile)
{
	sceneFile << "Model " << "\n";
	sceneFile << "Tex " << texture->getResourcePath() << "\n";
	sceneFile << "Mat " << material->getResourcePath() << "\n";
	sceneFile << "Shader " << shader->getResourcePath() << "\n";
	sceneFile << "meshNamePath " << mesh->getResourcePath() << "\n";
}