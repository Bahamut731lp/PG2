#pragma once
#include <iostream>
#include <string>
#include <sstream>

// Nuklear "dependencies" import
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

// Glew
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

// Application 
#include "Logger.h"
#include "App.h"
#include "Camera.h"
#include "Window.h"
#include "OBJLoader.h"

#include "Mesh.h"
#include "Shader.h"
#include "Model.h"

#include "FrameCounter.h"
#include "DebugOutputManager.h"

#include "MainMenu.h"
#include "Tutorial.h"

// Nuklear
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_glfw_gl4.h"

App::App()
{
    // default constructor
    // nothing to do here (so far...)
    Logger::info("New Application has been constructed.");
    window = new Window(800, 600, "OpenGL Window", false, false);
    scene = Scene::SceneMainMenu;
}

bool App::init()
{
    // init glew
    // http://glew.sourceforge.net/basic.html
    glewInit();
    wglewInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

void App::report(void)
{
    GLint extensionCount;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);

    std::ostringstream vendor, renderer, version, extensions;

    vendor << "Vendor: " << glGetString(GL_VENDOR) << ", " << "rendered with " << glGetString(GL_RENDERER);
    version << "Version: " << glGetString(GL_VERSION) << ", " << extensionCount << " extensions.";

    Logger::debug(vendor.str());
    Logger::debug(version.str());

    App::configure();
}

void App::configure(void)
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int App::run()
{
    FrameCounter fps;
    DebugOutputManager debug;
    float delta = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float daytime = 0.0f;
    struct nk_context* ctx;
    struct nk_image img;

    Logger::debug("OpenGL Debug Output: " + (debug.isAvailable ? std::string("yes") : std::string("no")));

    //staticLights.calc();

    ctx = nk_glfw3_init(window->getWindow(), NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    {
        /* Load Fonts: if none of these are loaded a default font will be used  */
        /* Load Cursor: if you uncomment cursor loading please hide the cursor */
        struct nk_font_atlas* atlas;
        nk_glfw3_font_stash_begin(&atlas);

        struct nk_font* pixelifySans = nk_font_atlas_add_from_file(atlas, "./assets/fonts/pixelify_sans/pixelify_sans_variable.ttf", 24, 0);
        struct nk_font* roboto = nk_font_atlas_add_from_file(atlas, "./assets/fonts/roboto/Roboto-Regular.ttf", 16, 0);

        nk_glfw3_font_stash_end();
        nk_style_set_font(ctx, &pixelifySans->handle);
    }

    // Attach callbacks
    glfwSetCursorPosCallback(window->getWindow(), Window::mouse_callback);

    // Initialize first scene
    MainMenu::init(ctx);

    while (!glfwWindowShouldClose(window->getWindow()))
    {
        glfwPollEvents();
        nk_glfw3_new_frame();

        float currentFrame = glfwGetTime();
        delta = currentFrame - lastFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        switch (this->scene)
        {
        case Scene::SceneMainMenu:
            this->scene = MainMenu::render(ctx, window);
            break;
        case Scene::SceneTutorial:
            this->scene = Tutorial::render(ctx, window, delta);
            break;
        default:
            Logger::critical("Transition to the undefined scene");
            break;
        }

        // Calculate FPS
        if (fps.hasSecondPassed()) {
            fps.setNumberOfFrames(0);
        }

        /* IMPORTANT: `nk_glfw_render` modifies some global OpenGL state
         * with blending, scissor, face culling, depth test and viewport and
         * defaults everything back into a default state.
         * Make sure to either a.) save and restore or b.) reset your own state after
         * rendering the UI. */
        nk_glfw3_render(NK_ANTI_ALIASING_ON);

        // Restoring ou own config
        App::configure();

        // Swap front and back buffers
        glfwSwapBuffers(window->getWindow());
    }

    std::cout << std::endl;

    return 0;
}

App::~App()
{
    // clean-up
    std::cout << "Bye...\n";
}