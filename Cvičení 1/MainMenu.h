#pragma once
#include "Window.h"
#include "Scenes.h"
#include "nuklear/types.h"

class MainMenu
{
public:
	static void init(nk_context* context);
	static Scene render(nk_context* context, Window* window);
};

