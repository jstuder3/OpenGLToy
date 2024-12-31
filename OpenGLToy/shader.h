#pragma once
#include <glad/glad.h>
#include <filesystem>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;

class Shader
{
public:
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	~Shader();

	void use();
	GLuint getProgram() { return shaderProgram; }

	void constructShader();

private:
	GLuint shaderProgram;
	char* vertexShaderSource;
	char* fragmentShaderSource;

	fs::file_time_type lastFragmentShaderWriteTime;
	std::chrono::steady_clock::time_point lastShaderWriteCheck;
	int genShader(const char* shaderSource, GLenum shaderType);
	int genProgram(int vertexShader, int fragmentShader);
};
