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
#include <numeric>



namespace VulkanEngine {

	struct GlobalUbo
	{
		alignas(16) glm::mat4 projectionView{1.f};
		alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
	};

	App::App()
	{
		globalPool = EngineDescriptorPool::Builder(engineDevice)
			.setMaxSets(EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameobjects();
	}
	App::~App() {}

	void App::run()
	{
		std::vector<std::unique_ptr<EngineBuffer>> uboBuffer(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffer.size(); i++) {
			uboBuffer[i] = std::make_unique<EngineBuffer>(
				engineDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT //| VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			);
			uboBuffer[i]->map();
		}

		auto globalSetLayout = EngineDescriptorSetLayout::Builder(engineDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = uboBuffer[i]->descriptorInfo();
			EngineDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRendererSystem{ engineDevice, engineRenderer.getSwapChainRenderPass(),globalSetLayout->getDescriptorSetLayout()};
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
				int frameIndex = engineRenderer.getFrameIndex();
				FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera,globalDescriptorSets[frameIndex]};
				//update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffer[frameIndex]->writeToBuffer(&ubo);
				uboBuffer[frameIndex]->flush();

				//render
				engineRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRendererSystem.renderGameobjects(frameInfo, gameObjects);
				engineRenderer.endSwapChainRenderPass(commandBuffer);
				engineRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(engineDevice.device());
	}

	void App::loadGameobjects()
	{
		std::shared_ptr<EngineModel> engineModel = EngineModel::createModelFromFile(engineDevice, "models/teapot.obj");

		auto gameObj = GameObject::createGameObject();
		gameObj.model = engineModel;
		gameObj.transform.translation = { .0f, .5f, 2.5f };
		gameObj.transform.scale = glm::vec3{.5f};
		gameObj.transform.rotation = glm::vec3(0.f, 0.f, 0.f);
		gameObjects.push_back(std::move(gameObj));
	}

}


