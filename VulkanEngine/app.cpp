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

	void App::loadGameobjects()
	{
		std::shared_ptr<EngineModel> engineModel = EngineModel::createModelFromFile(engineDevice, "models/flat_vase.obj");

		auto gameObj = GameObject::createGameObject();
		gameObj.model = engineModel;
		gameObj.transform.translation = { .0f, .5f, 2.5f };
		gameObj.transform.scale = glm::vec3(3.f,3.f,3.f);
		gameObjects.push_back(std::move(gameObj));
	}

}


