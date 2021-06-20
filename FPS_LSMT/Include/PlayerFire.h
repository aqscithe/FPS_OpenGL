#pragma once
#include "Sprite.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "Player.h"

using namespace Resources;
using namespace Core::API;

namespace Gameplay
{
	namespace Entity
	{
		class PlayerFire : public Component
		{

		private :
			Transform* transform = nullptr;
			Transform* transformCamera = nullptr;
			Transform* transformObject = nullptr;
			Player* player = nullptr;
			InputManager* input = nullptr;
			Physics::PhysicsManager* physicsManager = nullptr;

			void start() override;
			void update() override;
			void fixedUpdate() override {}
			bool updateIMGUI() override;
			void save(std::ofstream& sceneFile) override;
		};
	}
}
