#include <iostream>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>

#include "App.h"
#include "FrameCounter.h"
#include "DebugOutputManager.h"
#include "Window.h"
#include "OBJLoader.h"
#include "Mesh.h"

App::App()
{
    // default constructor
    // nothing to do here (so far...)
    std::cout << "New App constructed\n";
    window = new Window(800, 600, "OpenGL Window");;
}

bool App::init()
{
    // init glew
    // http://glew.sourceforge.net/basic.html
    glewInit();
    wglewInit();

    return true;
}

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


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

    std::cout << "Debug Output: \t" << (debug.isAvailable ? "yes" : "no") << std::endl;

    OBJLoader test{ "C:/data/assets/obj/sphere_tri_vnt.obj" };
    Shader q{ "C:/data/resources/basic.vert", "C:/data/resources/basic.frag" };
    auto mesh = test.getMesh();

    while (!glfwWindowShouldClose(window->getWindow()))
    {
        // If a second has passed.
        if (fps.hasSecondPassed())
        {
            // Display the frame count here any way you want.
            std::cout << "FPS: \t" << fps.getNumberOfFrames() << std::endl;
            fps.setNumberOfFrames(0);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mesh.draw(q);

        // Swap front and back buffers
        glfwSwapBuffers(window->getWindow());

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