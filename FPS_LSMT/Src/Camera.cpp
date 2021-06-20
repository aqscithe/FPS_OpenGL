#include "Camera.h"
#include "Components.h"
#include "ToolBox3D.h"
#include "InputManager.h"
#include "Debug.h"
#include <fstream>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>

using namespace LowRenderer;
using namespace Core::Maths;
using namespace Core::Maths::ToolBox3D;
using namespace Core::Debug;

CameraManager* CameraManager::instance = 0;
CameraManager* CameraManager::getInstance()
{
	if (instance == 0)
	{
		instance = new CameraManager();
	}

	return instance;
}

Camera::Camera() : api(Core::API::Application::getInstance())
{
	isOrthographic = false;
	enabled = false;
	yaw = 0.f;
	pitch = 0.f;
	editYaw = 0.f;
	editPitch = 0.f;
	roll = 0.f;
	position = {};
	aspect = 1000 / 800;
	fovY = (60.f * DEG2RAD);
	near = 0.01f;
	far = 500.f;
	transform = nullptr;
}

void Camera::start()
{
	transform = getComponent<Physics::Transform>();
	setGameCamera();
}

Camera* CameraManager::getActiveCamera()
{
	return activeCamera;
}

void CameraManager::setActiveCamera()
{
	if (gameCamera != nullptr)
		activeCamera = editorCamera;

	if (Core::API::Application::getInstance()->editMode)
	{
		activeCamera = editorCamera;
		Log::getInstance()->print("Camera Mode: EDIT", LogType::INFO);
	}
	else
	{
		activeCamera = gameCamera;
		Log::getInstance()->print("Camera Mode: GAME", LogType::INFO);
	}
}


void CameraManager::updateEditorCamera()
{
	InputManager* inputs = InputManager::getInstance();

	if (inputs->mouseCaptured)
	{
		editorCamera->update();
	}
}

CameraManager::~CameraManager()
{
	delete editorCamera;
}

void Camera::setGameCamera()
{
	enabled = true;
	CameraManager* cameraManager = CameraManager::getInstance();
	cameraManager->setGameCamera(this);
	cameraManager->setActiveCamera();

	position = {};
	pitch = 0;
	yaw = 0;
	roll = 0;
}

void Camera::update()
{
	if (!api->editMode)
	{
		position = PositionMatrix(transform->matrixModel);
		vec3 rotationParent = {};
		if (transform->transformParent != nullptr)
			rotationParent = transform->transformParent->rotation;

		pitch = ((-transform->rotation.x - rotationParent.x) * DEG2RAD);
		yaw = ((transform->rotation.y - rotationParent.y) * DEG2RAD);
		roll = ((transform->rotation.z - rotationParent.z) * DEG2RAD);
	}
	else
	{
		InputManager* inputs = api->inputManager;
		float deltaTime = api->time.deltaTime();

		const float MOUSE_SENSITIVITY = 0.002f;
		const float SPEED = 1.f;
		editYaw += inputs->deltaMousePos.x * MOUSE_SENSITIVITY;
		editPitch += inputs->deltaMousePos.y * MOUSE_SENSITIVITY;
		roll = 0;

		float forwardMovement = 0.f;
		if (inputs->up)
			forwardMovement += SPEED * deltaTime;
		if (inputs->down)
			forwardMovement -= SPEED * deltaTime;

		float strafeMovement = 0.f;
		if (inputs->left)
			strafeMovement -= SPEED * deltaTime;
		if (inputs->right)
			strafeMovement += SPEED * deltaTime;

		float verticalMovement = 0.f;
		if (inputs->holdSpace)
			verticalMovement += SPEED * deltaTime;
		if (inputs->ctrl)
			verticalMovement -= SPEED * deltaTime;

		position.x += Core::Maths::sin(editYaw) * forwardMovement;
		position.z -= Core::Maths::cos(editYaw) * forwardMovement;

		position.x += Core::Maths::cos(editYaw) * strafeMovement;
		position.z += Core::Maths::sin(editYaw) * strafeMovement;

		position.y += verticalMovement;
	}
}

mat4 Camera::getViewMatrix()
{
	if (!Core::API::Application::getInstance()->editMode)
		return  mat4x4::rotateZ(roll) * mat4x4::rotateX(pitch) * mat4x4::rotateY(yaw) * mat4x4::translate(-position);
	else
		return  mat4x4::rotateZ(roll) * mat4x4::rotateX(editPitch) * mat4x4::rotateY(editYaw) * mat4x4::translate(-position);
}

mat4 Camera::getProjection()
{
	if (!isOrthographic)
		return mat4x4::perspective(fovY, aspect, near, far);
	else
		return mat4x4::ortho(fovY, aspect, near, far);
}

mat4 Camera::getViewProj()
{
	return getProjection() * getViewMatrix();
}

vec3 Camera::getPosition()
{
    return position;
}

float LowRenderer::Camera::getYaw() const
{
    return yaw;
}

bool Camera::getOrthographic()
{
	return isOrthographic;
}

bool Camera::updateIMGUI()
{
	if (ImGui::TreeNode("Camera"))
	{
		ImGui::SliderFloat("Mouse Sensitivity", &MOUSE_SENSITIVITY, 0.5f, 10.f);
		ImGui::Text("apsect %.2f", aspect);

		float fovYDegr = fovY / (3.14f / 180);
		if (ImGui::SliderFloat("fovY", &fovYDegr, 1.f, 180.f))
		{
			fovY = fovYDegr * DEG2RAD;
		}

		ImGui::SliderFloat("pitch", &pitch, 0.f, 10.f);
		ImGui::SliderFloat("yaw", &yaw, 0.f, 10.f);

		ImGui::SliderFloat("near", &near, 0.001f, 1.f);
		ImGui::SliderFloat("far", &far, 0.001f, 10000.f);
		ImGui::Checkbox("Orthographic", &isOrthographic);
		//TODO : setActiveCamera

		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void Camera::save(std::ofstream& sceneFile)
{
	sceneFile << "Camera :" << "\n"
		<< "isOrthographic " << isOrthographic << "\n"
		<< "enabled " << enabled << "\n"
		<< "pitch " << pitch << "\n"
		<< "yaw " << yaw << "\n"
		<< "roll " << roll << "\n"
		<< "aspect " << aspect << "\n"
		<< "fovY " << fovY << "\n"
		<< "near " << near << "\n"
		<< "far " << far << "\n";
}
