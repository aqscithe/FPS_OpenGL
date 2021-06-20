#pragma once

#include "Component.h"
#include "Rigidbody.h"
#include "InputManager.h"
#include "Camera.h"
#include "CameraMovements.h"
#include "Application.h"

using namespace Resources;
using namespace LowRenderer;

namespace Gameplay
{
	namespace Entity
	{

		class CameraMovements : public Component
		{
		public :

		private :

			Transform* transform = nullptr;
			Transform* transformCamera = nullptr;
			InputManager* inputs = nullptr;
			Time* time = nullptr;

			void start() override;
			void update() override;
			void fixedUpdate() override {}
			bool updateIMGUI() override;
			void save(std::ofstream& sceneFile) override;
		};
	}
}
