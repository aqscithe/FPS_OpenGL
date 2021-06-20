#pragma once
#include <vector>
#include <memory>

namespace Resources
{
	template <class Component>
	class Components
	{
	public :
		Components();
		~Components();

		template<class T = Component>
		T* getComponent()
		{
			for (int i = 0; i < components.size(); i++)
			{
				if (T* ptr = dynamic_cast<T*>(components[i]))
					return dynamic_cast<T*>(components[i]);
			}

			return nullptr;
		}

		template<class T = Component>
		T* getComponentInChildren()
		{
			std::vector<Components<Component>*> gameObjectsComponents;
			getChildrenGameObjectComponents(gameObjectsComponents);

			for (int j = 0; j < gameObjectsComponents.size(); j++)
			{
				T* cmp = gameObjectsComponents[j]->getComponent<T>();
				if (cmp != nullptr)
					return cmp;
			}

			return nullptr;
		}

		template<class T = Component>
		T* addComponent()
		{
			Component* component = new T();
			component->setComponents(this);
			components.push_back(component);
			startComponents.push_back(component);
			updateComponentsId();
			return dynamic_cast<T*>(components.back());
		}

		void getChildrenGameObjectComponents(std::vector<Components<Component>*>& gameObjectsComponents);
		void updateComponents();
		void updateComponentsIMGUI();
		void updateComponentsId();
		void deleteComponent(int id);
		void deleteAllComponents()
		{
			for (int i = (int)components.size(); i > 0; i--)
			{
				deleteComponent(i - 1);
			}
		}

		void save(std::ofstream& sceneFile);
		void startAllComponents();

	private:
		std::vector<Component*> components;
		std::vector<Component*> startComponents;
	};
}