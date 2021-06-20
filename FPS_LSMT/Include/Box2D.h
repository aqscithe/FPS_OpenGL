#pragma once
#include "Maths.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

namespace Core
{
	namespace Maths
	{
		namespace Toolbox2D
		{
			class Box : public Resources::Component
			{
			public :
				Box();

				float h = 1.f;
				float w = 1.f;
				vec2 center;
				vec3 color;
				int option = 0;

				GLuint m_VAO;

				void start() override;
				void update() override;
				void fixedUpdate() override {}
				bool updateIMGUI() override;
				void save(std::ofstream& sceneFile) override;

				void draw();
				void loadTexture(std::string path);
				std::shared_ptr<Resources::Texture> texture;

			private :
				LowRenderer::CameraManager* cameraManager;
				std::shared_ptr<Resources::Shader> shader;
				Physics::Transform* transform;
			};
		}
	}
}