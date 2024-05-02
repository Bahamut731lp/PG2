#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "glm/glm.hpp"

#include "App.h"
#include "Camera.h"
#include "Window.h"
#include "OBJLoader.h"
#include "Mesh.h"
#include "Shader.h"

#include "FrameCounter.h"
#include "DebugOutputManager.h"

App::App()
{
    // default constructor
    // nothing to do here (so far...)
    std::cout << "New App constructed\n";
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

    std::cout << "OpenGL Information:" << std::endl;
    std::cout << "Vendor: " << "\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << "\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << "\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "Extensions: " << "\t" << extensionCount << std::endl;


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
}

int App::run()
{
    FrameCounter fps;
    DebugOutputManager debug;

    std::cout << "Debug Output: \t" << (debug.isAvailable ? "yes" : "no") << std::endl;

    OBJLoader test{ "./assets/obj/bunny_tri_vnt.obj" };

    auto camera = Camera{ glm::vec3(0.0f, 0.0f, 0.0f) };
    auto mesh = test.getMesh();
    auto vertexShaderPath = std::filesystem::path("./assets/shaders/basic.vert");
    auto fragmentShaderPath = std::filesystem::path("./assets/shaders/basic.frag");
    auto shader = Shader(vertexShaderPath, fragmentShaderPath);

    Window::cam = &camera;

    shader.setUniform("projection", camera.getProjectionMatrix());

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame

    std::cout << shader.ID << std::endl;
    glfwSetCursorPosCallback(window->getWindow(), Window::mouse_callback);
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window->getWindow()))
    {
        //menu.render();
        // If a second has passed.
        if (fps.hasSecondPassed()) {
            fps.setNumberOfFrames(0);
        }

        // Clearing the window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
        // New GUI frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.onKeyboardEvent(window->getWindow(), deltaTime); // process keys etc

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setUniform("transform", trans);

        // Tady si musim nastudovat, co to vlastnì kurva dìlá.
        // Jako position chápu, front už taky, ale ten up vector je nìjakej zakletej
        //camera.Pitch = 30.0f;

        shader.setUniform("view", camera.getViewMatrix());

        //glm::mat4 projection = glm::mat4(1.0f);
        //projection = glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
        shader.setUniform("projection", camera.getProjectionMatrix());

        mesh.draw(shader);

        // Draw HUD
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