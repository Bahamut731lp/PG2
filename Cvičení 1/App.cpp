#include <iostream>
#include <string>
#include <sstream>

// Nuklear "dependencies" import
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

// Glew
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

// Nuklear
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
#include "nuklear/nuklear.h"
#include "nuklear/nuklear_glfw_gl4.h"

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
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float daytime = 0.0f;
    struct nk_context* ctx;
    struct nk_image img;

    const int ONE_DAY = 84;

    Logger::debug("OpenGL Debug Output: " + (debug.isAvailable ? std::string("yes") : std::string("no")));

    // Create camera
    //auto camera = Camera{ glm::vec3(-2.50f, 15.0f, 0.0f) };
    //Window::cam = &camera;

    // Load all shaders
   /* auto materialShader = Shader(std::filesystem::path("./assets/shaders/material.vert"), std::filesystem::path("./assets/shaders/material.frag"));

    // Load all models needed for scene
    auto gate = Model("./assets/obj/gate.obj");
    auto coin = Model("./assets/obj/coin.obj");
    auto terrain = Model("./assets/obj/level_1.obj");
    auto glass = Model("./assets/obj/glass.obj");

    LightSystem staticLights;
    // Create another instances if needed without having to read files over again
    auto gate2 = Model(gate);

    // Define lights
    SpotLight spotLight;
    AmbientLight ambience;
    PointLight simpleLight2, simpleLight3;
    DirectionalLight sunlight;*/

    // Define transforms for all objects
    /*gate.transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    gate.transform = glm::scale(gate.transform, glm::vec3(0.5f));

    gate2.transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    gate2.transform = glm::scale(gate2.transform, glm::vec3(0.8f));

    coin.transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    coin.transform = glm::scale(coin.transform, glm::vec3(0.25f));

    glass.transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));

    ambience.color = glm::vec3(1.0f);
    ambience.intensity = 0.05f;

    spotLight.position = glm::vec3(0.0f, 12.5f, -8.0f);
    spotLight.direction = glm::vec3(0.0f, -0.2f, 1.0f);

    sunlight.direction = glm::vec3(0.0f, -0.5f, 0.8f);
    sunlight.diffusion = glm::vec3(0.0f);

    simpleLight2.position = glm::vec3(10.0f, 15.0f, 0.0f);
    simpleLight3.position = glm::vec3(-10.0f, 15.0f, 0.0f);*/


    // The light is not moving, so we do not have to update position in shader every frame
    /*staticLights.add(sunlight);
    staticLights.add(ambience);
    staticLights.add(spotLight);
    staticLights.add(simpleLight2);
    staticLights.add(simpleLight3);
    staticLights.add(materialShader);*/

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

    while (!glfwWindowShouldClose(window->getWindow()))
    {
        glfwPollEvents();
        nk_glfw3_new_frame();

        switch (this->scene)
        {
        case Scene::SceneMainMenu:
            MainMenu{ ctx, window };
            break;
        case Scene::SceneLevelOne:
            break;
        default:
            Logger::critical("Transition to the undefined scene");
            break;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate FPS
        if (fps.hasSecondPassed()) {
            fps.setNumberOfFrames(0);
        }
        
        /*float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        daytime = glm::sin((glm::pi<float>() * glfwGetTime() / ONE_DAY) - glm::half_pi<float>());

        float normDayTime = (float)((int)currentFrame % (int)ONE_DAY) / ONE_DAY;

        // Clearing the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Sunlight movement
        sunlight.diffusion = glm::vec3(5 * daytime + 5);

        // Process events
        camera.onKeyboardEvent(window->getWindow(), deltaTime);

        // Send light data to shaders
        staticLights.calc();

        // Render all models
        terrain.render(camera, materialShader);
        coin.render(camera, materialShader);
        gate.render(camera, materialShader);
        gate2.render(camera, materialShader);
        glass.render(camera, materialShader);

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