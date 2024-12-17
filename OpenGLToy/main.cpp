// OpenGLToy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "utils.h"

int main()
{
    RenderState* renderState = new RenderState();

    // set up render state
    if (initOpenGL(renderState))
    {
		std::cout << "Game initialization failed!" << std::endl;
		return -1;
    }

	// set up everything we need to render the scene (i.e. shaders, buffers, etc.)
    if(initData(renderState)) {
		std::cout << "Data initialization failed!" << std::endl;
		return -1;
    }

    // render the scene
    renderLoop(renderState);

	// once finished, clean up
	terminate(renderState);

    return 0;
}