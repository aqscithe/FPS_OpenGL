#include "Scene.h"
#include "Debug.h"
#include "Application.h"
#include "Mesh.h"
#include "Transform.h"
#include "Model.h"
#include "Camera.h"
#include "Lights.h"
#include "Collider3D.h"
#include "Box2D.h"
#include "Rigidbody.h"
#include "PlayerMovements.h"
#include "CameraMovements.h"
#include "FollowPlayer.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerFire.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace Resources;
using namespace Physics;
using namespace Gameplay::Entity;
using namespace Core::Debug;

Scene::Scene() : resourcesManager(ResourcesManager::getInstance())
{
}

void Scene::saveScene()
{
	std::ofstream sceneFile("Resources\\scenes\\" + resourcePath);

	Log::getInstance()->print("Save Scene " + resourcePath, LogType::INFO);
	sceneFile << std::fixed;

	sceneFile << std::fixed;
	if (gameObjectManager.gameObjects.size() != 0)
	{
		std::vector<std::string> meshsPath;
		resourcesManager->getMeshsPath(meshsPath);

		sceneFile << "Mesh \n";
		for (int i = 0; i < meshsPath.size(); i++)
			sceneFile << "meshPath " << meshsPath[i] << "\n";
	}

	gameObjectManager.saveGameObjects(sceneFile);
	sceneFile.close();
}

void Scene::loadResource()
{
	std::ifstream in("Resources\\scenes\\" + resourcePath, std::ios::in);
	if (!in)
	{
		Log::getInstance()->print("Load scene failed: Resources\\scenes\\" + resourcePath, LogType::ERROR);
		exit(1);
	}
	Log::getInstance()->print("Load scene " + resourcePath, LogType::INFO);

	std::string line;
	std::vector<int> parentIndexs;

	while (std::getline(in, line))
	{
		if (line.substr(0, 9) == "meshPath ")
		{
			std::string meshPath;
			std::istringstream mPath(line.substr(8));
			mPath >> meshPath;

			std::vector<std::shared_ptr<Mesh>> meshs;
			loadMesh(meshPath, meshs);
		}

		if (line.substr(0, 10) == "GameObject")
		{
			GameObject* gameObject = new GameObject;
			gameObjectManager.addGameObject(gameObject);
			std::getline(in, line);
			std::istringstream name(line.substr(5));
			std::string nameStr;
			name >> nameStr;
			gameObject->setName(nameStr);
			Log::getInstance()->print("Adding GameObject " + nameStr, LogType::DEBUG);

			std::getline(in, line);
			if (line.substr(0, 10) == "Components")
			{
				Log::getInstance()->print("Adding " + nameStr + " components", LogType::DEBUG);
				//std::getline(in, line);
				while (std::getline(in, line))
				{
					if (line.compare("") == 0)
						break;

					if (line.substr(0, 9) == "Transform")
					{
						Log::getInstance()->print("Component: Transform", LogType::DEBUG);
						Transform* transform = gameObject->components->getComponent<Transform>();

						std::getline(in, line);
						std::istringstream pos(line.substr(8));
						for (int i = 0; i < 3; i++)
							pos >> transform->pos.e[i];

						std::getline(in, line);
						std::istringstream rot(line.substr(9));

						for (int i = 0; i < 3; i++)
							rot >> transform->rotation.e[i];

						std::getline(in, line);
						std::istringstream scale(line.substr(6));

						for (int i = 0; i < 3; i++)
							scale >> transform->scale.e[i];

						std::getline(in, line);
						std::istringstream parent(line.substr(7));
						int index;
						parent >> index;
						parentIndexs.push_back(index);

						continue;
					}

					if (line.substr(0, 5) == "Model")
					{
						Log::getInstance()->print("Component: Model", LogType::DEBUG);
						Model* model = gameObject->components->addComponent<Model>();
						std::getline(in, line);

						//Texture
						std::istringstream tex(line.substr(4));
						std::string textureName;
						tex >> textureName;
						model->texture = resourcesManager->addResource<Texture>(textureName);

						//Material
						std::getline(in, line);
						std::istringstream mat(line.substr(4));
						std::string mtlName;
						mat >> mtlName;
						model->material = resourcesManager->addResource<Material>(mtlName);

						//Shader
						std::getline(in, line);
						std::istringstream shader(line.substr(7));
						std::string shaderName;
						shader >> shaderName;
						model->shader = resourcesManager->addResource<Shader>(shaderName);

						//Mesh
						std::getline(in, line);
						std::istringstream m(line.substr(13));
						std::string meshName;
						m >> meshName;
						model->mesh = resourcesManager->addResource<Mesh>(meshName);

						continue;
					}

					if (line.substr(0, 15) == "CameraMovements")
					{
						Log::getInstance()->print("Component: CameraMovements", LogType::DEBUG);
						CameraMovements* camMovements = gameObject->components->addComponent<CameraMovements>();

						continue;
					}

					if (line.substr(0, 6) == "Camera")
					{
						Log::getInstance()->print("Component: Camera", LogType::DEBUG);
						Camera* camera = gameObject->components->addComponent<Camera>();

						bool value;

						std::getline(in, line);
						std::istringstream ortho(line.substr(15));
						ortho >> value;
						camera->setOrthographic(value);

						std::getline(in, line);
						std::istringstream enabled(line.substr(8));
						enabled >> value;
						camera->setEnabled(value);

						float tmp;

						std::getline(in, line);
						std::istringstream pitch(line.substr(6));
						pitch >> tmp;
						camera->setPitch(tmp);

						std::getline(in, line);
						std::istringstream yaw(line.substr(4));
						yaw >> tmp;
						camera->setYaw(tmp);

						std::getline(in, line);
						std::istringstream roll(line.substr(5));
						roll >> tmp;
						camera->setRoll(tmp);

						std::getline(in, line);
						std::istringstream aspect(line.substr(7));
						aspect >> tmp;
						camera->setAspect(tmp);

						std::getline(in, line);
						std::istringstream fovY(line.substr(5));
						fovY >> tmp;
						camera->setFovY(tmp);

						std::getline(in, line);
						std::istringstream near(line.substr(5));
						near >> tmp;
						camera->setNear(tmp);

						std::getline(in, line);
						std::istringstream far(line.substr(4));
						far >> tmp;
						camera->setFar(tmp);

						continue;
					}


					if (line.substr(0, 5) == "Light")
					{
						Log::getInstance()->print("Component: Light", LogType::DEBUG);
						Light* light = gameObject->components->addComponent<Light>();

						std::getline(in, line);
						std::istringstream type(line.substr(10));
						type >> light->lightType;

						std::getline(in, line);
						std::istringstream ambient(line.substr(8));
						for (int i = 0; i < 3; i++)
							ambient >> light->ambient.e[i];

						std::getline(in, line);
						std::istringstream diffuse(line.substr(8));
						for (int i = 0; i < 3; i++)
							diffuse >> light->diffuse.e[i];

						std::getline(in, line);
						std::istringstream specular(line.substr(9));
						for (int i = 0; i < 3; i++)
							specular >> light->specular.e[i];

						std::getline(in, line);
						std::istringstream attenuation(line.substr(12));
						for (int i = 0; i < 3; i++)
							attenuation >> light->attenuation.e[i];

						std::getline(in, line);
						std::istringstream cutOff(line.substr(7));
						cutOff >> light->cutOff;

						std::getline(in, line);
						std::istringstream outerCutOff(line.substr(12));
						outerCutOff >> light->outerCutOff;

						continue;
					}

					if (line.substr(0, 11) == "BoxCollider")
					{
						Log::getInstance()->print("Component: BoxCollider", LogType::DEBUG);
						ToolBox3D::BoxCollider* boxCollider = gameObject->components->addComponent<ToolBox3D::BoxCollider>();

						std::getline(in, line);
						std::istringstream isTrigger(line.substr(10));
						isTrigger >> boxCollider->isTrigger;

						std::getline(in, line);
						std::istringstream drawCollider(line.substr(13));
						drawCollider >> boxCollider->drawCollider;

						vec3 tmp = {};
						std::getline(in, line);
						std::istringstream localPos(line.substr(9));
						for (int i = 0; i < 3; i++)
							localPos >> tmp.e[i];
						boxCollider->setLocalPos(tmp);

						std::getline(in, line);
						std::istringstream scale(line.substr(6));
						for (int i = 0; i < 3; i++)
							scale >> boxCollider->extensions.e[i];

						continue;
					}

					if (line.substr(0, 14) == "SphereCollider")
					{
						Log::getInstance()->print("Component: SphereCollider", LogType::DEBUG);
						ToolBox3D::SphereCollider* sphereCollider = gameObject->components->addComponent<ToolBox3D::SphereCollider>();

						std::getline(in, line);
						std::istringstream isTrigger(line.substr(10));
						isTrigger >> sphereCollider->isTrigger;

						std::getline(in, line);
						std::istringstream drawCollider(line.substr(13));
						drawCollider >> sphereCollider->drawCollider;

						vec3 tmp = {};
						std::getline(in, line);
						std::istringstream localPos(line.substr(9));
						for (int i = 0; i < 3; i++)
							localPos >> tmp.e[i];
						sphereCollider->setLocalPos(tmp);

						std::getline(in, line);
						std::istringstream radius(line.substr(7));
						radius >> sphereCollider->r;

						continue;
					}

					if (line.substr(0, 6) == "2D Box")
					{
						Log::getInstance()->print("Component: 2D Box", LogType::DEBUG);
						Toolbox2D::Box* box = gameObject->components->addComponent<Toolbox2D::Box>();

						std::getline(in, line);
						std::istringstream width(line.substr(6));
						width >> box->w;

						std::getline(in, line);
						std::istringstream height(line.substr(7));
						height >> box->h;

						std::getline(in, line);
						std::istringstream option(line.substr(7));
						option >> box->option;

						std::getline(in, line);
						std::istringstream tex(line.substr(8));
						std::string textureName;
						tex >> textureName;
						box->texture = resourcesManager->addResource<Texture>(textureName);

						continue;
					}

					if (line.substr(0, 9) == "Rigidbody")
					{
						Log::getInstance()->print("Component: Rigidbody", LogType::DEBUG);
						Rigidbody* rigidbody = gameObject->components->addComponent<Rigidbody>();

						std::getline(in, line);
						std::istringstream mass(line.substr(5));
						mass >> rigidbody->mass;

						std::getline(in, line);
						std::istringstream velocity(line.substr(9));
						for (int i = 0; i < 3; i++)
							velocity >> rigidbody->velocity.e[i];

						continue;
					}

					if (line.substr(0, 15) == "PlayerMovements")
					{
						Log::getInstance()->print("Component: PlayerMovements", LogType::DEBUG);
						PlayerMovements* playerMovements = gameObject->components->addComponent<PlayerMovements>();

						float tmp;
						std::getline(in, line);
						std::istringstream speed(line.substr(6));
						speed >> tmp;
						playerMovements->setSpeed(tmp);

						std::getline(in, line);
						std::istringstream jumpForce(line.substr(10));
						jumpForce >> tmp;
						playerMovements->setJumpForce(tmp);

						continue;
					}

					if (line.substr(0, 12) == "FollowPlayer")
					{
						Log::getInstance()->print("Component: FollowPlayer", LogType::DEBUG);
						FollowPlayer* followPlayer = gameObject->components->addComponent<FollowPlayer>();

						float tmp;
						std::getline(in, line);
						std::istringstream speed(line.substr(6));
						speed >> tmp;
						followPlayer->setSpeed(tmp);

						vec3 tmpvec3{};
						std::getline(in, line);
						std::istringstream targetPos(line.substr(9));
						for (int i = 0; i < 3; i++)
							targetPos >> tmpvec3.e[i];
						followPlayer->setTarget(tmpvec3);


						continue;
					}

					if (line.substr(0, 10) == "PlayerFire")
					{
						Log::getInstance()->print("Component: PlayerFire", LogType::DEBUG);
						gameObject->components->addComponent<PlayerFire>();
						continue;
					}

					if (line.substr(0, 6) == "Player")
					{
						Log::getInstance()->print("Component: Player", LogType::DEBUG);
						gameObject->components->addComponent<Player>();
						continue;
					}


					if (line.substr(0, 5) == "Enemy")
					{
						Log::getInstance()->print("Component: Enemy", LogType::DEBUG);
						Enemy* enemy = gameObject->components->addComponent<Enemy>();

						int tmpInt;
						std::getline(in, line);
						std::istringstream damage(line.substr(7));
						damage >> tmpInt;
						enemy->setDamage(tmpInt);

						std::getline(in, line);
						std::istringstream health(line.substr(7));
						health >> tmpInt;
						enemy->setHealth(tmpInt);

						nbrEnemies++;

						continue;
					}
				}
			}
			Log::getInstance()->print(nameStr + " components added", LogType::DEBUG);
		}
	}

	in.close();

	for (int i = 0; i < gameObjectManager.gameObjects.size(); i++)
	{
		if (parentIndexs[i] != -1)
		{
			Transform* transform = gameObjectManager.gameObjects[parentIndexs[i]]->components->getComponent<Transform>();
			gameObjectManager.gameObjects[i]->components->getComponent<Transform>()->setParent(transform);
		}
		else
		{
			gameObjectManager.gameObjects[i]->components->getComponent<Transform>()->setParent(nullptr);
		}
	}
}

void Scene::sceneUpdate()
{
	updateMouse();
	gameObjectManager.update();
}

void Scene::unloadResource()
{
	gameObjectManager.deleteAllGameObject();
}

void Scene::updateMouse()
{
	GLFWwindow* window = Core::API::Application::getInstance()->window;
	InputManager* inputs = InputManager::getInstance();

	updateMousePos(window, inputs);
	verifyMouseCapture(window, inputs);
}

void Resources::Scene::updateMousePos(GLFWwindow* window, InputManager* inputs)
{
	double newMouseX, newMouseY;
	glfwGetCursorPos(window, &newMouseX, &newMouseY);

	inputs->mouseDeltaX = (float)(newMouseX - mouseX);
	inputs->mouseDeltaY = (float)(newMouseY - mouseY);

	mouseX = newMouseX;
	mouseY = newMouseY;
}

void Scene::verifyMouseCapture(GLFWwindow* window, InputManager* inputs)
{
	if (glfwGetKey(window, GLFW_KEY_0))
	{
		inputs->mouseCaptured = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		Log::getInstance()->print("Mouse Released", LogType::INFO);
	}
	else if (glfwGetKey(window, GLFW_KEY_9))
	{
		inputs->mouseCaptured = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Log::getInstance()->print("Mouse Captured", LogType::INFO);
	}
}

