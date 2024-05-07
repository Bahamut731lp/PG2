#include <iostream>
#include <string>
#include <sstream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Logger.h"
#include "App.h"
#include "Camera.h"
#include "Window.h"
#include "OBJLoader.h"
#include "Mesh.h"
#include "Shader.h"

#include "Model.h"

#include "SimpleLight.h"

#include "FrameCounter.h"
#include "DebugOutputManager.h"

App::App()
{
    // default constructor
    // nothing to do here (so far...)
    Logger::info("New Application has been constructed.");
    window = new Window(800, 600, "OpenGL Window", false, false);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    Logger::debug("Enabled Backface Culling (CCW)");
}

int App::run()
{
    FrameCounter fps;
    DebugOutputManager debug;
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    Logger::debug("OpenGL Debug Output: " + (debug.isAvailable ? std::string("yes") : std::string("no")));

    // Create camera
    auto camera = Camera{ glm::vec3(0.0f, 15.0f, 0.0f) };
    Window::cam = &camera;

    // Load all shaders
    auto materialShader = Shader(std::filesystem::path("./assets/shaders/material.vert"), std::filesystem::path("./assets/shaders/material.frag"));
    auto meshShader = Shader(std::filesystem::path("./assets/shaders/material.vert"), std::filesystem::path("./assets/shaders/material.frag"));
    auto terrainShader = Shader(std::filesystem::path("./assets/shaders/material.vert"), std::filesystem::path("./assets/shaders/material.frag"));

    // Load all models needed for scene
    auto gate = Model("./assets/obj/gate.obj");
    auto coin = Model("./assets/obj/coin.obj");
    auto terrain = Model("./assets/obj/level_1.obj");

    // Create another instances if needed without having to read files over again
    auto gate2 = Model(gate);

    // Define lights
    auto simpleLight = SimpleLight(glm::vec3(1.0f, 15.0f, -5.0f), 5.0f);

    // Define transforms for all objects
    gate.transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    gate.transform = glm::scale(gate.transform, glm::vec3(0.5f));

    gate2.transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    gate2.transform = glm::scale(gate2.transform, glm::vec3(0.8f));

    // The light is not moving, so we do not have to update position in shader every frame
    materialShader.activate();
    materialShader.setUniform("light.position", simpleLight.position);
    materialShader.setUniform("light.ambient", simpleLight.ambient);
    materialShader.setUniform("light.diffuse", simpleLight.diffusion);
    materialShader.setUniform("light.specular", simpleLight.specular);
    materialShader.setUniform("light.constant", 1.0f);
    materialShader.setUniform("light.linear", 0.09f);
    materialShader.setUniform("light.quadratic", 0.032f);

    terrainShader.activate();
    terrainShader.setUniform("light.position", simpleLight.position);
    terrainShader.setUniform("light.ambient", simpleLight.ambient);
    terrainShader.setUniform("light.diffuse", simpleLight.diffusion);
    terrainShader.setUniform("light.specular", simpleLight.specular);
    terrainShader.setUniform("light.constant", 1.0f);
    terrainShader.setUniform("light.linear", 0.5f);
    terrainShader.setUniform("light.quadratic", 0.0019f);

    // Attach callbacks
    glfwSetCursorPosCallback(window->getWindow(), Window::mouse_callback);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window->getWindow()))
    {
        //menu.render();
        // If a second has passed.
        if (fps.hasSecondPassed()) {
            fps.setNumberOfFrames(0);
        }

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clearing the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process events
        camera.onKeyboardEvent(window->getWindow(), deltaTime);

        // Draw scene
        simpleLight.render(camera);

        // Render all models
        terrain.render(camera, terrainShader);
        coin.render(camera, materialShader);
        gate.render(camera, materialShader);
        gate2.render(camera, materialShader);

        // New GUI frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(200, ImGui::GetTextLineHeightWithSpacing() * 3));
        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::Begin("FPS Counter", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
        // Could be realtime if std::round(1 / deltatime);
        ImGui::Text("FPS: %d", fps.getLastNumberOfFrames());
        ImGui::Text("Vsync: %s", window->isVSynced() ? "True" : "False");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //// Swap front and back buffers
        glfwSwapBuffers(window->getWindow());

        //// Poll for and process events
        glfwPollEvents();
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    std::cout << std::endl;

    return 0;
}

App::~App()
{
    // clean-up
    std::cout << "Bye...\n";
}