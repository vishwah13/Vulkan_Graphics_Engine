#include "app.hpp"
#include <stdexcept>


namespace VulkanEngine {
	App::App()
	{
		createPipelineLayout();
		createPipeline();
		createCommandBuffer();
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
		}
	}
	void App::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO CREATE PIPLINE LAYOUT !!!");
		}
	}
	void App::createPipeline()
	{
		auto piplineConfig = EnginePipeline::defaultPipelineConfigurationInfo(engineSwapChain.width(), engineSwapChain.height());
		piplineConfig.renderPass = engineSwapChain.getRenderPass();
		piplineConfig.pipelineLayout = pipelineLayout;
		enginePipline = std::make_unique<EnginePipeline>(engineDevice, piplineConfig, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv");

	}
	void App::createCommandBuffer()
	{
	}
	void App::drawFrame()
	{
	}
}


