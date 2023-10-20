#include "app.hpp"
#include <stdexcept>
#include <array>


namespace VulkanEngine {
	App::App()
	{
		loadModel();
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
			drawFrame();
		}
		vkDeviceWaitIdle(engineDevice.device());
	}
	void App::loadModel()
	{
		std::vector<EngineModel::Vertex> vertices {
			{{0.0f, -0.5f}},
			{ {0.5f, 0.5f} },
			{ {-0.5f, 0.5f} }
		};

		engineModel = std::make_unique<EngineModel>(engineDevice, vertices);
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
		commandBuffer.resize(engineSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = engineDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffer.size());

		if (vkAllocateCommandBuffers(engineDevice.device(), &allocInfo, commandBuffer.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffer !!!");
		}

		for (int i = 0; i < commandBuffer.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffer[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("FAILD TO BEGIN RECORDING COMMAND BUFFER !!!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = engineSwapChain.getRenderPass();
			renderPassInfo.framebuffer = engineSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0,0 };
			renderPassInfo.renderArea.extent = engineSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f,0.1f ,0.1f ,1.0f };
			clearValues[1].depthStencil = { 1.0f,0};
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			enginePipline->bind(commandBuffer[i]);
			engineModel->bind(commandBuffer[i]);
			engineModel->draw(commandBuffer[i]);

			vkCmdEndRenderPass(commandBuffer[i]);
			if (vkEndCommandBuffer(commandBuffer[i]) != VK_SUCCESS) {
				throw std::runtime_error("FAILED TO RECORD COMMAND BUFFER");
			}
		}
	}
	void App::drawFrame()
	{
		uint32_t imageIndex;
		auto result = engineSwapChain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("FAILED TO ACQUIR SWAP CHAIN IMAGE !!!");
		}

		result = engineSwapChain.submitCommandBuffers(&commandBuffer[imageIndex], &imageIndex);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO PRESENT SWAP CHAIN IMAGE !!!");
		}
	}
}


