#include "Enemy.h"
#include "Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Debug.h"
#include "GameObject.h"

using namespace LowRenderer;
using namespace Core::Debug;


void Gameplay::Entity::Enemy::start()
{
	health = 2;
	damage = 0;
}

void Gameplay::Entity::Enemy::isDead()
{
	Core::API::Application::getInstance()->graph->scene->nbrEnemies--;
	getGameObject<GameObject>()->deleteGameObjectWithChildren();
	Log::getInstance()->print("Enemy died", LogType::INFO);
}

void Gameplay::Entity::Enemy::takeDamage(const int damage)
{
	health -= damage;
	Log::getInstance()->print("Enemy took damage: " + damage, LogType::INFO);
	Log::getInstance()->print("Enemy health: " + std::to_string(health), LogType::INFO);
}

void Gameplay::Entity::Enemy::update()
{
	damage = 1;
	if (health <= 0)
		isDead();
}

bool Gameplay::Entity::Enemy::updateIMGUI()
{
	if (ImGui::TreeNode("Enemy"))
	{
		ImGui::SliderInt("damage", &damage, 0, 5);
		ImGui::SliderInt("health", &health, 0, 5);

		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void Gameplay::Entity::Enemy::save(std::ofstream& sceneFile)
{
	sceneFile << "Enemy\n";
	sceneFile << "damage " << damage << "\n";
	sceneFile << "health " << health << "\n";
}
