#include "Components.h"
#include "Transform.h"
#include "Collider3D.h"
#include "Application.h"
#include "Box2D.h"
#include "Rigidbody.h"
#include "Model.h"
#include "PlayerMovements.h"
#include "CameraMovements.h"
#include "ResourcesManager.h"
#include "PlayerFire.h"
#include "Player.h"
#include "Enemy.h"
#include "Skybox.h"
#include "Debug.h"
#include <imgui.h>
#include <string>

using namespace Resources;
using namespace Core::Maths::ToolBox3D;
using namespace Gameplay::Entity;
using namespace LowRenderer;

Components<Component>::Components()
{
}

Components<Component>::~Components()
{
	deleteAllComponents();
	components.clear();
}

void Components<Component>::startAllComponents()
{
	for (Component* component : startComponents)
	{
		component->start();
		component = startComponents[startComponents.size() - 1];
		startComponents.pop_back();
	}
}

void Components<Component>::updateComponents()
{
	if (this == nullptr)
		return;


	for (int i = 0; i < components.size(); i++)
	{
		//for (int f = Application::getInstance()->time.fixing(); f > 0; --f)
		//{
			components[i]->fixedUpdate();
		//}
	}

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->update();
	}
}

void Components<Component>::updateComponentsId()
{
	for (int i = 0; i < components.size(); i++)
		components[i]->setId(i);
}

void Components<Component>::deleteComponent(int id)
{
	//Core::Debug::Log::getInstance()->print("Delete Component ", Core::Debug::LogType::DEBUG);
	delete components[id];
	components[id] = components[components.size() - 1];
	components.pop_back();
	updateComponentsId();
}

void Components<Component>::getChildrenGameObjectComponents(std::vector<Components<Component>*>& gameObjectsComponents)
{
	ResourcesManager* resourcesManager = ResourcesManager::getInstance();
	GameObjectManager* gameObjectManager = &resourcesManager->getResource<Scene>()->gameObjectManager;
	
	std::vector<Transform*> transforms;
	for (int i = 0; i < gameObjectManager->gameObjects.size(); i++)
	{
		transforms.push_back(gameObjectManager->gameObjects[i]->components->getComponent<Transform>());
	}

	Transform* transform = getComponent<Transform>();
	for (int i = 0; i < transforms.size(); i++)
	{
		if (transforms[i]->transformParent == transform)
		{
			Components<Component>* cmp = transforms[i]->getComponents();
			if (cmp != this)
			{
				gameObjectsComponents.push_back(cmp);
			}
		}
	}
}

void Components<Component>::save(std::ofstream& sceneFile)
{
	sceneFile << "Components \n";
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->save(sceneFile);
	}
}


void Components<Component>::updateComponentsIMGUI()
{
	if (ImGui::TreeNode("Components"))
	{
		for (auto& component : components)
		{
				bool deleteComponents = component->updateIMGUI();
				if (deleteComponents)
					deleteComponent(component->getId());
		}

		static int item_current_component = 0;
		const char* items[] = { "Camera", "Light", "Model", "SphereCollider", "BoxCollider", "Rigidbody",
			"PlayerMovements", "2D Box", "CameraMovements", "PlayerFire", "Player", "Enemy", "Skybox"};
		ImGui::Combo("Component", &item_current_component, items, 13);
		if (ImGui::Button("Add Component"))
		{ 
			switch (item_current_component)
			{
			case 0 :
				addComponent<Camera>();
				break;
			case 1 :
				addComponent<Light>();
				break;
			case 2 :
				addComponent<Model>();
				break;
			case 3:
				addComponent<SphereCollider>();
				break;
			case 4:
				addComponent<BoxCollider>();
				break;
			case 5:
				addComponent<Rigidbody>();
				break;
			case 6:
				addComponent<Gameplay::Entity::PlayerMovements>();
				break;
			case 7:
				addComponent<Core::Maths::Toolbox2D::Box>();
				break;
			case 8:
				addComponent<CameraMovements>();
				break;
			case 9:
				addComponent<PlayerFire>();
				break;
			case 10:
				addComponent<Player>();
				break;
			case 11:
				addComponent<Gameplay::Entity::Enemy>();
				break;
			case 12:
				addComponent<Skybox>();
				break;

			default:
				break;
			}
		}

		ImGui::TreePop();
	}
}

bool Component::deleteComponentIMGUI()
{
	if (ImGui::Button("Delete Component"))
		return true;
	return false;
}

template<>
GameObject* Component::getGameObject<GameObject>()
{
	ResourcesManager* resourcesManager = ResourcesManager::getInstance();
	GameObjectManager* gameObjectManager = &resourcesManager->getResource<Scene>()->gameObjectManager;

	for (int i = 0; i < gameObjectManager->gameObjects.size(); i++)
	{
		if (gameObjectManager->gameObjects[i]->components == components)
			return gameObjectManager->gameObjects[i];
	}

	return nullptr;
}
