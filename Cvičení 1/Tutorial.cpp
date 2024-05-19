#pragma once
#include "Tutorial.h"
#include "Logger.h"
#include <thread>

Camera* Tutorial::camera = nullptr;
Shader* Tutorial::materialShader = nullptr;
LightSystem* Tutorial::lights = nullptr;
SpotLight* Tutorial::spotLight = nullptr;
AmbientLight* Tutorial::ambience = nullptr;
PointLight* Tutorial::simpleLight2, *Tutorial::simpleLight3 = nullptr;
DirectionalLight* Tutorial::sunlight = nullptr;
Model* Tutorial::coin = nullptr;
Model* Tutorial::terrain = nullptr;
Model* Tutorial::glass = nullptr;

void Tutorial::init()
{
	// Create camera
	camera = new Camera{ glm::vec3(-2.50f, 15.0f, 0.0f) };
	Window::cam = camera;

	// Load all models needed for scene
	terrain = new Model("./assets/obj/level_1.obj");
	glass = new Model("./assets/obj/glass.obj");
	coin = new Model("./assets/obj/coin.obj");

	// Define lights
	lights = new LightSystem;
	spotLight = new SpotLight;
	ambience = new AmbientLight;
	simpleLight2 = new PointLight;
	simpleLight3 = new PointLight;
	sunlight = new DirectionalLight;

	// Define initial transforms for all objects
	ambience->color = glm::vec3(1.0f);
	ambience->intensity = 0.05f;

	spotLight->position = glm::vec3(50.0f, 12.0f, 2.5f);
	spotLight->direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	spotLight->diffusion = glm::vec3(200.0f);

	simpleLight2->position = glm::vec3(50.0f, 15.0f, 2.5f);
	simpleLight2->diffusion = glm::vec3(30.0f);

	sunlight->ambient = glm::vec3(0.02f);
	sunlight->diffusion = glm::vec3(0.5f);

	// The light is not moving, so we do not have to update position in shader every frame
	lights->add(ambience);
	lights->add(spotLight);
	lights->add(simpleLight2);
	lights->add(sunlight);
	lights->add(*materialShader);
}

Scene Tutorial::render(nk_context* context, Window* window, float delta, int fps)
{
	const int ONE_DAY = 16;
	const auto gametime = glfwGetTime();
	auto daytime = glm::sin(((2 * glm::pi<float>() / ONE_DAY) * gametime));

	// Opening new window
	nk_style_push_style_item(context, &context->style.window.fixed_background, nk_style_item_color(nk_rgba(0, 0, 0, 128)));

	if (!nk_begin(context, "FPS Counter", nk_rect(0, 0, 640, 96), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND)) {
		nk_end(context);
		return Scene::SceneTutorial;
	}

	nk_layout_row_dynamic(context, 20, 1);
	nk_label(context, std::string("FPS: " + std::to_string(fps)).c_str(), NK_TEXT_LEFT);

	nk_layout_row_dynamic(context, 20, 1);
	nk_label(context, std::string("Daytime: " + std::to_string(daytime)).c_str(), NK_TEXT_LEFT);

	nk_layout_row_dynamic(context, 20, 1);
	auto camera_front = std::string("X:" + std::to_string(camera->Front.x) + ", " + "Y:" + std::to_string(camera->Front.y) + ", " + "Z:" + std::to_string(camera->Front.z));
	nk_label(context, std::string("Front: " + camera_front).c_str(), NK_TEXT_LEFT);

	auto sine_wave = glm::sin(glm::pi<float>() * glfwGetTime());

	// Sunlight movement
	// Send light data to shaders
	simpleLight2->position.x = 30.0f + 20 * daytime;

	coin->transform = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 5.0f, 2.5f));
	coin->transform = glm::rotate(coin->transform, (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

	glass->transform = glm::translate(glm::mat4(1.0f), glm::vec3(18.0f, 5 + sine_wave, 2.0f));

	lights->calc();

	terrain->render(*camera, *materialShader);
	glass->render(*camera, *materialShader);
	coin->render(*camera, *materialShader);

	// Process events
	Window::cam->onKeyboardEvent(glfwGetCurrentContext(), delta);

	nk_end(context);
	nk_style_pop_style_item(context);

	return Scene::SceneTutorial;
}

Scene Tutorial::load(nk_context* context, Window* window, std::shared_ptr<int> progress)
{
	// Used to be thread here, but I just gave up trying to make it work with OpenGL.
	if (*progress == 1) {
		Tutorial::init();
		glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		return Scene::SceneTutorial;
	}

	// Opening new window
	if (!nk_begin(context, "Loading", nk_rect(0, 0, window->width, window->height), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND)) {
		nk_end(context);
		return Scene::LoadToTutorial;
	}

	nk_layout_row_dynamic(context, window->height - 25, 1);
	nk_label(context, "Loading...", NK_TEXT_ALIGN_BOTTOM | NK_TEXT_ALIGN_RIGHT);
	
	nk_end(context);
	*progress = 1;

	return Scene::LoadToTutorial;
}
