#pragma once

#include "Maths.h"
#include "Component.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Collider3D.h"
#include "PhysicsManager.h"


namespace Gameplay
{
	namespace Entity
	{
		class FollowPlayer : public Component
		{
		public:
			void setSpeed(float speed_) { speed = speed_; }
			void setTarget(const vec3& targetPos_) { targetPos = targetPos_; }

		private:
			
			float speed = 0.f;
			vec3 targetPos{};
			bool hasTarget = false;

			Segment raycast;
			PhysicsManager* physicsManager = nullptr;

			SphereCollider* playerDetector = nullptr;
			Rigidbody* rigidbody = nullptr;
			Transform* parentTrans = nullptr;
			Transform* transform = nullptr;

			void initTransRigid();
			void move();
			void checkTarget();

			void start() override;
			void update() override;
			void fixedUpdate() override {}
			bool updateIMGUI() override;
			void save(std::ofstream& sceneFile) override;

		};
	}
}
