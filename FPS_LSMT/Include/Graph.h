#pragma once
#include "ResourcesManager.h"
#include "Scene.h"
#include "InputManager.h"

using namespace Resources;

namespace Core
{
	namespace DataStructure
	{
		class Graph
		{
		public :
			Graph();
			~Graph();

			void graphUpdate();
			void graphIMGUI(bool& editMode);

			ResourcesManager* resourcesManager;

			std::shared_ptr<Scene> scene;

			void loadScene(const char* scenePath);
			void newScene(const char* scenePath);

			void loadModel(const char* meshPath, const char* shaderPath);
		};
	}
}