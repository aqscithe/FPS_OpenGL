#pragma once
#include "GLFW/glfw3.h"
#include "Maths.h"

using namespace Core::Maths;

namespace Core
{
	namespace API
	{
		class InputManager
		{
		public : 

			static InputManager* getInstance();

			bool esc	= false;
			bool space	= false;
			bool left	= false; 
			bool right	= false;
			bool up		= false;
			bool down	= false;
			bool ctrl   = false;
			bool lKey	= false;
			bool rKey	= false;
			bool key8	= false;
			bool key8_down	= false;

			bool mlb	= false;	//mouse left button
			bool mlb_down = false;
			bool mrb	= false;	//mouse right button
			bool mouseCaptured = false;

			bool holdSpace = false;
			bool holdLKey = false;
			bool holdRKey = false;

			float mouseDeltaX = 0.f;
			float mouseDeltaY = 0.f;

			int SPACE	= GLFW_KEY_SPACE;
			int LEFT	= GLFW_KEY_A;
			int RIGHT	= GLFW_KEY_D;
			int UP		= GLFW_KEY_W;
			int DOWN	= GLFW_KEY_S;
			int CTRL	= GLFW_KEY_LEFT_CONTROL;

			vec2 mousePos = {};
			vec2 deltaMousePos = {};

			void updateInput(GLFWwindow* window);
		private : 
			static InputManager* instance;
		};
	}
}
