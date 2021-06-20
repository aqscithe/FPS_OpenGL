#include "PlayerFire.h"
#include "GameObject.h"
#include "CheckCollision3D.h"
#include "ToolBox3D.h"
#include "Sprite.h"
#include "Enemy.h"
#include "Debug.h"
#include <iostream>

using namespace Gameplay::Entity;
using namespace Core::Maths::ToolBox3D::Collision;
using namespace Core::Debug;

void PlayerFire::start()
{
	transform = getComponent<Transform>();
	transformCamera = getComponentInChildren<Transform>();
	input = InputManager::getInstance();
	physicsManager = PhysicsManager::getInstance();
	player = getComponent<Player>();
}

void PlayerFire::update()
{
	if (input->mlb_down && player->getAmmunition() > 0)
	{
		Hit hit;
		Quaternion rot = QuaternionFromEuler((transformCamera->rotation.x) * DEG2RAD, 
			transform->rotation.y * DEG2RAD, transform->rotation.z * DEG2RAD);

		vec3 dir = Normalize(ToolBox3D::Vector3RotateByQuaternion(vec3{ 0, 0, -1 }, rot));

		if (physicsManager->RaycastHit(transform->pos + transformCamera->pos + Normalize(ToolBox3D::Vector3RotateByQuaternion(vec3{ 0, 0, -1.f }, rot))
			, dir, 100, hit))
		{
			Log::getInstance()->print("Player fire hit: " + hit.gameObject->getName(), LogType::INFO);

			#pragma region Sprite Hit Impact

			if (transformObject == nullptr)
			{
				ResourcesManager* resourcesManager = ResourcesManager::getInstance();
				GameObjectManager* gameObjectManager = &resourcesManager->getResource<Scene>()->gameObjectManager;
				GameObject* object = gameObjectManager->addGameObject();
				transformObject = object->components->getComponent<Transform>();
				Gameplay::UI::Sprite* sprite = object->components->addComponent<Gameplay::UI::Sprite>();
				sprite->loadTexture("hit.png");

				transformObject->scale = { 0.2f, 0.2f, 0.2f };
			}

			
			vec3 rotation = hit.interNormal * 90;
			rotation = vec3{ -rotation.y, rotation.x, rotation.z };
			rot = QuaternionFromEuler(rotation.x * DEG2RAD, rotation.y * DEG2RAD, rotation.z * DEG2RAD);

			transformObject->pos = hit.interPt + ToolBox3D::Vector3RotateByQuaternion(vec3{ 0, 0, 0.025f }, rot);
			transformObject->rotation = rotation;

			#pragma endregion

			Enemy* enemy = hit.gameObject->components->getComponent<Enemy>();
			if (enemy != nullptr)
			{
				enemy->takeDamage(1);
			}
		}

		player->ammunitionUsed();
		Log::getInstance()->print("Player fired. Ammunition: " + player->getAmmunition(), LogType::INFO);
	}

}

bool PlayerFire::updateIMGUI()
{
	if (ImGui::TreeNode("PlayerFire"))
	{
		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void PlayerFire::save(std::ofstream& sceneFile)
{
	sceneFile << "PlayerFire" << "\n";
}