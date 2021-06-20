#pragma once

#include "maths.h"
#include "Shader.h"

using namespace Core::Maths;
using namespace Resources;

namespace Gameplay
{
	namespace UI
	{
		class PlayerUI
		{
		private:
			std::shared_ptr<Shader> uiShader;
			std::shared_ptr<Shader> fontShader;

			//crosshair
			vec2	csPos = { 0.f, 0.f };
			vec3	csColor = { 1.f, 1.f, 1.f };
			float	csSize = 0.002f;
			float	csHeight = 0.4f;
			float	csThickness = 0.04f;
			float	csShift = 0.04f;

			//health bar
			vec2	hbPos = { -0.9f, -0.84f };
			vec3	hbColor = { 0.5f, 0.f, 0.f };
			vec3	hbLerpColor = { 0.8f, 0.8f, 0.8f };
			vec3	hbBackColor = { 0.2f, 0.2f, 0.2f };
			float	hbHeight = 0.05f;
			float	hbWidth = 0.5f;
			float	edgeH = 1.2f;
			float	edgeW = 1.05f;
			float	lerpedRatio = 1.f;
			float	lerpSpeed = 0.1f;

			//ammunition count
			vec3	fontColor = { 1.f, 1.f, 1.f };
			vec2	ammPos = { -0.94f, -0.72f };
			float	fontSize = 0.005f;

		public:
			PlayerUI();

			void drawGUI(float healthRatio, int ammo, int maxAmmo);

			void drawCrosshair();
			void drawHealthBar(float ratio);
			void drawAmmunition(int ammo, int maxAmmo);

			void drawChar(char c, float& tempX, float& tempY);
			void drawString(std::string str);

			void showIMGUI();
		};
	}
}