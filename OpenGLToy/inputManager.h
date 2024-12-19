#pragma once
#include <GLFW/glfw3.h>

class InputManager
{
public:

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void mouseButtonCallback(GLFWwindow* window, int key, int action, int mods);

	void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);

	/* this function only serves as a "reflection" towards the instance-specific keyCallback function */
	static void staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/* this function only serves as a "reflection" towards the instance-specific mouseButtonCallback function */
	static void staticMouseButtonCallback(GLFWwindow* window, int key, int action, int mods);

	/* this function only serves as a "reflection" towards the instance-specific mousePositionCallback function */
	static void staticMousePositionCallback(GLFWwindow* window, double xpos, double ypos);

	double* getMouseState();

	void update(GLFWwindow* window);

private:
	/* mouseCoords consists of: currentX, currentY, clickStartX, clickStartY
	 * currentX and currentY are limited to the window coordinates and snap to width/2 and height/2 if outside the window;
	 * clickStartX and clickStartY are the coordinates where the mouse was clicked. they are zero if the mouse is not clicked.
	 */
	double mouseCoords[4];
	bool mouseClicked = false;
	bool mouseClickConsumed = false;

	void updateMouseState(GLFWwindow* window);

};


