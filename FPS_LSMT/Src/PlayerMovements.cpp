#include "PlayerMovements.h"
#include "Application.h"
using namespace Gameplay::Entity;

void PlayerMovements::start()
{
	rigidbody = getComponent<Rigidbody>();
	transform = getComponent<Transform>();
	input = InputManager::getInstance();
	cam = CameraManager::getInstance()->getActiveCamera();
}

void PlayerMovements::update()
{
	cam = CameraManager::getInstance()->getActiveCamera();

    if (!Application::getInstance()->editMode)
    {
	    float yaw = cam->getYaw();
	    float forwardmovement = (float)(input->down - input->up);
	    float strafemovement = (float)(input->right - input->left);

        float x = -forwardmovement * sinf(yaw);
	    float z = forwardmovement * cosf(yaw);
	    x += strafemovement * cosf(yaw);
	    z += strafemovement * sinf(yaw);

	    if (rigidbody->inAir)
	    	rigidbody->addForce(vec3{ x, 0.f, z } * airSpeed, ForceMode::Force);
	    else
	    	rigidbody->addForce(vec3{ x, 0.f, z } * speed, ForceMode::Assign);

	    if (!rigidbody->inAir && input->space)
	    	rigidbody->addForce(vec3{ 0,1,0 } * jumpForce, ForceMode::Assign);
    }

	// replace player when they fall
	if (transform->pos.y <= -5)
	{
		transform->pos = vec3{ 0,5,0 };
		rigidbody->velocity = vec3{ 0,0,0 };
	}
}

bool PlayerMovements::updateIMGUI()
{
	if (ImGui::TreeNode("PlayerMovements"))
	{
		ImGui::Text("on the ground : %d", !rigidbody->inAir);
		ImGui::DragFloat("speed", &speed);
		ImGui::DragFloat("jumpForce", &jumpForce);

		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void PlayerMovements::save(std::ofstream& sceneFile)
{
	sceneFile << "PlayerMovements\n";
	sceneFile << "speed " << speed << "\n";
	sceneFile << "jumpForce " << jumpForce << "\n";
}
