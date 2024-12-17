#include <iostream>

#include "shader.h"

#include "utils.h"


Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	GLuint vertexShader = genShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = genShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	shaderProgram = genProgram(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

int Shader::genShader(const char* shaderSource, GLenum shaderType)
{

	std::string fileContent = loadFile(shaderSource);
	const char* fileContentChars = fileContent.c_str(); // have to do this in two steps for some reason. calling .c_str() directly on the function call produces bogus values

	char infoLog[512];
	int success;

	GLuint shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &fileContentChars, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}

int Shader::genProgram(int vertexShader, int fragmentShader) {
	char infoLog[512];
	int success;

	GLuint program;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(0);

	return program;
}

void Shader::use() {
	glUseProgram(shaderProgram);
}



