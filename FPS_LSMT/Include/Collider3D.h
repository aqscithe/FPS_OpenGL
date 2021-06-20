#pragma once

#include "Maths.h"
#include "Collider.h"
#include <memory>

namespace Core
{
	namespace Maths
	{
		namespace ToolBox3D
		{
			class Segment
			{
			public:
				vec3 start, end;
				vec3 dir;

				Segment() = default;
				Segment(const vec3& s, const vec3& e);
			};

			class PlaneCollider
			{
			public:
				PlaneCollider(const vec3& Normal, const float& Distance);
				PlaneCollider(const vec3& Normal, const vec3& point);
				PlaneCollider(const vec3& a, const vec3& b, const vec3& c);

				vec3 Normal;
				float d;
			};

			class SphereCollider : public Physics::Collider 
			{
			public:
				SphereCollider() {};
				SphereCollider(const vec3& point, const Quaternion& q1, const float& radius, const float& mass);
				SphereCollider(const vec3& point, const Quaternion& q1, const float& radius);
				SphereCollider(const vec3& point, const float& radius) { origin = point; r = radius; };
				~SphereCollider();
				vec3 origin = { 0 };
				float r = 1.f;
				Quaternion q = {};
				vec3 velocity = { 0.f, 0.f, 0.f };
				float m = 0.f;

				GLuint m_VAO;

				void start() override;
				void update() override;
				void fixedUpdate() override {}
				bool updateIMGUI() override;
				void TransformColliderUpdate() override;

				void DrawCollider() override;
				void save(std::ofstream& sceneFile) override;
			};

			class CylinderCollider
			{
			public:
				CylinderCollider() { infinite = false; };
				CylinderCollider(const vec3& a, const Quaternion& q1, const float& radius, const float& height, const int finite);

				vec3 pt1 = { 0 };
				vec3 pt2 = { 0 };
				float r = 0;
				float h = 0;
				bool infinite;
				Quaternion q = {};
			};

			class QuadCollider
			{
			public:
				QuadCollider(const vec3& position, const Quaternion& q1, const vec2& _extensions);

				vec3 pos;
				Quaternion q;
				vec2 extensions;
			};

			class Quad
			{
			public:
				vec3 center;
				vec2 extensions; //width, height
				Quaternion q;
				bool reverse = false;

				Quad() = default;
				Quad(const vec3& center, const vec2& extensions, const Quaternion& q, const bool reverse);

				vec3 getNormal() const;
			};

			class BoxCollider : public Physics::Collider
			{
			public:
				BoxCollider() {};
				BoxCollider(const vec3& position, const Quaternion& q1, const vec3& extensions, const float& r);
				BoxCollider(const vec3& position, const Quaternion& q1, const vec3& extensions);
				~BoxCollider();
				vec3 unitX = {1.f, 0, 0};
				vec3 unitY = {0, 1.f, 0};
				vec3 unitZ = { 0, 0, 1.f };

				vec3 center = {};
				Quaternion q = {};

				vec3 extensions = {1.f, 1.f, 1.f};
				vec3 localExtensions = { 1.f,1.f,1.f };
				float radius = 0;

				float friction = 0.2f;

				GLuint m_VAO;

				void start() override;
				void update() override;
				void fixedUpdate() override {}
				bool updateIMGUI() override;
				void TransformColliderUpdate() override;

				void DrawCollider() override;
				void save(std::ofstream& sceneFile) override;
			};

			class DiskCollider
			{
			public:
				DiskCollider(const vec3& Normal, const vec3& point, const float& radius) { this->Normal = Normal, origin = point; r = radius; };

				vec3 Normal;
				vec3 origin;
				float r;
			};

			class CapsuleCollider
			{
			public:
				CapsuleCollider(const vec3& a, const Quaternion& q1, const float& radius, const float& height);

				vec3 pt1 = { 0 };
				vec3 pt2 = { 0 };
				float r;
				float h;
				Quaternion q;

				void getCollider(SphereCollider& s1, SphereCollider& s2, CylinderCollider& c1);
			};
		}
	}
}