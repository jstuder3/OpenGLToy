#pragma once
#include <glad/glad.h>

class Shader
{
public:
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	~Shader();

	void use();
	GLuint getProgram() { return shaderProgram; }

private:
	GLuint shaderProgram;
	int genShader(const char* shaderSource, GLenum shaderType);
	int genProgram(int vertexShader, int fragmentShader);
};
