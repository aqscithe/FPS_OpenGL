#pragma once

#include "Maths.h"
#include "Component.h"

using namespace Core::Maths;
using namespace Resources;


namespace Physics
{
	class Transform : public Component
	{
	public:
		Transform();
		void start() override {};
		void update() override;
		void fixedUpdate() override {}
		bool updateIMGUI() override;
		void save(std::ofstream& sceneFile) override;


		mat4 matrixModel;
		vec3 pos;
		vec3 rotation;;
		vec3 scale;

		void setParent(Transform* transform);
		Transform* transformParent;
	};
}
