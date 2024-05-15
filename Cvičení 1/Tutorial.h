#pragma once

#include "LightSystem.h"
#include "Window.h"
#include "Model.h"
#include "Scenes.h"

#include "nuklear/types.h"

namespace Tutorial {
	extern Camera* camera;
	extern Shader* materialShader;
	extern LightSystem* lights;
	extern SpotLight* spotLight;
	extern AmbientLight* ambience;
	extern PointLight* simpleLight2, * simpleLight3;
	extern DirectionalLight* sunlight;
	extern Model* gate, * gate2;
	extern Model* coin;
	extern Model* terrain;
	extern Model* glass;

	extern void init();
	extern Scene render(nk_context* context, Window* window, float delta);
}

