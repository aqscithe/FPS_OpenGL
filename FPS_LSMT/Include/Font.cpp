#include "Font.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int Resources::Font::init()
{
	// loading lybrary
	FT_Library font;
	if (FT_Init_FreeType(&font))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	// loading font
	FT_Face face;
	if (FT_New_Face(font, "Resources/fonts/productsansregular.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -2;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// loading every ASCII character
	for (int i = 0; i < 128; ++i)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// creating font texture
		unsigned int tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		ch[i].texture = tex;
		ch[i].size = { (float)face->glyph->bitmap.rows, (float)face->glyph->bitmap.width };
		ch[i].bearing = { (float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top };
		ch[i].advance = face->glyph->advance.x;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(font);

	return 0;
}