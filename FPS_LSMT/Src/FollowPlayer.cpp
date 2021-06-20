#include "FollowPlayer.h"
#include "Enemy.h"
#include "Application.h"
#include "PlayerMovements.h"

using namespace Gameplay::Entity;
using namespace Core::Maths::ToolBox3D;
using namespace Core::Debug;

void FollowPlayer::checkTarget()
{
	hasTarget = false;
	for (auto& trigger : playerDetector->triggers)
	{
		if (trigger->collisions.size() == 0)
			continue;
		if (trigger->getComponent<PlayerMovements>())
		{
			targetPos = trigger->getComponent<Transform>()->pos;

			hasTarget = true;
			//std::cout << "origin enemy trigger " << playerDetector->origin.x << " " << playerDetector->origin.y << " " << playerDetector->origin.z << std::endl;
			break;
		}
	}
}

void FollowPlayer::move()
{
	float deltaTime = Application::getInstance()->time.deltaTime();
	if (hasTarget)
	{
		vec3 direction = targetPos - parentTrans->pos;
		//direction.y = 0.f;

		raycast.start = parentTrans->pos;
		raycast.end = targetPos;
		raycast.dir = direction;

		vec3 normal;
		if (!physicsManager->CollisionSegmentBox(&raycast, normal))
		{
			speed = 0.3f;
			vec3 translation = direction * speed * deltaTime;
			parentTrans->pos += translation;
			transform->pos = parentTrans->pos;
		}
		else
		{
			speed = 0.f;
			//std::cout << "ENEMY RAYCAST HIT OBSTACLE" << std::endl;
			//std::cout << "NORMAL: " << normal.x << " " << normal.y << " " << normal.z << std::endl;
		}
			
	}
}

void FollowPlayer::start()
{
	playerDetector = getComponent<SphereCollider>();
	physicsManager = PhysicsManager::getInstance();
}

void FollowPlayer::initTransRigid()
{
	transform = getComponent<Transform>();
	parentTrans = transform->transformParent;
	rigidbody = transform->getComponent<Rigidbody>();
}

void FollowPlayer::update()
{
	if (transform == nullptr && rigidbody == nullptr)
		initTransRigid();
	playerDetector->origin = parentTrans->pos;
	checkTarget();
	move();
}

bool FollowPlayer::updateIMGUI()
{

	if (ImGui::TreeNode("FollowPlayer"))
	{
		ImGui::SliderFloat("speed", &speed, 0, 10);
		ImGui::DragFloat3("target", targetPos.e);

		if (deleteComponentIMGUI())
		{
			ImGui::TreePop();
			return true;
		}

		ImGui::TreePop();
	}

	return false;

}

void FollowPlayer::save(std::ofstream& sceneFile)
{
	sceneFile << "PlayerMovements\n";
	sceneFile << "speed " << speed << "\n";
	sceneFile << "target position " << targetPos.x << " " << targetPos.y << " " << targetPos.z << "\n";
	sceneFile << "speed " << speed << "\n";
	sceneFile << "target position " << targetPos.x << " " << targetPos.y << " " << targetPos.z << "\n";
}
