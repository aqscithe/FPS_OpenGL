#include "PlayerUI.h"

#include "Model.h"
#include "ToolBox3D.h"

using namespace Core::Maths::ToolBox3D;

Gameplay::UI::PlayerUI::PlayerUI()
{
	uiShader = Resources::ResourcesManager::getInstance()->addResource<Shader>("shaderInGameUI");
	fontShader = Resources::ResourcesManager::getInstance()->addResource<Shader>("shaderInGameFont");
}

void Gameplay::UI::PlayerUI::drawGUI(float healthRatio, int ammo, int maxAmmo)
{
	drawCrosshair();
	drawHealthBar(healthRatio);
	drawAmmunition(ammo, maxAmmo);
}

void Gameplay::UI::PlayerUI::drawCrosshair()
{
	vec2 sh[5];
	sh[0] = { 0.f * csShift, 0.f * csShift };
	sh[1] = { 1.f * csShift, 0.f * csShift };
	sh[2] = {-1.f * csShift, 0.f * csShift };
	sh[3] = { 0.f * csShift, 1.f * csShift };
	sh[4] = { 0.f * csShift,-1.f * csShift };

	for (int i = 0; i < 5; ++i)
	{
		float x[4];
		x[0] =	csThickness * sh[i].y + csHeight * sh[i].x + csSize * (i == 0) + csPos.x + sh[i].x;
		x[1] =	csThickness * sh[i].y - csHeight * sh[i].x + csSize * (i == 0) + csPos.x + sh[i].x;
		x[2] = -csThickness * sh[i].y - csHeight * sh[i].x - csSize * (i == 0) + csPos.x + sh[i].x;
		x[3] = -csThickness * sh[i].y + csHeight * sh[i].x - csSize * (i == 0) + csPos.x + sh[i].x;
		float y[4];
		y[0] =  csThickness * sh[i].x + csHeight * sh[i].y + csSize * (i == 0) + csPos.y + sh[i].y;
		y[1] =  csThickness * sh[i].x - csHeight * sh[i].y - csSize * (i == 0) + csPos.y + sh[i].y;
		y[2] = -csThickness * sh[i].x - csHeight * sh[i].y - csSize * (i == 0) + csPos.y + sh[i].y;
		y[3] = -csThickness * sh[i].x + csHeight * sh[i].y + csSize * (i == 0) + csPos.y + sh[i].y;

		float vertices[] = {
			x[0], y[0], 0.f,
			x[1], y[1], 0.f,
			x[2], y[2], 0.f,
			x[3], y[3], 0.f,
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3,
		};

		unsigned int EBO, VAO, VBO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		Core::Debug::Assertion::assertShrdPtr(uiShader);
		uiShader->use();

		//Color
		uiShader->setVec3("color", csColor);

		glBindVertexArray(VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		glDeleteVertexArrays(1, &VAO);
	}
}

void Gameplay::UI::PlayerUI::drawHealthBar(float ratio)
{
	//actual bar
	{
		float vertices[] = {
			 hbWidth * ratio + hbPos.x + hbWidth * ratio,  hbHeight + hbPos.y, 0.f,
			 hbWidth * ratio + hbPos.x + hbWidth * ratio, -hbHeight + hbPos.y, 0.f,
			-hbWidth * ratio + hbPos.x + hbWidth * ratio, -hbHeight + hbPos.y, 0.f,
			-hbWidth * ratio + hbPos.x + hbWidth * ratio,  hbHeight + hbPos.y, 0.f,
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3,
		};

		unsigned int EBO, VAO, VBO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		Core::Debug::Assertion::assertShrdPtr(uiShader);
		uiShader->use();

		//Color
		uiShader->setVec3("color", hbColor);

		glBindVertexArray(VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		glDeleteVertexArrays(1, &VAO);
	}

	//lerped bar
	{
		float deltaTime = Application::getInstance()->time.deltaTime();
		lerpedRatio = constantLerp(lerpedRatio, ratio, lerpSpeed * deltaTime);

		float vertices[] = {
			 hbWidth * lerpedRatio + hbPos.x + hbWidth * lerpedRatio,  hbHeight + hbPos.y, 0.f,
			 hbWidth * lerpedRatio + hbPos.x + hbWidth * lerpedRatio, -hbHeight + hbPos.y, 0.f,
			-hbWidth * lerpedRatio + hbPos.x + hbWidth * lerpedRatio, -hbHeight + hbPos.y, 0.f,
			-hbWidth * lerpedRatio + hbPos.x + hbWidth * lerpedRatio,  hbHeight + hbPos.y, 0.f,
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3,
		};

		unsigned int EBO, VAO, VBO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		Core::Debug::Assertion::assertShrdPtr(uiShader);
		uiShader->use();

		//Color
		uiShader->setVec3("color", hbLerpColor);

		glBindVertexArray(VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		glDeleteVertexArrays(1, &VAO);
	}

	//background bar
	{
		float h = hbHeight * edgeH;
		float w = hbWidth * edgeW;

		float vertices[] = {
			 w + hbPos.x + hbWidth,  h + hbPos.y, 0.f,
			 w + hbPos.x + hbWidth, -h + hbPos.y, 0.f,
			-w + hbPos.x + hbWidth, -h + hbPos.y, 0.f,
			-w + hbPos.x + hbWidth,  h + hbPos.y, 0.f,
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3,
		};

		unsigned int EBO, VAO, VBO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		Core::Debug::Assertion::assertShrdPtr(uiShader);
		uiShader->use();

		//Color
		uiShader->setVec3("color", hbBackColor);

		glBindVertexArray(VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		glDeleteVertexArrays(1, &VAO);
	}
}

void Gameplay::UI::PlayerUI::drawAmmunition(int ammo, int maxAmmo)
{
	drawString(std::to_string(ammo) + '/' + std::to_string(maxAmmo));
}

void Gameplay::UI::PlayerUI::drawChar(char c, float& tempX, float& tempY)
{
	Character* ch = Application::getInstance()->font.ch;

	glActiveTexture(GL_TEXTURE0);

	float x = tempX + ch[c].bearing.x * fontSize;
	float y = tempY - (ch[c].size.x - ch[c].bearing.y) * fontSize;

	float h = ch[c].size.x * fontSize;
	float w = ch[c].size.y * fontSize;

	float vertices[] = {
			x, y + h, 0.f, 0.f,
			x,     y, 0.f, 1.f,
		x + w,     y, 1.f, 1.f,
		x + w, y + h, 1.f, 0.f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int EBO, VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindTexture(GL_TEXTURE_2D, ch[c].texture);

	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	Core::Debug::Assertion::assertShrdPtr(fontShader);
	fontShader->use();

	//Color
	fontShader->setVec3("color", fontColor);

	glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	tempX += (ch[c].advance >> 6) * fontSize;

	glDisable(GL_BLEND);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAO);
}

void Gameplay::UI::PlayerUI::drawString(std::string str)
{
	unsigned int length = (int)str.length();
	float x = ammPos.x;
	float y = ammPos.y;

	for (unsigned int i = 0; i < length; ++i)
	{
		drawChar(str.c_str()[i], x, y);
	}
}

void Gameplay::UI::PlayerUI::showIMGUI()
{
	if (ImGui::TreeNode("Player UI"))
	{
		if (ImGui::TreeNode("Health Bar"))
		{
			ImGui::DragFloat2("position", hbPos.e);
			ImGui::ColorEdit3("color", hbColor.e);
			ImGui::ColorEdit3("lerp color", hbLerpColor.e);
			ImGui::ColorEdit3("back color", hbBackColor.e);
			ImGui::DragFloat("height", &hbHeight);
			ImGui::DragFloat("width", &hbWidth);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Crosshair"))
		{
			ImGui::ColorEdit3("color", csColor.e);
			ImGui::DragFloat("size", &csSize);
			ImGui::DragFloat("height", &csHeight);
			ImGui::DragFloat("thickness", &csThickness);
			ImGui::DragFloat("shift", &csShift);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Ammunitions"))
		{
			ImGui::ColorEdit3("color", fontColor.e);
			ImGui::DragFloat2("position", ammPos.e);
			ImGui::DragFloat("font size", &fontSize);

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}
