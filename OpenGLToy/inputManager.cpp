#include "inputManager.h"

#include <iostream>

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int key, int action, int mods)
{
	if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mouseClicked = true;
		mouseClickConsumed = false;
		std::cout << "Mouse clicked!" << std::endl;
	}
	if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouseClicked = false;
		mouseClickConsumed = false;
		std::cout << "Mouse released!" << std::endl;
	}

	updateMouseState(window);

}

void InputManager::mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	updateMouseState(window);
}


/* this function only serves as a "reflection" towards the instance-specific keyCallback function */
void InputManager::staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	InputManager* instance = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->keyCallback(window, key, scancode, action, mods);
	}
}

/* this function only serves as a "reflection" towards the instance-specific mouseButtonCallback function */
void InputManager::staticMouseButtonCallback(GLFWwindow* window, int key, int action, int mods) {
	InputManager* instance = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->mouseButtonCallback(window, key, action, mods);
	}
}

/* this function only serves as a "reflection" towards the instance-specific mousePositionCallback function */
void InputManager::staticMousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
	InputManager* instance = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
	if (instance) {
		instance->mousePositionCallback(window, xpos, ypos);
	}
}

double* InputManager::getMouseState() {
	return mouseCoords;
}

void InputManager::update(GLFWwindow* window) {
	glfwPollEvents();
}


void InputManager::updateMouseState(GLFWwindow* window) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	// only forward data if the mouse is inside the window
	if (x < 0 || y < 0 || x > width || y > height) {
		x = 0.5 * width;
		y = 0.5 * height;
	}

	mouseCoords[0] = x;
	mouseCoords[1] = y;

	// only update thee clickStartX and clickStartY if the mouse was not clicked before
	if (mouseClicked && !mouseClickConsumed) {
		mouseClickConsumed = true;
		mouseCoords[2] = x;
		mouseCoords[3] = y;
	}
	else if (mouseClicked && mouseClickConsumed) {
		// do nothing
	}
	else {
		mouseCoords[2] = 0.0;
		mouseCoords[3] = 0.0;
	}
}



