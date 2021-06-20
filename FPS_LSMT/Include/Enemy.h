#pragma once

#include "Component.h"

using namespace Resources;

namespace Gameplay
{
	namespace Entity
	{
		class Enemy : public Component
		{
		public:
			
			inline float getTimeInContact() { return timeInContact; }
			inline void resetTimeInContact() { timeInContact = 0.f; }
			inline void addTimeInContact(float time) { timeInContact += time; }

			inline int getDamage() { return damage; }
			inline void setDamage(int damage_) { damage = damage_; }
			
			void takeDamage(const int damage);

			inline int getHealth() { return health; }
			inline void setHealth(int health_) { health = health_; }
		private:

			int damage;
			float timeInContact = 0.f;
			int health;

			void isDead();


			void start() override;
			void update() override;
			void fixedUpdate() override {}
			bool updateIMGUI() override;
			void save(std::ofstream& sceneFile) override;
		};
	}
}
