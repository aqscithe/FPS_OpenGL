#include "Player.h"
#include "Application.h"
#include "Enemy.h"
#include "Debug.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Debug.h"

using namespace Core::Debug;

void Gameplay::Entity::Player::start()
{
	collider = getComponent<SphereCollider>();
	shader = ResourcesManager::getInstance()->addResource<Shader>("shaderFinalScreen");
}

void Gameplay::Entity::Player::drawScreen(std::shared_ptr<Texture> texture)
{
	vec3 color = { 1.f, 1.f, 1.f };

	float vertices[] = {
	-1.f, -1.f, 0.f, 0.f, 0.f,
	-1.f, 1.f, 0.f, 0.f, 1.f,
	 1.f, 1.f, 0.f, 1.f, 1.f,
	 1.f, -1.f, 0.f, 1.f, 0.f,
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3,
	};

	unsigned int EBO, VBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	if (VAO == 0)
		glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Core::Debug::Assertion::assertShrdPtr(shader);
	shader->use();

	//Color
	shader->setVec3("myColor", color);

	if (texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, texture->data);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
}

void Gameplay::Entity::Player::update()
{
	checkEnemyContact();

	if (health > maxHealth)
		health = maxHealth;
	if (health <= 0)
	{
		health = 0;
		gameOverDuration -= Application::getInstance()->time.deltaTime();

		if (gameOverTexture == nullptr)
			gameOverTexture = ResourcesManager::getInstance()->addResource<Texture>("Resources//assets//Game_Over_Screen.png");

		drawScreen(gameOverTexture);
	}
	if (Application::getInstance()->graph->scene->nbrEnemies <= 0)
	{
		gameOverDuration -= Application::getInstance()->time.deltaTime();
	
		if (winScreenTexture == nullptr)
			winScreenTexture = ResourcesManager::getInstance()->addResource<Texture>("Resources//assets//Win_Screen.png");
	
		drawScreen(winScreenTexture);
	}

	if (gameOverDuration <= 0)
	{
		Application::getInstance()->graph->loadScene("mainMenu.scn");
		return;
	}

	if (InputManager::getInstance()->rKey)
		setAmmunition(maxAmmo + (int)(ammunition > 0));

	pui.drawGUI(health / (float)maxHealth, ammunition, maxAmmo);
}

void Gameplay::Entity::Player::checkEnemyContact()
{
	float deltaTime = Application::getInstance()->time.deltaTime();
	
	for (auto& collision : collider->collisions)
	{
		if (collision->getComponent<Enemy>())
		{
			auto enemy = collision->getComponent<Enemy>();
			enemy->addTimeInContact(deltaTime);;
			if (enemy->getTimeInContact() >= 1.f)
			{
				health -= enemy->getDamage();
				Log::getInstance()->print(" Player took damage: " + enemy->getDamage(), LogType::INFO);
				Log::getInstance()->print("Player health: " + std::to_string(health), LogType::INFO);
				enemy->resetTimeInContact();
			}
		}
	}
}

bool Gameplay::Entity::Player::updateIMGUI()
{
	if (ImGui::TreeNode("Player"))
	{
		ImGui::DragInt("max health", &maxHealth, 1.f, 0);
		ImGui::SliderInt("health", &health, 0, maxHealth);
		ImGui::DragInt("max ammunitions", &maxAmmo, 1.f, 0);
		ImGui::SliderInt("ammunitions", &ammunition, 0, maxAmmo);

		pui.showIMGUI();

		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void Gameplay::Entity::Player::save(std::ofstream& sceneFile)
{
	sceneFile << "Player\n";
}

void Gameplay::Entity::Player::takeDamage()
{
}

void Gameplay::Entity::Player::heal()
{
}

void Gameplay::Entity::Player::respawn()
{
}

void Gameplay::Entity::Player::setDefault()
{
}
