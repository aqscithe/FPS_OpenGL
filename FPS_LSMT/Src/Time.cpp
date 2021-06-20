#include "Times.h"
#include <iostream>
#include <GLFW/glfw3.h>

using namespace Core::API;

float& Time::timeScale()
{
	return ts;
}

float Time::deltaTime()
{
	return dt * ts;
}

float Time::fixedDeltaTime()
{
	return fdt * ts;
}

int Time::fixing()
{
	return f == 0 ? 1 : f;
}

void Time::resetLastTime()
{
	lastTime = (float)glfwGetTime();
}

void Time::update()
{
	float curTime = (float)glfwGetTime();

	// delta time
	// ----------
	dt = curTime - lastTime;
	lastTime = curTime;

	// fixed delta time
	// ----------------
	f = 0;
	float temp = dt;
	while (temp > fdt)
	{
		temp -= fdt;

		++f;
	}
}