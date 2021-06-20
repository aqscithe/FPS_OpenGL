#include "Lights.h"
#include "Components.h"
#include <fstream>

using namespace LowRenderer;

LightsManager* LightsManager::instance = 0;
LightsManager* LightsManager::getInstance()
{
	if (instance == 0)
	{
		instance = new LightsManager();
	}

	return instance;
}

void LightsManager::lightSendShader(std::shared_ptr<Shader>& shader, int indexLight)
{
	std::string lightsIndex = "lights[" + std::to_string(indexLight) + "].";

	shader->setInt((lightsIndex + "lightType").c_str(), lights[indexLight]->lightType);
	shader->setVec3((lightsIndex + "pos").c_str(), lights[indexLight]->pos);
	shader->setVec3((lightsIndex + "dir").c_str(), lights[indexLight]->dir);
	shader->setVec3((lightsIndex + "ambient").c_str(), lights[indexLight]->ambient);
	shader->setVec3((lightsIndex + "diffuse").c_str(), lights[indexLight]->diffuse);
	shader->setVec3((lightsIndex + "specular").c_str(), lights[indexLight]->specular);
	shader->setVec3((lightsIndex + "attenuation").c_str(), lights[indexLight]->attenuation);
	shader->setFloat((lightsIndex + "cutOff").c_str(), cosf(lights[indexLight]->cutOff * DEG2RAD));
	shader->setFloat((lightsIndex + "outerCutOff").c_str(), cosf(lights[indexLight]->outerCutOff * DEG2RAD));

}

void LightsManager::lightsUpdateOnModel(std::shared_ptr<Shader>& shader)
{
	for (int i = 0; i < lights.size(); i++)
	{
		lightSendShader(shader, i);
	}
}

void LightsManager::deleteLight(Light* light_)
{
	for (auto& light : lights)
	{
		if (light == light_)
		{
			light = lights[lights.size() - 1];
			lights.pop_back();
			return;
		}
	}
}

Light::Light()
{
	transform = nullptr;
	LightsManager::getInstance()->addLight(this);

	lightType = 0; //off/dir/point/spot
	pos = {};
	dir = {};
	ambient = {};
	diffuse = {};
	specular = {};
	attenuation = {}; //constant/linear/quadratic
	cutOff = 0;
	outerCutOff = 0;
	transform = nullptr;
}

void Light::start()
{
	transform = getComponent<Physics::Transform>();
}

void Light::update()
{
	pos = transform->pos;
	dir = transform->rotation;
}

bool Light::updateIMGUI()
{
	if (ImGui::TreeNode("Light"))
	{
			ImGui::Text("Light type : 0 = OFF, 1 = Directional, 2 = PointLight, 3 = SpotLight");
			ImGui::SliderInt("Light Type", &lightType, 0, 3);

			if (lightType != 0)
			{

				//if (lightType != 1)
				//	ImGui::SliderFloat3("Position", pos.e, -50, 50);
				//
				//if (scene->lights.lights[item_current_light].lightType != 2)
				//	ImGui::SliderFloat3("Direction", scene->lights.lights[item_current_light].dir.e, -1, 1);

				ImGui::ColorEdit3("Ambient", ambient.e);
				ImGui::ColorEdit3("diffuse", diffuse.e);
				ImGui::ColorEdit3("specular", specular.e);

				if (lightType != 1)
				{
					ImGui::Text("Attenuation : constant  /  linear  /  quadratic");
					ImGui::SliderFloat3("Attenuation", attenuation.e, 0, 1);
				}

				if (lightType == 3)
				{
					ImGui::SliderFloat("cutOff", &cutOff, 0, 180);
					ImGui::SliderFloat("outerCutOff", &outerCutOff, 0, 180);
				}
			}

			if (deleteComponentIMGUI())
			{
				ImGui::TreePop();
				return true;
			}

			ImGui::TreePop();
	}

	return false;
}

void Light::save(std::ofstream& sceneFile)
{
	sceneFile << "Light\n";
	sceneFile << "lightType " << lightType << "\n";
	sceneFile << "ambient " << ambient.x << " " << ambient.y << " " << ambient.z << "\n";
	sceneFile << "diffuse " << diffuse.x << " " << diffuse.y << " " << diffuse.z << "\n";
	sceneFile << "specular " << specular.x << " " << specular.y << " " << specular.z << "\n";
	sceneFile << "attenuation " << attenuation.x << " " << attenuation.y << " " << attenuation.z << "\n";
	sceneFile << "cutOff " << cutOff << "\n";
	sceneFile << "outerCutOff " << outerCutOff << "\n";
}

Light::~Light()
{
	LightsManager::getInstance()->deleteLight(this);
}