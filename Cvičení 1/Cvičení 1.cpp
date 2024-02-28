#include <iostream>
#include <GLFW/glfw3.h>

using namespace std;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
       
    cout << "OpenGL Information:" << "\n";
    cout << "Vendor: " << "\t" << glGetString(GL_VENDOR) << "\n";
    cout << "Renderer: " << "\t" << glGetString(GL_RENDERER) << "\n";
    cout << "Version: " << "\t" << glGetString(GL_VERSION) << "\n";
    cout << "\n";
    cout << "Program Information:\n";

    double previousTime = glfwGetTime();
    int frameCount = 0;

    cout << "FPS: \t" << "0";

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Measure speed
        double currentTime = glfwGetTime();
        frameCount++;


        // If a second has passed.
        if (currentTime - previousTime >= 1.0)
        {
            // Display the frame count here any way you want.
            cout << "\r" << "FPS: \t" << frameCount;

            frameCount = 0;
            previousTime = currentTime;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* ... more draw-calls ... */
        /* ... */
        /* ... */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}