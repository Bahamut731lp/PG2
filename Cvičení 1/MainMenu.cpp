
#include "MainMenu.h"
#include "nuklear/types.h"
#include "Window.h"



MainMenu::MainMenu(nk_context* context, Window* window)
{

    auto test = nk_begin(context, "Main Menu", nk_rect(100, 100, 200, 200), NK_WINDOW_TITLE);

    // Begin Nuklear GUI layout
    if (test) {

        // Add GUI elements here
        nk_layout_row_dynamic(context, 30, 1);
        if (nk_button_label(context, "Start Game")) {
            // Handle start game action
        }

        nk_layout_row_dynamic(context, 30, 1);
        if (nk_button_label(context, "Options")) {
            // Handle options action
        }

        nk_layout_row_dynamic(context, 30, 1);
        if (nk_button_label(context, "Exit")) {
            // Handle exit action
            glfwSetWindowShouldClose(window->getWindow(), GLFW_TRUE);
        }
    }
}
