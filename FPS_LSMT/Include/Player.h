#pragma once

#include "Component.h"
#include "PlayerUI.h"
#include "Sprite.h"
#include "Collider3D.h"

using namespace Resources;
using namespace Gameplay::UI;
using namespace Core::Maths::ToolBox3D;

namespace Gameplay
{
	namespace Entity
	{
		class Player : public Component
		{
		private:
			int			maxHealth = 5;
			int			health = 5;
			float						gameOverDuration = 5.f;
			PlayerUI					pui;
			std::shared_ptr<Shader>		shader;
			std::shared_ptr<Texture>	gameOverTexture;
			std::shared_ptr<Texture>	winScreenTexture;
			unsigned int				VAO = 0;
			int			maxAmmo = 10;
			int			ammunition = 10;
			SphereCollider* collider = nullptr;

			void start() override;
			void update() override;
			void checkEnemyContact();
			void fixedUpdate() override {}
			bool updateIMGUI() override;
			void save(std::ofstream& sceneFile) override;

		public:

			void takeDamage();
			void heal();

			void ammunitionUsed() { ammunition -= 1; }
			int getAmmunition() { return ammunition; }
			void setAmmunition(int value) { ammunition = value; }

			void respawn();
			void setDefault();
			void drawScreen(std::shared_ptr<Texture> texture);
		};
	}
}
