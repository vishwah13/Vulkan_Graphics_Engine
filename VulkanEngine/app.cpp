#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "app.hpp"
#include <stdexcept>
#include <array>
#include <assert.h>
#include "simple_render_system.hpp"



namespace VulkanEngine {

	App::App()
	{
		loadGameobjects();
	}
	App::~App(){}

	void App::run()
	{
		SimpleRenderSystem simpleRendererSystem{ engineDevice, engineRenderer.getSwapChainRenderPass() };
		while (!veWindow.shouldClose())
		{
			glfwPollEvents();
			
			if (auto commandBuffer = engineRenderer.beginFrame()) {
				engineRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRendererSystem.renderGameobjects(commandBuffer, gameObjects);
				engineRenderer.endSwapChainRenderPass(commandBuffer);
				engineRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(engineDevice.device());
	}
	void App::loadGameobjects()
	{
		std::vector<EngineModel::Vertex> vertices {
			{{0.0f, -0.5f}, { 1.0f,0.0f,0.0f }},
			{ {0.5f, 0.5f}, { 0.0f,1.0f,0.0f } },
			{ {-0.5f, 0.5f}, { 0.0f,0.0f,1.0f } }
		};

		auto engineModel = std::make_shared<EngineModel>(engineDevice, vertices);

		std::vector<glm::vec3> colors{
			{0.925f, .0f, .458f, },
			{1.f, .35f, 0.f},
			{ 0.964f, 0.533f, .298f },
			{ 1.0f, 0.913f, .168f },
			{ .172f, .89f, 0.71f }
		};

		for (auto& color : colors)
		{
			color = glm::pow(color, glm::vec3{2.2f});
		}

		for (int i = 0; i < 40; i++)
		{
			auto triangle = GameObject::createGameObject();
			triangle.model = engineModel;
			triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
			triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
			triangle.color = colors[i % colors.size()];

			gameObjects.push_back(std::move(triangle));
		}

		
	}
	
}


