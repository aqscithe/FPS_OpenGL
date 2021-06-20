#include "Toolbox2D.h"
#include "Debug.h"
#include "PhysicsManager.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

using namespace Core::Maths;

Toolbox2D::Box::Box()
{
	shader = Resources::ResourcesManager::getInstance()->addResource<Shader>("shaderButton");
	cameraManager = LowRenderer::CameraManager::getInstance();
}

void Toolbox2D::Box::loadTexture(std::string path)
{
	texture = Resources::ResourcesManager::getInstance()->addResource<Texture>(path);
}

void Toolbox2D::Box::start()
{
	transform = getComponent<Transform>();
}

void Toolbox2D::Box::update()
{
	color = { 0.5f, 0.5f, 0.5f };

	center = transform->pos.xy;
	vec2 tmp = center;

	double mousePosX, mousePosY;
	int width, height;

	GLFWwindow* window = Core::API::Application::getInstance()->window;
	glfwGetWindowSize(window, &width, &height);

	glfwGetCursorPos(window, &mousePosX, &mousePosY);
	vec2 mousePos = { (float)mousePosX, (float)mousePosY };
	mousePos.x = Toolbox2D::map(mousePos.x, 0.f, (float)width, 0.f, 1.f);
	mousePos.y = Toolbox2D::map(mousePos.y, 0.f, (float)height, 0.f, 1.f);
	mousePos.y = -mousePos.y;

	center.x = Toolbox2D::map(center.x, -1.f, 1.f, 0.f, 1.f);
	center.y = Toolbox2D::map(center.y, 0.5f, -0.5f, 0.f, -1.f);


	if (Toolbox2D::pointBoxIntersection(mousePos, *this))
	{
		color = { 1.f, 1.f, 1.f };

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
		{
			switch (option)
			{
			case 0 :
				Core::API::Application::getInstance()->graph->loadScene("level.scn");
				break;
			case 1:
				break;
			case 2:
				Core::API::Application::getInstance()->graph->loadScene("settings.scn");
				break;
			case 3:
				Core::API::Application::getInstance()->graph->loadScene("mainMenu.scn");
				break;
			default:
				glfwSetWindowShouldClose(window, true);
				break;
			}
		}
	}

	center = tmp;
	draw();
}

bool Toolbox2D::Box::updateIMGUI()
{
	if (ImGui::TreeNode("2D Box"))
	{
		static char texturePath[150];
		std::string path = texturePath;

		ImGui::Text("Texture Path : Resources\\assets\\");
		ImGui::InputText("Texture", texturePath, IM_ARRAYSIZE(texturePath));

		ImGui::SliderFloat("width", &w, 0.f, 10.f);
		ImGui::SliderFloat("height", &h, 0.f, 10.f);
		ImGui::ColorEdit3("color", color.e);
		ImGui::SliderInt("option", &option, 0, 4);

		if (ImGui::Button("change texture"))
		{
			loadTexture("Resources//assets//" + path);
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

void Toolbox2D::Box::draw()
{
	float vertices[] = {
		w + center.x,  h + center.y, 0.f, 1.f, 1.f,
		w + center.x, -h + center.y, 0.f, 1.f, 0.f,
	   -w + center.x, -h + center.y, 0.f, 0.f, 0.f,
	   -w + center.x,  h + center.y, 0.f, 0.f, 1.f,
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
	//Color
	shader->setVec3("myColor", color);

	if (texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, texture->data);

	glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6 * sizeof(float), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &EBO);
}

void Toolbox2D::Box::save(std::ofstream& sceneFile)
{
	sceneFile << "2D Box\n";
	sceneFile << "width " << w << "\n";
	sceneFile << "height " << h << "\n";
	sceneFile << "option " << option << "\n";
	sceneFile << "texture " << texture->getResourcePath() << "\n";
}