#pragma once
#include "Component.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"

namespace Physics
{
	class Collider : public Component
	{
	public :
		Collider()
		{
			shader = Resources::ResourcesManager::getInstance()->addResource<Shader>("shaderCollider");
			cameraManager = LowRenderer::CameraManager::getInstance();

			drawCollider = true;
		}

		bool drawCollider = false;
		bool isTrigger = false;

		std::vector<Collider*> collisions;
		std::vector<Collider*> triggers;

		virtual void TransformColliderUpdate() = 0;
		virtual void DrawCollider() = 0;
		void setLocalPos(vec3 pos) { localPosition = pos; }

	protected:
		Transform* transform;
		vec3 localPosition = {};
		std::shared_ptr<Shader> shader;
		LowRenderer::CameraManager* cameraManager;
	};
}