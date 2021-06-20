#pragma once

#include "Debug.h"

#include <string>
#include <vector>
#include <memory>
#include <imgui.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Resources
{
	class Resource
	{
	public :
		Resource() : resourceId(-1) {};
		virtual ~Resource() {};

		virtual void loadResource() = 0;
		virtual void unloadResource() = 0;

		void setResourceId(unsigned id) { resourceId = id; };
		unsigned getResourceId() { return resourceId; };

		void setResourcePath(std::string path) { resourcePath = path; };
		std::string getResourcePath() { return resourcePath; };


	protected:
		unsigned resourceId;
		std::string resourcePath;
	};


	class ResourcesManager //singleton
	{
	public :
		~ResourcesManager();
		static ResourcesManager* getInstance();


		template<class T = Resource>
		std::shared_ptr<T> addResource(std::string filePath)
		{
			for (int i = 0; i < resources.size(); i++)
			{
				if (filePath == resources[i]->getResourcePath())
					return std::dynamic_pointer_cast<T>(resources[i]);
			}

			Core::Debug::Log::getInstance()->print("Resource added " + filePath, Core::Debug::LogType::INFO);
			Resource* resource = new T;
			resource->setResourcePath(filePath);
			resource->loadResource();
			resources.push_back(std::shared_ptr<Resource>(resource));
			resourceIdUpdate();
			return std::dynamic_pointer_cast<T>(resources[resource->getResourceId()]);
		}

		void resourceIdUpdate();
		
		void deleteResource(const unsigned id);

		template <class T = Resource>
		std::shared_ptr<T> getResource(const unsigned int id)
		{
			return std::dynamic_pointer_cast<T>(resources[id]);
		};

		template <class T = Resource>
		std::shared_ptr<T> getResource()
		{
			for (int i = 0; i < resources.size(); i++)
			{
				if (std::shared_ptr<T> ptr = std::dynamic_pointer_cast<T>(resources[i]))
					return std::dynamic_pointer_cast<T>(resources[i]);
			}

			return nullptr;
		};

		void getMeshsPath(std::vector<std::string>& meshsPath);

		void unloadUnusedResources();
		void resourcesManagerIMGUI();

	private :

		std::vector<std::shared_ptr<Resource>> resources;

		static ResourcesManager* instance;
	};
}
