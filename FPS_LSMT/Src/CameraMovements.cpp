#include "CameraMovements.h"
#include "Application.h"
#include "ToolBox3D.h"

using namespace Gameplay::Entity;

void CameraMovements::start()
{
	inputs = InputManager::getInstance();
	transform = getComponent<Transform>();
	time = &Core::API::Application::getInstance()->time;
}
 
void CameraMovements::update()
{
	if (transformCamera == nullptr)
		transformCamera = getComponentInChildren<Transform>();

	if(inputs->mouseCaptured)
	{
		transform->rotation.y -= inputs->mouseDeltaX * time->deltaTime();
		transformCamera->rotation.x -= inputs->mouseDeltaY * time->deltaTime();
		transformCamera->rotation.x = Clamp(transformCamera->rotation.x, -90.f, 90.f);
	}
}

bool CameraMovements::updateIMGUI()
{
	if (ImGui::TreeNode("CameraMovements"))
	{

		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void CameraMovements::save(std::ofstream& sceneFile)
{
	sceneFile << "CameraMovements" << "\n";
}
