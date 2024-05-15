#pragma once

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Window.h"
#include "Model.h"
#include "Scenes.h"
#include "LightSystem.h"
#include "nuklear/types.h"

namespace Tutorial {
	Camera* camera;
	Shader* materialShader;
	LightSystem* lights;
	SpotLight* spotLight;
	AmbientLight* ambience;
	PointLight *simpleLight2, *simpleLight3;
	DirectionalLight* sunlight;
	Model *gate, *gate2;
	Model* coin;
	Model* terrain;
	Model* glass;

	void init(nk_context* context);
	Scene render(nk_context* context, Window* window, float delta);
}

