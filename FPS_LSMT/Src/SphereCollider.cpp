#include "Collider3D.h"
#include "ToolBox3D.h"
#include "PhysicsManager.h"
#include "Debug.h"
#include <imgui.h>

using namespace Core::Maths::ToolBox3D;
using namespace Core::Maths;

void SphereCollider::start()
{
	transform = getComponent<Transform>();
	PhysicsManager::getInstance()->addSphereCollider(this);
}

void SphereCollider::update()
{
	TransformColliderUpdate();

	if (drawCollider)
		DrawCollider();
}

bool SphereCollider::updateIMGUI()
{
	if (ImGui::TreeNode("Sphere Collider"))
	{
		ImGui::Checkbox("isTrigger", &isTrigger);
		ImGui::Checkbox("draw collider", &drawCollider);
		ImGui::DragFloat3("Center", localPosition.e);
		ImGui::DragFloat("Radius", &r);
		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void SphereCollider::TransformColliderUpdate()
{
	origin = transform->pos + localPosition;
}

vec3 getSphereCoordinates(const float& r, const float& phi, const float& theta)
{
	return {
		r * sinf(theta) * cosf(phi),
		r * sinf(theta) * sinf(phi),
		r * cosf(theta)
	};
}

void SphereCollider::DrawCollider()
{
	std::vector<float> sphereVertices;
	std::vector<int> sphereIndices;
	const int segment = 15;

	for (int y = 0; y <= segment; y++)
	{
		float theta = (y / (float)segment) * PI;

		for (int x = 0; x <= segment; x++)
		{
			float phi = (x / (float)segment) * 2.f * PI;

			vec3 pos = getSphereCoordinates(r, phi, theta);
			sphereVertices.push_back(pos.x + localPosition.x);
			sphereVertices.push_back(pos.z + localPosition.y);
			sphereVertices.push_back(pos.y + localPosition.z); 
		}
	}


	for (int i = 0; i < segment; i++)
	{
		for (int j = 0; j < segment; j++)
		{
			sphereIndices.push_back(i * (segment + 1) + j);
			sphereIndices.push_back((i + 1) * (segment + 1) + j);
			sphereIndices.push_back((i + 1) * (segment + 1) + j + 1);
			sphereIndices.push_back(i * (segment + 1) + j);
			sphereIndices.push_back((i + 1) * (segment + 1) + j + 1);
			sphereIndices.push_back(i * (segment + 1) + j + 1);
		}
	}

	unsigned int VBO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	if (m_VAO == 0)
		glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), sphereIndices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

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
	shader->setVec3("myColor", { 0,1,0 });

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(m_VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, segment * segment * 6, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void SphereCollider::save(std::ofstream& sceneFile)
{
	sceneFile << "SphereCollider\n";
	sceneFile << "isTrigger " << isTrigger << "\n";
	sceneFile << "drawCollider " << drawCollider << "\n";
	sceneFile << "localPos " << localPosition.x << " " << localPosition.y << " " << localPosition.z << "\n";
	sceneFile << "radius " << r << "\n";
}

SphereCollider::~SphereCollider()
{
	PhysicsManager::getInstance()->deleteSphereCollider(this);
}