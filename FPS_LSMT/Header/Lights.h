#pragma once
#include "Maths.h"
#include "Component.h"
#include "Transform.h"
#include "Shader.h"

using namespace Resources;
using namespace Core::Maths;
using namespace Physics;

namespace LowRenderer
{
	class Light : public Component
	{
	public:
		Light();
		~Light();

		void start() override;
		void update() override;
		void fixedUpdate() override {}
		bool updateIMGUI() override;
		void save(std::ofstream& sceneFile) override;

		int lightType; //off/dir/point/spot
		vec3 pos;
		vec3 dir;
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		vec3 attenuation; //constant/linear/quadratic
		float cutOff;
		float outerCutOff;
	private:
		Physics::Transform* transform;
	};

	class LightsManager
	{
	public :
		static LightsManager* getInstance();
		void addLight(Light* light) { lights.push_back(light); }
		void lightsUpdateOnModel(std::shared_ptr<Shader>& shader);
		void deleteLight(Light* light);

	private:
		std::vector<Light*> lights;
		static LightsManager* instance;
		void lightSendShader(std::shared_ptr<Shader>& shader, int indexLight);
	};
}