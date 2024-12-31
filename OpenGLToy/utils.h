#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "inputManager.h"
#include "shader.h"

struct RenderState
{
	GLFWwindow* window;

	std::shared_ptr<Shader> shader;
	std::shared_ptr<InputManager> inputManager;

	GLuint VAO;
	GLuint VBO;

	float lastMouseX;
	float lastMouseY;
};

int initOpenGL(RenderState* renderState);
int initData(RenderState* renderState);
void processInputs(RenderState* renderState);
void render(RenderState* renderState);
void renderLoop(RenderState* renderState);
void terminate(RenderState* renderState);

int genBuffer();

std::string loadFile(const char* path);
bool isFileGood(const char* path);
