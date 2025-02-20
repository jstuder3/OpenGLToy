# Overview
### What is this?
A simple framework that mimics the functionality of the ShaderToy website, but locally!

### What do I need to run this?
You will need:
- Visual Studio 2022
- [glad](https://github.com/Dav1dde/glad) (takes care of platform-specific OpenGL function bindings; you can find a precompiled version [here](https://glad.dav1d.de/))
- [GLFW](https://www.glfw.org/) (used to create a window and to handle mouse input, download [here](https://www.glfw.org/download))

For a guide on how to include both of these in Visual Studio, take a look [here](https://learnopengl.com/Getting-started/Creating-a-window)

To run the project, open the .sln file in Visual Studio and build it. If you have the necessary dependencies, you should be able to run the project without any issues.

### What do I do?
Similar to ShaderToy, you should only set the fragment shader. An example of a pretty ray marching shader is provided under `shader/ray_marching_fragment_shader.frag`. 

To change which shader is used, you can change the path [here](https://github.com/jstuder3/OpenGLToy/blob/ed4995710f47c11e6382d16f7d6a5ad583005c19/OpenGLToy/utils.cpp#L81).

### What inputs are provided?
The following input is provided, similar to Shadertoy:
- `iResolution: vec2` containing the current view resolution in pixels.
- `iTime: float` containing the time since the start of the program in seconds.
- `iMouse: vec4` containing the current mouse xy positions in pixels (indices 0 and 1), and the location where the mouse button was located when the current click&drag started (zero if not clicked, indices 2 and 3).

Once you can run the project, you should see something like the gif below, and then you can start living out your wildest Shader dreams.

![OpenGLToy_Demo](https://github.com/user-attachments/assets/198d2d54-194e-4717-b22b-16b5bcf1ab18)

