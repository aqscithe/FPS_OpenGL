#include "Transform.h"
#include "ToolBox3D.h"
#include "Application.h"
#include "GameObject.h"
#include <iostream>
#include <imgui.h>

using namespace Physics;
using namespace Core::Maths::ToolBox3D;

Physics::Transform::Transform()
{
	matrixModel = mat4x4::identity();
	pos = {};
	rotation = {};
	scale = { 1.f, 1.f, 1.f };
	transformParent = nullptr;
}

#include <iostream>
#include "Model.h"
void Physics::Transform::update()
{
	matrixModel = mat4x4::translate(pos) * 
		mat4x4::rotateZ(rotation.z * DEG2RAD) * 
		mat4x4::rotateX(rotation.x * DEG2RAD) * 
		mat4x4::rotateY(rotation.y * DEG2RAD) * 
		mat4x4::scale(scale);

	if (transformParent != nullptr)
	{
		matrixModel = transformParent->matrixModel * matrixModel;
	}
}

void Physics::Transform::setParent(Transform* _transform)
{
	if (_transform == nullptr)
	{
		transformParent = nullptr;
		return;
	}

	if (this == _transform)
		return;

	if (_transform->transformParent != this)
	{
		transformParent = _transform;
	}
	else
	{
		_transform->transformParent = nullptr;
		transformParent = _transform;
	}
}

bool Transform::updateIMGUI()
{

	if (ImGui::TreeNode("Transform"))
	{
		ImGui::DragFloat3("position", pos.e);
		ImGui::DragFloat3("rotation", rotation.e);
		ImGui::DragFloat3("scale", scale.e);

		Core::API::Application* api = Core::API::Application::getInstance();

		static int item_current_parentObject = 0;
		for (int i = 1; i <= api->graph->scene->gameObjectManager.gameObjects.size(); i++)
		{
			if (transformParent == api->graph->scene->gameObjectManager.gameObjects[i - 1]->components->getComponent<Transform>())
			{
				item_current_parentObject = i;
				break;
			}
			else
				item_current_parentObject = 0;
		}

		if (item_current_parentObject != 0)
			ImGui::Text("Transform Parent : %s", api->graph->scene->gameObjectManager.gameObjects[item_current_parentObject - 1]->getName().c_str());
		else
			ImGui::Text("Transform Parent : no parent");

		if (ImGui::TreeNode("Model parent settings"))
		{
			std::vector < std::string > gameObjectsName;
			std::vector < const char* > gameObjectsNameCstr;

			for (int i = 0; i < api->graph->scene->gameObjectManager.gameObjects.size(); i++)
				gameObjectsName.push_back(api->graph->scene->gameObjectManager.gameObjects[i]->getName());

			for (int i = 0; i < gameObjectsName.size(); i++)
				gameObjectsNameCstr.push_back(gameObjectsName[i].c_str());

			static int item_current_change = 0;
			ImGui::Combo("Change Parent", &item_current_change, gameObjectsNameCstr.data(), (int)gameObjectsNameCstr.size());
			if (ImGui::Button("Set parent"))
			{
				setParent(api->graph->scene->gameObjectManager.gameObjects[item_current_change]->components->getComponent<Transform>());
			}
			if (ImGui::Button("Set parent null"))
			{
				setParent(nullptr);
			}
			
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	return false;
}

void Transform::save(std::ofstream& sceneFile)
{
	sceneFile << "Transform " << "\n";
	sceneFile << "positon " << pos.x << " " << pos.y << " " << pos.z << "\n";
	sceneFile << "rotation " << rotation.x << " " << rotation.y << " " << rotation.z << "\n";
	sceneFile << "scale " << scale.x << " " << scale.y << " " << scale.z << "\n";

	if (transformParent != nullptr)
	{
		std::vector<GameObject*> gameObjects = Core::API::Application::getInstance()->graph->scene->gameObjectManager.gameObjects;
		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (transformParent == gameObjects[i]->components->getComponent<Transform>())
			{
				sceneFile << "parent " << i << "\n";
			}
		}
	}
	else
	{
		sceneFile << "parent " << "-1" << "\n";
	}
}