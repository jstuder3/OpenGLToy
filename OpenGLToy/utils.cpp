#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>

#include "utils.h"
#include "inputManager.h"


void errorCallback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int initOpenGL(RenderState* renderState) {

	if (!glfwInit())
	{
		// Initialization failed
	}

	glfwSetErrorCallback(errorCallback);
	renderState->window = glfwCreateWindow(800, 600, "OpenGLToy", NULL, NULL);
	if (!renderState->window)
	{
		// Window or OpenGL context creation failed
	}

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwMakeContextCurrent(renderState->window);

	renderState->inputManager = std::make_shared<InputManager>();

	// bind the input manager to the window so we can access it in the input callbacks
	glfwSetWindowUserPointer(renderState->window, renderState->inputManager.get());

	// bind key inputs
	glfwSetKeyCallback(renderState->window, InputManager::staticKeyCallback);

	// bind mouse inputs
	glfwSetMouseButtonCallback(renderState->window, InputManager::staticMouseButtonCallback);
	glfwSetCursorPosCallback(renderState->window, InputManager::staticMousePositionCallback);

	// bind framebuffer size callback
	glfwSetFramebufferSizeCallback(renderState->window, framebufferSizeCallback);

	int version = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
	if (version == 0) {
		printf("Failed to initialize OpenGL context\n");
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(renderState->window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);

	return 0;
}

int initData(RenderState* renderState) {

	float vertices[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	std::shared_ptr<Shader> shader = std::make_shared<Shader>("shaders/pass_through_vertex_shader.vert", "shaders/ray_marching_fragment_shader.frag");
	// transfer all values to the render state for later use
	renderState->VAO = VAO;
	renderState->VBO = VBO;
	renderState->shader = shader;

	return 0;
}

void renderLoop(RenderState* renderState) {
	while (!glfwWindowShouldClose(renderState->window))
	{
		processInputs(renderState);
		render(renderState);
		glfwSwapBuffers(renderState->window);
	}
}

void processInputs(RenderState* renderState) {
	renderState->inputManager->update(renderState->window);
}

void render(RenderState* renderState) {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderState->shader->use();
	glBindVertexArray(renderState->VAO);

	int width, height;
	glfwGetFramebufferSize(renderState->window, &width, &height);
	GLint resolutionLocation = glGetUniformLocation(renderState->shader->getProgram(), "iResolution");
	glUniform2f(resolutionLocation, (float)width, (float)height);

	GLint timeLocation = glGetUniformLocation(renderState->shader->getProgram(), "iTime");
	glUniform1f(timeLocation, (float)glfwGetTime());

	// stuff for mouse
	GLint mouseLocation = glGetUniformLocation(renderState->shader->getProgram(), "iMouse");
	double* mouseState = renderState->inputManager->getMouseState();
	glUniform4f(mouseLocation, (float)mouseState[0], (float)mouseState[1], (float)mouseState[2], (float)mouseState[3]);
	
	std::cout << "Mouse: " << mouseState[0] << " " << mouseState[1] << " " << mouseState[2] << " " << mouseState[3] << std::endl;

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void terminate(RenderState* renderState) {
	glfwDestroyWindow(renderState->window);
	glfwTerminate();
	delete renderState;
}

std::string loadFile(const char* path) {
	std::string fileContent;
	std::ifstream file;
	try {
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(path);
		std::stringstream fileStream;
		fileStream << file.rdbuf();
		file.close();
		fileContent = fileStream.str();
		return fileContent;

	}
	catch (std::ifstream::failure e) {
		std::cerr << "ERROR::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
		exit(1);
	}

}

bool isFileGood(const char* path) {
	std::ifstream file(path);
	return file.good();
}
