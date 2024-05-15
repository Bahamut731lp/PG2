#pragma once
#include "Tutorial.h"

namespace Tutorial {
	Camera* camera = nullptr;
	Shader* materialShader = nullptr;
	LightSystem* lights = nullptr;
	SpotLight* spotLight = nullptr;
	AmbientLight* ambience = nullptr;
	PointLight* simpleLight2, * simpleLight3 = nullptr;
	DirectionalLight* sunlight = nullptr;
	Model* gate, * gate2 = nullptr;
	Model* coin = nullptr;
	Model* terrain = nullptr;
	Model* glass = nullptr;
}

void Tutorial::init()
{
	using namespace Tutorial;

	// Create camera
	camera = new Camera{ glm::vec3(-2.50f, 15.0f, 0.0f) };
	Window::cam = camera;

	// Load all shaders
	materialShader = new Shader(std::filesystem::path("./assets/shaders/material.vert"), std::filesystem::path("./assets/shaders/material.frag"));

	// Load all models needed for scene
	gate = new Model("./assets/obj/gate.obj");
	coin = new Model("./assets/obj/coin.obj");
	terrain = new Model("./assets/obj/level_1.obj");
	glass = new Model("./assets/obj/glass.obj");

	// Create another instances if needed without having to read files over again
	gate2 = new Model(*gate);

	// Define lights
	lights = new LightSystem;
	spotLight = new SpotLight;
	ambience = new AmbientLight;
	simpleLight2 = new PointLight;
	simpleLight3 = new PointLight;
	sunlight = new DirectionalLight;

	// Define initial transforms for all objects
	gate->transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
	gate->transform = glm::scale(gate->transform, glm::vec3(0.5f));

	gate2->transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
	gate2->transform = glm::scale(gate2->transform, glm::vec3(0.8f));

	coin->transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
	coin->transform = glm::scale(coin->transform, glm::vec3(0.25f));

	glass->transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));

	ambience->color = glm::vec3(1.0f);
	ambience->intensity = 0.05f;

	spotLight->position = glm::vec3(0.0f, 12.5f, -8.0f);
	spotLight->direction = glm::vec3(0.0f, -0.2f, 1.0f);

	sunlight->direction = glm::vec3(0.0f, -0.5f, 0.8f);
	sunlight->diffusion = glm::vec3(0.0f);

	simpleLight2->position = glm::vec3(10.0f, 15.0f, 0.0f);
	simpleLight3->position = glm::vec3(-10.0f, 15.0f, 0.0f);

	// The light is not moving, so we do not have to update position in shader every frame
	lights->add(*ambience);
	lights->add(*spotLight);
	lights->add(*simpleLight2);
	lights->add(*simpleLight3);
	lights->add(*materialShader);
	lights->add(*sunlight);

	lights->calc();

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Scene Tutorial::render(nk_context* context, Window* window, float delta)
{
	const int ONE_DAY = 16;
	auto daytime = glm::sin((glm::pi<float>() * glfwGetTime() / ONE_DAY) - glm::half_pi<float>());

	// Sunlight movement
	sunlight->diffusion = glm::vec3(5 * daytime + 5);

	// Process events
	Window::cam->onKeyboardEvent(window->getWindow(), delta);

	// Send light data to shaders
	lights->calc();

	// Render all models
	terrain->render(*camera, *materialShader);
	coin->render(*camera, *materialShader);
	gate->render(*camera, *materialShader);
	gate2->render(*camera, *materialShader);
	glass->render(*camera, *materialShader);

	return Scene::SceneTutorial;
}
