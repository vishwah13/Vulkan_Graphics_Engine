#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "app.hpp"
#include <stdexcept>
#include <array>
#include <assert.h>



namespace VulkanEngine {
	struct SimplePushConstantData {
		glm::mat2 transform{1.f};
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};
	App::App()
	{
		loadGameobjects();
		createPipelineLayout();
		createPipeline();
	}
	App::~App()
	{
		vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
	}
	void App::run()
	{
		while (!veWindow.shouldClose())
		{
			glfwPollEvents();
			
			if (auto commandBuffer = engineRenderer.beginFrame()) {
				engineRenderer.beginSwapChainRenderPass(commandBuffer);
				renderGameobjects(commandBuffer);
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
	void App::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO CREATE PIPLINE LAYOUT !!!");
		}
	}
	void App::createPipeline()
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigurationInfo pipelineConfig{};
		EnginePipeline::defaultPipelineConfigurationInfo(pipelineConfig);
		pipelineConfig.renderPass = engineRenderer.getSwapChainRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		enginePipline = std::make_unique<EnginePipeline>(engineDevice, pipelineConfig, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv");

	}
	

	void App::renderGameobjects(VkCommandBuffer commandBuffer)
	{
		enginePipline->bind(commandBuffer);

		int i = 0;
		for ( auto& obj: gameObjects)
		{
			i += 1;
			obj.transform2d.rotation = glm::mod<float>(obj.transform2d.rotation + 0.001f * i,2.f * glm::pi<float>());
			SimplePushConstantData push{};
			push.offset = obj.transform2d.translation;
			push.color = obj.color;
			push.transform = obj.transform2d.mat2();

			enginePipline->bind(commandBuffer);
			for (auto& obj : gameObjects)
			{
				SimplePushConstantData push{};
				push.offset = obj.transform2d.translation;
				push.color = obj.color;
				push.transform = obj.transform2d.mat2();

				vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
				obj.model->bind(commandBuffer);
				obj.model->draw(commandBuffer);
			}
		}
	}
}


