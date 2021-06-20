#include "Collider3D.h"
#include "Debug.h"
#include "ToolBox3D.h"
#include "PhysicsManager.h"
#include <GLFW/glfw3.h>
#include <imgui.h>

using namespace Core::Maths::ToolBox3D;

void BoxCollider::start()
{
    transform = getComponent<Transform>();
    PhysicsManager::getInstance()->addBoxCollider(this);
}

void BoxCollider::update()
{
	TransformColliderUpdate();

	if (drawCollider)
		DrawCollider();
}

bool BoxCollider::updateIMGUI()
{
	if (ImGui::TreeNode("Box Collider"))
	{
        ImGui::Checkbox("isTrigger", &isTrigger);
		ImGui::Checkbox("draw collider", &drawCollider);
        ImGui::DragFloat3("extensions", localExtensions.e);
        ImGui::DragFloat3("Center", localPosition.e);
        ImGui::DragFloat("friction", &friction);

        if (deleteComponentIMGUI())
        {
            ImGui::TreePop();
            return true;
        }

		ImGui::TreePop();
	}

    return false;
}

void BoxCollider::TransformColliderUpdate()
{
	center = transform->pos + localPosition;

    //using matrix
	//q = QuaternionFromMatrix(transform->matrixModel);
    //using euler rotation
    q = QuaternionFromEuler(transform->rotation.x * DEG2RAD, transform->rotation.y * DEG2RAD, -transform->rotation.z * DEG2RAD);
    unitX = Vector3RotateByQuaternion(unitX, q);
    unitY = Vector3RotateByQuaternion(unitY, q);
    unitZ = Vector3RotateByQuaternion(unitZ, q);

    extensions = vec3{ transform->scale.x, transform->scale.y, transform->scale.z } * localExtensions;
}


void BoxCollider::DrawCollider()
{
    float width  = localExtensions.x / 2.f;
    float heigth = localExtensions.y / 2.f;
    float length = localExtensions.z / 2.f;

    //float width  = extensions.x / 2.f;
    //float heigth = extensions.y / 2.f;
    //float length = extensions.z / 2.f;

    float vertices[] = {
     width + localPosition.x,  heigth + localPosition.y, -length + localPosition.z,
     width + localPosition.x, -heigth + localPosition.y, -length + localPosition.z,
    -width + localPosition.x, -heigth + localPosition.y, -length + localPosition.z,
    -width + localPosition.x,  heigth + localPosition.y, -length + localPosition.z,

     width + localPosition.x,  heigth + localPosition.y, length + localPosition.z,
     width + localPosition.x, -heigth + localPosition.y, length + localPosition.z,
    -width + localPosition.x, -heigth + localPosition.y, length + localPosition.z,
    -width + localPosition.x,  heigth + localPosition.y, length + localPosition.z,
    };

    unsigned int indices[] = {
        0, 1, 3,   
        1, 2, 3,   

        4, 5, 7,   
        5, 6, 7,   

        4, 5, 0,   
        5, 1, 0,   

        7, 6, 3,   
        6, 2, 3,   

        0, 4, 3,   
        3, 7, 4,   

        1, 5, 2,   
        2, 6, 5   
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void BoxCollider::save(std::ofstream& sceneFile)
{ 
    sceneFile << "BoxCollider\n";
    sceneFile << "isTrigger " << isTrigger << "\n";
    sceneFile << "drawCollider " << drawCollider << "\n";
    sceneFile << "localPos " << localPosition.x << " " << localPosition.y << " " << localPosition.z << "\n";
    sceneFile << "scale " << extensions.x << " " << extensions.y << " " << extensions.z << "\n";
}

BoxCollider::~BoxCollider()
{
    PhysicsManager::getInstance()->deleteBoxCollider(this);
}
