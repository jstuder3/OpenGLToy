#include <iostream>

#include "shader.h"
#include "utils.h"

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	this->vertexShaderSource = const_cast<char*>(vertexShaderSource);
	this->fragmentShaderSource = const_cast<char*>(fragmentShaderSource);

	lastFragmentShaderWriteTime = fs::last_write_time(fragmentShaderSource);

	lastShaderWriteCheck = std::chrono::high_resolution_clock::now();
	constructShader();


}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

void Shader::constructShader() {
	GLuint vertexShader = genShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = genShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	shaderProgram = genProgram(vertexShader, fragmentShader);
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
	// for hot-reloading: check if the fragment shader has been changed. recompile it if necessary.
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - lastShaderWriteCheck).count() >= 250) {

		// wait for file to be ready
		while (!isFileGood(fragmentShaderSource));

		if (lastFragmentShaderWriteTime != fs::last_write_time(fragmentShaderSource)) {
			glUseProgram(0);
			glDeleteProgram(shaderProgram);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			constructShader();
		}

		lastShaderWriteCheck = std::chrono::high_resolution_clock::now();
	}

	glUseProgram(shaderProgram);
}



