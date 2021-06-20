#pragma once

#include "ResourcesManager.h"
#include "Maths.h"
#include "GameObject.h"
#include "InputManager.h"
#include <vector>

using namespace Core::Maths;
using namespace LowRenderer;

namespace Resources
{
	class Scene : public Resource
	{
	public:
		Scene();
		~Scene() {}

		void loadResource() override;
		void unloadResource() override;

		ResourcesManager* resourcesManager;
		GameObjectManager gameObjectManager;
		double	mouseX = 0.0;
		double	mouseY = 0.0;
		int		nbrEnemies = 0;

		void updateMouse();
		
		void sceneUpdate();
		void saveScene();
		//void loadModel(std::ifstream& in, std::string& line, std::vector<int>& parentIndexs);

	private:

		void updateMousePos(GLFWwindow* window, Core::API::InputManager* inputs);
		void verifyMouseCapture(GLFWwindow* window, Core::API::InputManager* inputs);
	};
}