#pragma once

#include "Components.h"

namespace LowRenderer
{
	class GameObject;
}

namespace Resources
{
	class Component
	{
	public:

		virtual ~Component() {};

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void fixedUpdate() = 0;
		virtual bool updateIMGUI() = 0;
		virtual void save(std::ofstream& sceneFile) = 0;
		bool deleteComponentIMGUI();

		void setComponents(Components<Component>* _components) { components = _components; }

		void setId(int _id) { id = _id; }
		int getId() { return id; }

	public:
		
		template<class T = Component>
		T* getComponent()
		{
			return components->getComponent<T>();
		}

		template<class T = Component>
		T* getComponentInChildren()
		{
			return components->getComponentInChildren<T>();
		}

		Components<Component>* getComponents() { return components; }

		template<class T>
		T* getGameObject();

	private :
		Components<Component>* components;
		int id = 0;
	};

}