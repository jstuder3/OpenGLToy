#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>

#include "utils.h"
#include "inputHandling.h"


void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int initOpenGL(RenderState* renderState) {

	if (!glfwInit())
	{
		// Initialization failed
	}

	glfwSetErrorCallback(error_callback);
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

	glfwSetKeyCallback(renderState->window, key_callback);
	glfwSetFramebufferSizeCallback(renderState->window, framebuffer_size_callback);

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
		processInputs();
		render(renderState);
		glfwSwapBuffers(renderState->window);
	}
}

void processInputs() {
	glfwPollEvents();
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

	GLint mouseLocation = glGetUniformLocation(renderState->shader->getProgram(), "iMouse");
	double x, y;
	glfwGetCursorPos(renderState->window, &x, &y);
	//std::cout << "X: " << x << ", Y: " << y << std::endl;

	// only forward camera data if the mouse is inside the window
	if (x < 0 || y < 0 || x > width || y > height) {
		x = 0.5 * width;
		y = 0.5 * height;
	}

	glUniform4f(mouseLocation, (float)x, (float)(height-y), 0.0f, 0.0f);

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
