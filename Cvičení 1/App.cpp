#include <iostream>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

#include "App.h"
#include "FrameCounter.h"
#include "DebugOutputManager.h"
#include "VSync.h";
#include "InputManager.h";

App::App()
{
    // default constructor
    // nothing to do here (so far...)
    std::cout << "New App constructed\n";
}

bool App::init()
{
    // init glfw
    // https://www.glfw.org/documentation.html
    if (!glfwInit()) {
        return false;
    }

    // open window (GL canvas) with no special properties
    // https://www.glfw.org/docs/latest/quick.html#quick_create_window
    window = glfwCreateWindow(800, 600, "OpenGL context", NULL, NULL);
    glfwMakeContextCurrent(window);

    // init glew
    // http://glew.sourceforge.net/basic.html
    glewInit();
    wglewInit();

    return true;
}

void App::report(void)
{
    GLint extensionCount;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);

    std::cout << "OpenGL Information:" << std::endl;
    std::cout << "Vendor: " << "\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << "\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << "\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "Extensions: " << "\t" << extensionCount << std::endl;
}

int App::run()
{
    FrameCounter fps;
    DebugOutputManager debug;
    InputManager inputs;

    std::cout << "Debug Output: \t" << (debug.isAvailable ? "yes" : "no") << std::endl;


    glfwSetKeyCallback(window, inputs.keyCallback);

    while (!glfwWindowShouldClose(window))
    {
        // If a second has passed.
        if (fps.hasSecondPassed())
        {
            // Display the frame count here any way you want.
            std::cout << "FPS: \t" << fps.getNumberOfFrames() << std::endl;
            fps.setNumberOfFrames(0);
        }

        // Clear OpenGL canvas, both color buffer and Z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    std::cout << std::endl;

    return 0;
}

App::~App()
{
    // clean-up
    std::cout << "Bye...\n";
}