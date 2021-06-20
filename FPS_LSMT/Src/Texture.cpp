#include "Texture.h"
#include "Debug.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Resources;
using namespace Core::Debug;

void Texture::loadResource()
{
	glGenTextures(1, &data);
	glBindTexture(GL_TEXTURE_2D, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(resourcePath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		Log::getInstance()->print("Loading texture: " + resourcePath, LogType::DEBUG);
	}
	else
	{
		Core::Debug::Log::getInstance()->print("Failed to load texture " + resourcePath, LogType::ERROR);
	}

	stbi_image_free(data);
}