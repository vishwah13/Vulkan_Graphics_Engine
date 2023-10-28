#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "app.hpp"
#include "engine_Camera.hpp"
#include "Keyboard.hpp"
#include "simple_render_system.hpp"
#include <stdexcept>
#include <array>
#include <assert.h>
#include <chrono>




namespace VulkanEngine {

	App::App()
	{
		loadGameobjects();
	}
	App::~App() {}

	void App::run()
	{
		SimpleRenderSystem simpleRendererSystem{ engineDevice, engineRenderer.getSwapChainRenderPass() };
		EngineCamera camera{};
		//camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
		camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

		auto viewrObject = GameObject::createGameObject();
		Keyboard cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();
		
		
		while (!veWindow.shouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.moveInPlaneXZ(veWindow.getGLFWwindow(), frameTime, viewrObject);
			camera.setViewYXZ(viewrObject.transform.translation, viewrObject.transform.rotation);

			float aspect = engineRenderer.getAspectRatio();
			//camera.setOrthographicProjection(-aspect,aspect, -1, 1, -1, 1);
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			if (auto commandBuffer = engineRenderer.beginFrame()) {
				engineRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRendererSystem.renderGameobjects(commandBuffer, gameObjects,camera);
				engineRenderer.endSwapChainRenderPass(commandBuffer);
				engineRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(engineDevice.device());
	}

	std::unique_ptr<EngineModel> createCubeModel(EngineDevice& device, glm::vec3 offset) {
		EngineModel::Builder modelbuilder{};
		modelbuilder.vertices = {

			// left face (white)
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
		{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

			// right face (yellow)
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
		{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

			// bottom face (red)
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
		{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

			// nose face (blue)
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
		{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
		{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto& v : modelbuilder.vertices) {
			v.position += offset;
		}
		modelbuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
						  12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };
		return std::make_unique<EngineModel>(device, modelbuilder);
	}

	void App::loadGameobjects()
	{
		std::shared_ptr<EngineModel> engineModel = createCubeModel(engineDevice, { .0f,.0f,.0f });

		auto cube = GameObject::createGameObject();
		cube.model = engineModel;
		cube.transform.translation = { .0f, .0f, 2.5f };
		cube.transform.scale = { .5f, .5f, .5f };
		gameObjects.push_back(std::move(cube));
	}

}


