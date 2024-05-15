
#include "MainMenu.h"
#include "Tutorial.h"

#include "nuklear/nuklear.h"
#include "Window.h"
#include "Logger.h"

void MainMenu::init(nk_context* context)
{
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
}

Scene MainMenu::render(nk_context* context, Window* window)
{
    bool isButtonHovered = false;

    // Opening new window
    if (!nk_begin(context, "Menu", nk_rect(0, 0, window->width, window->height), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND)) {
        nk_end(context);
        return Scene::SceneMainMenu;
    }

    nk_layout_row_dynamic(context, 50, 1);
    nk_label(context, "Main Menu", NK_TEXT_LEFT);

    nk_layout_row_dynamic(context, 50, 2);
    auto isGoingToPlay = nk_button_label(context, "Play");
    auto bounds = nk_widget_bounds(context);
    //bounds.y -= bounds.h;
    bounds.x -= bounds.w;
    isButtonHovered |= nk_input_is_mouse_hovering_rect(&context->input, bounds);

    nk_layout_row_dynamic(context, 50, 2);
    auto isGoingToOptions = nk_button_label(context, "Options");
    bounds = nk_widget_bounds(context);
    //bounds.y -= bounds.h;
    bounds.x -= bounds.w;
    isButtonHovered |= nk_input_is_mouse_hovering_rect(&context->input, bounds);

    nk_layout_row_dynamic(context, 50, 2);
    auto isClosingWindow = nk_button_label(context, "Exit");
    bounds = nk_widget_bounds(context);
    //bounds.y -= bounds.h;
    bounds.x -= bounds.w;
    isButtonHovered |= nk_input_is_mouse_hovering_rect(&context->input, bounds);

    auto cursor = isButtonHovered ? GLFW_HAND_CURSOR : GLFW_NOT_ALLOWED_CURSOR;
    glfwSetCursor(window->getWindow(), glfwCreateStandardCursor(cursor));

    // CLEANUP
    nk_end(context);


    if (isGoingToPlay) {
        Tutorial::init();
        return Scene::SceneTutorial;
    }
    if (isClosingWindow) {
        glfwSetWindowShouldClose(window->getWindow(), 1);
    }

    return Scene::SceneMainMenu;
}
