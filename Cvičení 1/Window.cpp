#pragma once
#include "Logger.h"
#include "Window.h"
#include "Camera.h"

// Initializing static variables
float Window::lastX = 0.0f;
float Window::lastY = 0.0f;
bool Window::isMouseMoved = false;
Camera* Window::cam = nullptr;

Window::Window(int width, int height, const char* title, bool fullscreen, bool vsync)
    : fullscreen(fullscreen), vsync(vsync) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        std::exit(-1);
    }

    if (fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        window = glfwCreateWindow(mode->width, mode->height, title, monitor, nullptr);
    }
    else {
        window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    }

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    this->set_vsync(vsync);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Window::getWindow() const {
    return window;
}

void Window::set_fullscreen(bool fullscreen) {
    this->fullscreen = fullscreen;
    if (fullscreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
    }
    else {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(window, nullptr, 150, 150, mode->width / 2, mode->height / 2, GLFW_DONT_CARE);
    }
}

void Window::set_vsync(bool vsync)
{
    this->vsync = vsync;

    if (vsync) {
        glfwSwapInterval(1);
        return;
    }

    glfwSwapInterval(0);
}

bool Window::isVSynced() const
{
    return vsync;
}

bool Window::isFullscreen() const {
    return fullscreen;
}


// Static key callback function
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // glfwGetWindowUserPointer() vyt�hne z okna custom pointer, kter� je k n�mu p�i�azen
    // v na�em p��pad� jsem k tomu p�idadil instanci t��dy Window v jej�m konstruktoru
    // static_cast<Window*> je type_casting syntax (p�evod datov�ho typu) - t�m my c++ �ekneme, �e ten pointer je na t��du Window
    Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->handle_key_event(key, action);
    }
}

/*
    Funkce pro handlov�n� inputu v okn�.
*/
void Window::handle_key_event(int key, int action) {

    switch (action) {
        case GLFW_PRESS:
            this->handle_key_press(key, action);
        default:
            break;
    }
}

void Window::handle_key_press(int key, int action) {
    switch (key) {
        case GLFW_KEY_ESCAPE: {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
        case GLFW_KEY_F: {
            fullscreen = !fullscreen;
            set_fullscreen(fullscreen);
            Logger::info("Fullscreen " + std::string(fullscreen ? "enabled" : "disabled"));
            break;
        }
        case GLFW_KEY_V: {
            vsync = !vsync;
            set_vsync(vsync);
            Logger::info("VSync " + std::string(vsync ? "enabled" : "disabled"));
            break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    // Check if any camera was assigned to the window,
    // since default value is null pointer, which could lead
    // to crashes. We do not want that.
    if (Window::cam == nullptr) {
        Logger::warning("No camera was assigned to the window");
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (Window::isMouseMoved)
    {
        Window::lastX = xpos;
        Window::lastY = ypos;
        Window::isMouseMoved = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    Window::cam->onMouseEvent(xoffset, yoffset, GL_TRUE);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Pro vysv�tlen� viz Window::handle_key_event
    Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->handle_scroll_event(xoffset, yoffset);
    }
}

void Window::handle_scroll_event(double xoffset, double yoffset) {
    GLclampf redf = static_cast<GLclampf>(yoffset / 10);
    GLclampf greenf = static_cast<GLclampf>(yoffset / 10);
    GLclampf bluef = static_cast<GLclampf>(yoffset / 10);

    glClearColor(redf, greenf, bluef, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
