#pragma once
#include "Component.h"
#include "Transform.h"
#include "PhysicsManager.h"
#include "Collider3D.h"

namespace Physics
{
	enum class ForceMode
	{
		Force,			//add a force depending on mass
		Impulse,		//reassign velocity depending on mass
		Acceleration,	//add a force ignoring mass
		VelocityChange,	//reassign velocity ignoring mass
		Assign,			//assign new velocity if new velocity is not 0, ignores mass
	};

	class Rigidbody : public Component
	{
	public:
		Rigidbody() {};
		~Rigidbody() {};

		float mass = 1.f;
		float airFriction = 0.01f;

		vec3 velocity = {};
		bool inAir = true;

		void start() override;
		void update() override {}
		void fixedUpdate() override;
		bool updateIMGUI() override;
		void save(std::ofstream& sceneFile) override;

		void exitCollisionsTriggers(std::vector<Core::Maths::ToolBox3D::SphereCollider*>& nonCollisions, std::vector<SphereCollider*>& spheres);
		void enterTriggers(std::vector<SphereCollider*>& spheres);
		void enterCollisions(std::vector<SphereCollider*>& spheres);

		void exitCollisionsTriggers(std::vector<Core::Maths::ToolBox3D::BoxCollider*>& nonCollisions, std::vector<BoxCollider*>& boxes);
		void enterTriggers(std::vector<BoxCollider*>& boxes);
		void enterCollisions(std::vector<BoxCollider*>& boxes);

		void addForce(const vec3& force, ForceMode forceMode);
		void applyFriction(std::vector<float>& frictions);

	private : 
		Transform* transform;
		PhysicsManager* physicsManager;
		SphereCollider* sphereCollider;
	};
}