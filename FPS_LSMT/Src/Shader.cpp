#include "Shader.h"
#include "Debug.h"

#include <fstream>
#include <string>
#include <sstream>

using namespace Resources;
using namespace Core::Maths;
using namespace Core::Debug;

Shader::Shader() : shaderProgram(glCreateProgram()) {}

std::string Shader::parseShaderFile(std::string filename)
{
	std::ifstream file(filename);
	if (!file)
	{
		Log::getInstance()->print("Failed to load shader: " + filename, LogType::ERROR);
	}

	Core::Debug::Log::getInstance()->print("Shader loaded " + filename, LogType::INFO);
	std::stringstream buffer;

	if (file)
	{
		buffer << file.rdbuf();
		file.close();
	}

	return buffer.str();
}

void Shader::loadResource()
{
	std::string vert = parseShaderFile("Resources/shaders/" + resourcePath + ".vert");
	std::string frag = parseShaderFile("Resources/shaders/" + resourcePath + ".frag");

	const char* vertexShaderSource = vert.c_str();
	const char* fragmentShaderSource = frag.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLint success;

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[500];
		glGetShaderInfoLog(vertexShader, 512, NULL, &infoLog[0]);
		Log::getInstance()->print("Vertex Shader compile failed : " + resourcePath + "\n" + infoLog, LogType::ERROR);

	};

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[500];
		glGetShaderInfoLog(fragmentShader, 512, NULL, &infoLog[0]);
		Log::getInstance()->print("Fragment Shader compile failed : " + resourcePath + "\n" + infoLog, LogType::ERROR);

	};

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[500];
		glGetProgramInfoLog(shaderProgram, 512, NULL, &infoLog[0]);
		Log::getInstance()->print("Shader link failed : " + resourcePath + "\n" + infoLog, LogType::ERROR);

	}
	Log::getInstance()->print("Shader link successful : " + resourcePath, LogType::INFO);
}

void Shader::use()
{
	glUseProgram(shaderProgram);
}

void Shader::setMatrix4(const char* name, const mat4& m)
{
	GLint uniMvp = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(uniMvp, 1, GL_FALSE, m.e);
}

void Shader::setVec3(const char* name, const vec3& v)
{
	GLint modelLocation4 = glGetUniformLocation(shaderProgram, name);
	glUniform3f(modelLocation4, v.x, v.y, v.z);
}

void Shader::setFloat(const char* name, const float f)
{
	GLint materialShininessLocation = glGetUniformLocation(shaderProgram, name);
	glUniform1f(materialShininessLocation, f);
}

void Shader::setInt(const char* name, const int i)
{
	GLint materialShininessLocation = glGetUniformLocation(shaderProgram, name);
	glUniform1i(materialShininessLocation, i);
}
