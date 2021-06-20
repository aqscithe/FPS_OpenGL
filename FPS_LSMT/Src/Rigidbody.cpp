#include "Rigidbody.h"
#include "Application.h"
#include "ToolBox3D.h"
#include "Debug.h"
#include "GameObject.h"
#include <imgui.h>

#include "Times.h"

using namespace Physics;
using namespace Core::Debug;
using namespace LowRenderer;

void Rigidbody::start()
{
	transform = getComponent<Transform>();
	physicsManager = PhysicsManager::getInstance();
	sphereCollider = getComponent<SphereCollider>();
}

void Rigidbody::fixedUpdate()
{
	float fdt = Application::getInstance()->time.fixedDeltaTime();

	vec3 position = transform->pos;
	vec3 acceleration = physicsManager->gravity;

	//float energy = 0.5f * mass + mass * acceleration.y * position.y;
	//float speed = sqrtf(2 * fabsf(energy - mass * acceleration.y * position.y) / mass);

	velocity += acceleration * fdt;

	vec3 nextPos = position + velocity * fdt;
	// segment from actual pos to next pos
	Segment tn(position, nextPos);

	// store every normals to know the reactions direction
	std::vector<vec3> normals;
	// sotre every friction values
	std::vector<float> frictions;

	inAir = true;

	std::vector<BoxCollider*> boxes;
	std::vector<BoxCollider*> nonBoxCollisions;

	std::vector<SphereCollider*> spheres;
	std::vector<SphereCollider*> nonCollisions;

	if (sphereCollider)
	{
		if (physicsManager->CollisionBoxesSphere(*sphereCollider, normals, tn, boxes, nonBoxCollisions, frictions))
		{
			if (!sphereCollider->isTrigger)
			{
				// check every normals and compute every support reactions
				for (auto& normal : normals)
				{
					normal = Normalize(normal);
					velocity -= Dot(velocity, normal) * normal;
					inAir = false;
				}
			}

			enterCollisions(boxes);
			enterTriggers(boxes);
		}

		if (physicsManager->CollisionSphereSphere(sphereCollider, spheres, nonCollisions))
		{
			enterCollisions(spheres);
			enterTriggers(spheres);
		}
		
		exitCollisionsTriggers(nonBoxCollisions, boxes);
		exitCollisionsTriggers(nonCollisions, spheres);
	}

	applyFriction(frictions);

	position += velocity * fdt;
	transform->pos = position;
}

void Physics::Rigidbody::exitCollisionsTriggers(std::vector<Core::Maths::ToolBox3D::SphereCollider*>& nonCollisions, std::vector<SphereCollider*>& spheres)
{
	for (int i = 0; i < nonCollisions.size(); ++i)
	{
		if (nonCollisions[i]->isTrigger)
		{
			auto itA = std::find(sphereCollider->triggers.begin(), sphereCollider->triggers.end(), nonCollisions[i]);
			if (itA != sphereCollider->triggers.end())
			{
				//Log::getInstance()->print("EXIT TRIGGER SPHERE-SHPERE", LogType::DEBUG);
				sphereCollider->triggers.erase(itA);
			}


			auto itB = std::find(nonCollisions[i]->triggers.begin(), nonCollisions[i]->triggers.end(), sphereCollider);

			if (itB != nonCollisions[i]->triggers.end())
			{
				//Log::getInstance()->print("EXIT TRIGGER SPHERE-SHPERE", LogType::DEBUG);
				nonCollisions[i]->triggers.erase(itB);
			}
		}
		else
		{
			auto itA = std::find(sphereCollider->collisions.begin(), sphereCollider->collisions.end(), nonCollisions[i]);
			if (itA != sphereCollider->collisions.end())
			{
				//Log::getInstance()->print("EXIT COLLISION SPHERE-SHPERE", LogType::DEBUG);
				sphereCollider->collisions.erase(itA);
			}

			for (auto& sphere : spheres)
			{
				auto itB = std::find(sphere->collisions.begin(), sphere->collisions.end(), nonCollisions[i]);

				if (itB != sphere->collisions.end())
				{
					//Log::getInstance()->print("EXIT COLLISION SPHERE-SHPERE", LogType::DEBUG);
					sphere->collisions.erase(itB);
				}
			}
		}
	}
}


void Physics::Rigidbody::enterTriggers(std::vector<SphereCollider*>& spheres)
{
	for (auto& sphere : spheres)
	{
		auto itTriggersA = std::find(sphereCollider->triggers.begin(), sphereCollider->triggers.end(), sphere);
		bool alreadyTriggeredA = itTriggersA != sphereCollider->triggers.end();

		auto itTriggersB = std::find(sphere->triggers.begin(), sphere->triggers.end(), sphereCollider);
		bool alreadyTriggeredB = itTriggersB != sphere->triggers.end();

		if (sphereCollider->isTrigger)
		{
			if (!alreadyTriggeredA)
			{
				//Log::getInstance()->print("ENTER TRIGGER SPHERE-SHPERE", LogType::DEBUG);
				sphereCollider->triggers.push_back(sphere);
			}
		}
		if (sphere->isTrigger)
		{
			if (!alreadyTriggeredB)
			{
				//Log::getInstance()->print("ENTER TRIGGER SPHERE-SHPERE", LogType::DEBUG);
				sphere->triggers.push_back(sphereCollider);
			}
		}
	}
}

void Physics::Rigidbody::enterCollisions(std::vector<SphereCollider*>& spheres)
{
	for (auto& sphere : spheres)
	{
		auto itCollisionsA = std::find(sphereCollider->collisions.begin(), sphereCollider->collisions.end(), sphere);
		bool alreadyCollidingA = itCollisionsA != sphereCollider->collisions.end();

		auto itCollisionsB = std::find(sphere->collisions.begin(), sphere->collisions.end(), sphereCollider);
		bool alreadyCollidingB = itCollisionsB != sphere->collisions.end();

		if (!sphereCollider->isTrigger && !sphere->isTrigger)
		{
			if (!alreadyCollidingA)
			{
				//Log::getInstance()->print("ENTER COLLISION SPHERE-SHPERE", LogType::DEBUG);
				sphereCollider->collisions.push_back(sphere);
			}
			if (!alreadyCollidingB)
			{
				//Log::getInstance()->print("ENTER COLLISION SPHERE-SHPERE", LogType::DEBUG);
				sphere->collisions.push_back(sphereCollider);
			}
		}
	}
}

void Rigidbody::exitCollisionsTriggers(std::vector<Core::Maths::ToolBox3D::BoxCollider*>& nonCollisions, std::vector<BoxCollider*>& boxes)
{
	for (int i = 0; i < nonCollisions.size(); ++i)
	{
		if (nonCollisions[i]->isTrigger)
		{
			auto itA = std::find(sphereCollider->triggers.begin(), sphereCollider->triggers.end(), nonCollisions[i]);
			if (itA != sphereCollider->triggers.end())
			{
				//Log::getInstance()->print("EXIT TRIGGER BOX-SHPERE", LogType::DEBUG);
				sphereCollider->triggers.erase(itA);
			}


			auto itB = std::find(nonCollisions[i]->triggers.begin(), nonCollisions[i]->triggers.end(), sphereCollider);

			if (itB != nonCollisions[i]->triggers.end())
			{
				//Log::getInstance()->print("EXIT TRIGGER BOX-SHPERE", LogType::DEBUG);
				nonCollisions[i]->triggers.erase(itB);
			}

		}
		else
		{
			auto itA = std::find(sphereCollider->collisions.begin(), sphereCollider->collisions.end(), nonCollisions[i]);
			if (itA != sphereCollider->collisions.end())
			{
				//Log::getInstance()->print("EXIT COLLISION BOX-SHPERE", LogType::DEBUG);
				sphereCollider->collisions.erase(itA);
			}

			for (auto& box : boxes)
			{
				auto itB = std::find(box->collisions.begin(), box->collisions.end(), nonCollisions[i]);

				if (itB != box->collisions.end())
				{
					//Log::getInstance()->print("EXIT COLLISION BOX-SHPERE", LogType::DEBUG);
					box->collisions.erase(itB);
				}
			}
		}
	}
}

void Rigidbody::enterTriggers(std::vector<BoxCollider*>& boxes)
{
	for (auto& box : boxes)
	{
		auto itTriggersA = std::find(sphereCollider->triggers.begin(), sphereCollider->triggers.end(), box);
		bool alreadyTriggeredA = itTriggersA != sphereCollider->triggers.end();

		auto itTriggersB = std::find(box->triggers.begin(), box->triggers.end(), sphereCollider);
		bool alreadyTriggeredB = itTriggersB != box->triggers.end();

		if (sphereCollider->isTrigger)
		{
			if (!alreadyTriggeredA)
			{
				//Log::getInstance()->print("ENTER TRIGGER BOX-SHPERE", LogType::DEBUG);
				sphereCollider->triggers.push_back(box);
			}
		}
		if (box->isTrigger)
		{
			if (!alreadyTriggeredB)
			{
				//Log::getInstance()->print("ENTER TRIGGER BOX-SHPERE", LogType::DEBUG);
				box->triggers.push_back(sphereCollider);
			}
		}
	}
}

void Rigidbody::enterCollisions(std::vector<BoxCollider*>& boxes)
{
	for (auto& box : boxes)
	{
		auto itCollisionsA = std::find(sphereCollider->collisions.begin(), sphereCollider->collisions.end(), box);
		bool alreadyCollidingA = itCollisionsA != sphereCollider->collisions.end();

		auto itCollisionsB = std::find(box->collisions.begin(), box->collisions.end(), sphereCollider);
		bool alreadyCollidingB = itCollisionsB != box->collisions.end();

		if (!sphereCollider->isTrigger && !box->isTrigger)
		{
			if (!alreadyCollidingA)
			{
				//Log::getInstance()->print("ENTER COLLISION BOX-SHPERE", LogType::DEBUG);
				sphereCollider->collisions.push_back(box);
			}
			if (!alreadyCollidingB)
			{
				//Log::getInstance()->print("ENTER COLLISION BOX-SHPERE", LogType::DEBUG);
				box->collisions.push_back(sphereCollider);
			}
		}
	}
}

void Rigidbody::addForce(const vec3& force, ForceMode forceMode)
{
	switch (forceMode)
	{
		case ForceMode::Force:
		{
			velocity += force / mass;
			break;
		}
		case ForceMode::Impulse:
		{
			velocity = force / mass;
			break;
		}
		case ForceMode::Acceleration:
		{
			velocity += force;
			break;
		}
		case ForceMode::VelocityChange:
		{
			velocity = force;
			break;
		}
		case ForceMode::Assign:
		{
			velocity.x = force.x == 0.f ? velocity.x : force.x;
			velocity.y = force.y == 0.f ? velocity.y : force.y;
			velocity.z = force.z == 0.f ? velocity.z : force.z;
			break;
		}
		default:
			break;
	}
}

void Physics::Rigidbody::applyFriction(std::vector<float>& frictions)
{
	if (frictions.size() == 0)
	{
		velocity -= airFriction * Normalize(velocity);
	}

	for (auto& friction : frictions)
	{
		velocity -= friction * velocity;
	}
}

bool Rigidbody::updateIMGUI()
{
	if (ImGui::TreeNode("Rigidbody"))
	{
		ImGui::SliderFloat("mass", &mass, 0.1f, 10000.f);
		ImGui::DragFloat("air friction", &airFriction);
		ImGui::Text("velocity %.2f %.2f %.2f", velocity.x, velocity.y, velocity.z);
		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;
}

void Rigidbody::save(std::ofstream& sceneFile)
{
	sceneFile << "Rigidbody\n";
	sceneFile << "mass " << mass << "\n";
	sceneFile << "velocity " << velocity.x << " " << velocity.y << " " << velocity.z << "\n";
}
