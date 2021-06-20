#pragma once

#include "Component.h"
#include <string>

namespace LowRenderer
{
	class GameObject
	{
	public :
		GameObject();
		~GameObject();
		void update();
		bool updateIMGUI();
		void save(std::ofstream& sceneFile);
		Resources::Components<Resources::Component>* components;

		void deleteGameObjectWithChildren();
		void deleteGameObject();

		void setId(int _id) { id = _id; }
		int getId() { return id; }

		void setName(std::string _name) { name = _name; }
		std::string getName() { return name; }

	private :
		int id;
		std::string name;
	};

	class GameObjectManager
	{
	public : 
		void update();
		void updateIMGUI();

		GameObject* addGameObject();
		void addGameObject(GameObject* gameObject);
		void updateGameObjectId();
		void deleteGameObject(int id);
		void deleteGameObjectWithChildren(int id);
		void deleteAllGameObject();
		void saveGameObjects(std::ofstream& sceneFile);

		std::vector<GameObject*> gameObjects;
	};
}