#include "InputManager.h"

using namespace Core::API;

InputManager* InputManager::instance = 0;

InputManager* InputManager::getInstance()
{
	if (instance == 0)
	{

		instance = new InputManager();
	}

	return instance;
}

void InputManager::updateInput(GLFWwindow* window)
{
	esc = glfwGetKey(window, GLFW_KEY_ESCAPE);
	if (glfwGetKey(window, SPACE))
	{
		space = false;
		if (!holdSpace)
		{
			space = true;
			holdSpace = true;
		}
	}
	else
	{
		holdSpace = false;
	}
	left = glfwGetKey(window, LEFT);
	right = glfwGetKey(window, RIGHT);
	up = glfwGetKey(window, UP);
	down = glfwGetKey(window, DOWN);
	ctrl = glfwGetKey(window, CTRL);
	if (glfwGetKey(window, GLFW_KEY_L))
	{
		if (!holdLKey)
		{
			lKey = !lKey;
			holdLKey = true;
		}
	}
	else
	{
		holdLKey = false;
	}
	if (glfwGetKey(window, GLFW_KEY_R))
	{
		rKey = false;
		if (!holdRKey)
		{
			rKey = true;
			holdRKey = true;
		}
	}
	else
	{
		holdRKey = false;
	}

	bool mlb_tmp = mlb;
	mlb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
	
	if (!mlb_down && mlb && mlb != mlb_tmp)
	{
		mlb_down = true;
	}
	else if (mlb_down)
	{
		mlb_down = false;
	}

	bool key8_tmp = key8;
	key8 = glfwGetKey(window, GLFW_KEY_8);

	if (!key8_down && key8 && key8 != key8_tmp)
	{
		key8_down = true;
	}
	else if (key8_down)
	{
		key8_down = false;
	}

	mrb = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2);

	double newMouseX, newMouseY;
	glfwGetCursorPos(window, &newMouseX, &newMouseY);
	deltaMousePos.x = (float)(newMouseX - mousePos.x);
	deltaMousePos.y = (float)(newMouseY - mousePos.y);
	mousePos = { (float)newMouseX, (float)newMouseY };
}
