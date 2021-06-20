#include "Graph.h"
#include "Application.h"
#include "Camera.h"
#include "Model.h"
#include "PhysicsManager.h"
#include "Debug.h"

using namespace Core::DataStructure;
using namespace Core::API;
using namespace Core::Debug;

Graph::Graph() : resourcesManager{ ResourcesManager::getInstance() }, scene(nullptr) {}

void Graph::loadScene(const char* scenePath)
{
	scene = resourcesManager->addResource<Scene>(scenePath);
	Application::getInstance()->cameraManager->setActiveCamera();
}

void Graph::newScene(const char* scenePath)
{
	std::string scenePathstr = scenePath;
	std::ofstream sceneFile("Resources\\scenes\\" + scenePathstr);
	sceneFile.close();
	Log::getInstance()->print("#New scene " + scenePathstr, LogType::DEBUG);

	glfwSetCursorPos(Application::getInstance()->window, 0, 0);
	
	loadScene(scenePath);
}

void Graph::loadModel(const char* meshPath, const char* shaderPath)
{
	std::string meshPathStr = meshPath;
	Log::getInstance()->print("#Load model " + meshPathStr, LogType::DEBUG);

	std::vector<std::shared_ptr<Mesh>> meshs;
	loadMesh(meshPath, meshs);

	for (int i = 0; i < meshs.size(); i++)
	{
		GameObject* gameObject = new GameObject;
		Model* model = gameObject->components->addComponent<Model>();
		
		if (meshs[i]->texturePath.compare("") != 0)
		{
			model->texture = resourcesManager->addResource<Texture>("Resources//assets//" + meshs[i]->texturePath);
			Log::getInstance()->print("Added texture: " + meshs[i]->texturePath, LogType::DEBUG);
		}
		else
			model->texture = nullptr;
		
		model->material = resourcesManager->addResource<Material>(meshs[i]->mtlName);
		model->shader = resourcesManager->addResource<Shader>(shaderPath);
		model->mesh = meshs[i];
		
		scene->gameObjectManager.addGameObject(gameObject);
	}
}

void Graph::graphUpdate()
{
	if (scene == nullptr)
		return;

 	scene->sceneUpdate();
	resourcesManager->unloadUnusedResources();
}


Graph::~Graph()
{
	resourcesManager = nullptr;
	scene = nullptr;

	LightsManager* lightManager(LightsManager::getInstance());
	delete lightManager;
	Log::getInstance()->print("Light Manager deleted", LogType::DEBUG);

	Physics::PhysicsManager* physicsManager(Physics::PhysicsManager::getInstance());
	delete physicsManager;
	Log::getInstance()->print("Physics Manager deleted", LogType::DEBUG);
}