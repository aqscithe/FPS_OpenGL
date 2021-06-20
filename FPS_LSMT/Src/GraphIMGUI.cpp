#include "Graph.h"
#include "Camera.h"

using namespace Core::DataStructure;

void Graph::graphIMGUI(bool& editMode)
{
	if (!editMode)
		return;

	resourcesManager->resourcesManagerIMGUI();

	if (ImGui::Begin("Graph"))
	{
		if (editMode)
		{
			if (ImGui::TreeNode("Hierarchie"))
			{
				if (ImGui::Button("Add GameObject"))
					scene->gameObjectManager.addGameObject();

				scene->gameObjectManager.updateIMGUI();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Load Model"))
			{
				static char meshPath[150] = {};
				static char shaderPath[150] = {};

				ImGui::Text("Mesh Path : Resources\\assets\\");
				ImGui::InputText("Mesh", meshPath, IM_ARRAYSIZE(meshPath));
				ImGui::Text("shader Path : Resources\\Shaders\\ without extensions");
				ImGui::InputText("Shader", shaderPath, IM_ARRAYSIZE(shaderPath));

				if (ImGui::Button("new model"))
				{
					loadModel(meshPath, shaderPath);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Save Scene"))
				scene->saveScene();
		}
	}

	ImGui::End();
}