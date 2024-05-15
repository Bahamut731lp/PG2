
#include "MainMenu.h"

#include "nuklear/types.h"
#include "Window.h"

MainMenu::MainMenu(nk_context* context, Window* window)
{
    // SETUP
    struct nk_style* s = &context->style;

    context->style.window.background = nk_rgba(0, 0, 0, 0);
    context->style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));

    context->style.button.rounding = 0.0f;
    context->style.button.border = 2.0f;
    context->style.button.border_color = nk_rgba(255, 255, 255, 255);
    context->style.button.normal = nk_style_item_color(nk_rgba(255, 255, 255, 0));
    context->style.button.text_normal = nk_rgba(255, 255, 255, 255);
    context->style.button.hover = nk_style_item_color(nk_rgba(255, 255, 255, 255));
    context->style.button.text_hover = nk_rgba(0, 0, 0, 255);

    // Opening new context
    auto isOpen = nk_begin(context, "Menu", nk_rect(0, 0, window->width, window->height), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND);
    if (!isOpen) {
        nk_end(context);
        return;
    }

    nk_layout_row_static(context, 50, 250, 1);
    nk_label(context, "Main Menu", NK_TEXT_LEFT);

    nk_layout_row_static(context, 50, 250, 2);
    auto isGoingToPlay = nk_button_label(context, "Play");

    nk_layout_row_static(context, 50, 250, 2);
    auto isGoingToOptions = nk_button_label(context, "Options");

    nk_layout_row_static(context, 50, 250, 2);
    auto isClosingWindow = nk_button_label(context, "Exit");
    
    // CLEANUP
    nk_end(context);

    if (isGoingToPlay) {
    }
    if (isClosingWindow) {
        glfwSetWindowShouldClose(window->getWindow(), 1);
    }
}
