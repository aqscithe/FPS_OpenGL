#pragma once
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

using namespace Resources;
using namespace Physics;

namespace Gameplay
{
	namespace UI
	{
		class Sprite : public Component
		{
		private :
			Transform* transform = nullptr;
			std::shared_ptr<Resources::Texture> texture;
			std::shared_ptr<Resources::Shader> shader;
			LowRenderer::CameraManager* cameraManager;

			GLuint m_VAO;

			float alpha = 1.f;

			void start() override;
			void update() override;
			void fixedUpdate() override {}
			bool updateIMGUI() override;
			void save(std::ofstream& sceneFile) override;

		public :
			void loadTexture(std::string path);
		};
	}
}
