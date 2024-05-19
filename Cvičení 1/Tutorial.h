#pragma once

#include "LightSystem.h"
#include "Window.h"
#include "Model.h"
#include "Scenes.h"

#include "nuklear/types.h"

class Tutorial {
public:	
	static void init();
	static Scene render(nk_context* context, Window* window, float delta, int fps);
	static Scene load(nk_context* context, Window* window, std::shared_ptr<int> progress);
	static Shader* materialShader;

private:
	static Camera* camera;
	static LightSystem* lights;
	static SpotLight* spotLight;
	static AmbientLight* ambience;
	static PointLight* simpleLight2, * simpleLight3;
	static DirectionalLight* sunlight;
	static Model* coin;
	static Model* terrain;
	static Model* glass;
};
