#pragma once

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Camera.h"
#include "Lights.h"

using namespace Resources;
using namespace Physics;
using namespace LowRenderer;

namespace LowRenderer
{
	class Model : public Component
	{
	public:
		Model();
		~Model();

		void start() override;
		void update() override;
		void fixedUpdate() override {}
		bool updateIMGUI() override;
		void save(std::ofstream& sceneFile) override;

		ResourcesManager* resourceManager = ResourcesManager::getInstance();

		std::shared_ptr<Texture> texture;
		std::shared_ptr<Shader> shader;
		std::shared_ptr <Material> material;
		std::shared_ptr <Mesh> mesh;

		CameraManager* cameraManager;
		LightsManager* lightsManager;
		Physics::Transform* transform;
	};
}
