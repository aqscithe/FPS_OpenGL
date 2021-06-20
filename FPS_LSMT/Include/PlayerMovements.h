#pragma once

#include "Component.h"
#include "Rigidbody.h"
#include "InputManager.h"
#include "camera.h"

using namespace Resources;
using namespace Core::API;
using namespace LowRenderer;

namespace Gameplay
{
	namespace Entity
	{
		enum MovementState
		{
			Idle = 0,
			Run = 1,
			Jump = 2
		};

		class PlayerMovements : public Component
		{
		public :
			void setSpeed(float speed_) { speed = speed_; }
			void setJumpForce(float jumpForce_) { jumpForce = jumpForce_; }

		private :
			//MovementState movementState = MovementState::Idle;
			float speed;
			float airSpeed = 0.015f;
			float jumpForce;

			Rigidbody* rigidbody;
			Transform* transform;
			InputManager* input;
			Camera* cam;

			void start() override;
			void update() override;
			void fixedUpdate() override {}
			bool updateIMGUI() override;
			void save(std::ofstream& sceneFile) override;
		};
	}
}
