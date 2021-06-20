#include "GameObject.h"
#include "Transform.h"
#include "Debug.h"
#include "ResourcesManager.h"
#include "Scene.h"
#include <string>
#include <imgui.h>

using namespace LowRenderer;
using namespace Resources;
using namespace Core::Debug;

GameObject::GameObject() : id(0), name("")
{
	components = new Components<Component>();
	components->addComponent<Physics::Transform>();
}

GameObject::~GameObject()
{
	delete components;
}

void GameObject::update()
{
	components->updateComponents();
}

bool GameObject::updateIMGUI()
{

	components->updateComponentsIMGUI();

	if (ImGui::Button("Delete GameObject"))
		return true;

	return false;
}

void GameObject::save(std::ofstream& sceneFile)
{
	sceneFile << "GameObject \n";
	sceneFile << "Name " << name << "\n";
	components->save(sceneFile);
	sceneFile << "\n";
}

void GameObjectManager::update()
{
	int size = (int)gameObjects.size();

	for (int i = 0; i < size; i++)
	{
		gameObjects[i]->components->startAllComponents();
	}

	for (GameObject* gameObject : gameObjects)
	{
		//Security
		if (gameObjects.size() != size)
			return;

		gameObject->update();
	}
}


void GameObjectManager::updateIMGUI()
{
	for (GameObject* gameObject : gameObjects)
	{
		if (ImGui::TreeNode(gameObject->getName().c_str()))
		{
			if (gameObject->updateIMGUI())
				deleteGameObject(gameObject->getId());

			ImGui::TreePop();
		}
	}
}

void GameObjectManager::deleteGameObject(int id)
{
	if (gameObjects.size() < size_t(id) + size_t(1))
		return;
	delete gameObjects[id];
	gameObjects[id] = gameObjects[gameObjects.size() - 1];
	gameObjects.pop_back();
	updateGameObjectId();
}


void GameObjectManager::deleteAllGameObject()
{
	for (int i = (int)gameObjects.size() - 1; i >= 0; i--)
	{
		deleteGameObject(i);
	}

	gameObjects.clear();
}

void GameObjectManager::updateGameObjectId()
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->setId(i);
	}
}

GameObject* GameObjectManager::addGameObject()
{
	GameObject* gameObject = new GameObject;
	std::string gameObjectName = std::string("GameObject_" + std::to_string(gameObjects.size()));
	gameObject->setName(gameObjectName);
	gameObjects.push_back(gameObject);
	//Log::getInstance()->print("#Add GameObject " + gameObjectName, LogType::INFO);
	updateGameObjectId();

	return gameObject;
}

void GameObjectManager::addGameObject(GameObject* gameObject)
{
	std::string gameObjectName = std::string("GameObject_" + std::to_string(gameObjects.size()));
	gameObject->setName(gameObjectName);
	gameObjects.push_back(gameObject);
	//Log::getInstance()->print("#Add GameObject " + gameObjectName, LogType::INFO);
	updateGameObjectId();
}


void GameObjectManager::saveGameObjects(std::ofstream& sceneFile)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->save(sceneFile);
	}
}

void GameObjectManager::deleteGameObjectWithChildren(int id)
{
	std::vector<Components<Component>*> components;
	gameObjects[id]->components->getChildrenGameObjectComponents(components);

	for (int i = 0; i < components.size(); i++)
	{
		deleteGameObjectWithChildren(components[i]->getComponent()->getGameObject<GameObject>()->getId());
	}

	deleteGameObject(id);
}

void GameObject::deleteGameObject()
{
	ResourcesManager* resourcesManager = ResourcesManager::getInstance();
	GameObjectManager* gameObjectManager = &resourcesManager->getResource<Scene>()->gameObjectManager;

	gameObjectManager->deleteGameObject(id);
}

void GameObject::deleteGameObjectWithChildren()
{
	ResourcesManager* resourcesManager = ResourcesManager::getInstance();
	GameObjectManager* gameObjectManager = &resourcesManager->getResource<Scene>()->gameObjectManager;

	gameObjectManager->deleteGameObjectWithChildren(id);
}
