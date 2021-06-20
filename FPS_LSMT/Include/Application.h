#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Font.h"

#include "Debug.h"
#include "Graph.h"
#include "Times.h"

using namespace Core::DataStructure;

namespace LowRenderer
{
	class CameraManager;
}

namespace Core
{
	namespace API
	{

		const unsigned int SCR_WIDTH = 1000;
		const unsigned int SCR_HEIGHT = 800;

		class InputManager;

		class Application
		{
		public :
			Application();
			~Application();

			static Application* getInstance();

			GLFWwindow* window;
			Time time;
			Font font;

			InputManager* inputManager;
			CameraManager* cameraManager;

			int init();
			void update();

			Graph* graph;

			bool editMode = false;

		private :
			static Application* instance;
		};
	}
}