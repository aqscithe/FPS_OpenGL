#include "ResourcesManager.h"
#include "Debug.h"
#include "Mesh.h"

using namespace Resources;
using namespace Core::Debug;

ResourcesManager* ResourcesManager::instance = 0;

ResourcesManager* ResourcesManager::getInstance()
{
	if (instance == 0)
	{
		instance = new ResourcesManager();
	}

	return instance;
}

void ResourcesManager::deleteResource(const unsigned id)
{
	Core::Debug::Log::getInstance()->print("Delete Resource " + resources[id]->getResourcePath(), LogType::DEBUG);
	resources[id]->unloadResource();
	resources[id].~shared_ptr();
	resources[id] = resources[resources.size() - 1];
	resources.pop_back();
	resourceIdUpdate();
}

void ResourcesManager::getMeshsPath(std::vector<std::string>& meshsPath)
{
	for (auto& resource : resources)
	{
		if (std::shared_ptr<Mesh> mesh = getResource<Mesh>(resource->getResourceId()))
		{
			bool isExist = false;
			std::string meshPath = mesh->getResourcePath();
			meshPath.resize(meshPath.size() - mesh->meshName.size() - 1);

			for (int i = 0; i < meshsPath.size(); i++)
			{
				if (meshsPath[i].compare(meshPath.c_str()) == 0)
					isExist = true;
			}

			if (!isExist)
				meshsPath.push_back(meshPath);
		}
	}
}

void ResourcesManager::unloadUnusedResources()
{
	for (auto& resource : resources)
	{
		if (resource.use_count() == 1)
		{
			deleteResource(resource->getResourceId());
			resourceIdUpdate();
		}
	}
}

void ResourcesManager::resourceIdUpdate()
{
	for (int i = 0; i < resources.size(); i++)
	{
		resources[i]->setResourceId(i);
	}
}

void ResourcesManager::resourcesManagerIMGUI()
{
	if (ImGui::Begin("Resources Manager Debug"))
	{
		if (ImGui::TreeNode("All Resources path"))
		{
			ImGui::Text("size of resources manager = %i", resources.size());
			for (int i = 0; i < resources.size(); i++)
			{
				ImGui::Text("%i %s : use = %i", resources[i]->getResourceId(), resources[i]->getResourcePath().c_str(), resources[i].use_count());
			}
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

ResourcesManager::~ResourcesManager()
{
	for (auto& resource : resources)
	{
		resource->unloadResource();
	}

	for (auto& resource : resources)
	{
		resource.~shared_ptr();
		resource = nullptr;
	}

	Core::Debug::Log::getInstance()->print("All Resources Deleted", LogType::INFO);
}
