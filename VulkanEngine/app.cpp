#include "app.hpp"
#include <stdexcept>
#include <array>
#include <assert.h>


namespace VulkanEngine {
	App::App()
	{
		loadModel();
		createPipelineLayout();
		recreateSwapChain();
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
			{{0.0f, -0.5f}, { 1.0f,0.0f,0.0f }},
			{ {0.5f, 0.5f}, { 0.0f,1.0f,0.0f } },
			{ {-0.5f, 0.5f}, { 0.0f,0.0f,1.0f } }
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
		assert(engineSwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigurationInfo pipelineConfig{};
		EnginePipeline::defaultPipelineConfigurationInfo(pipelineConfig);
		pipelineConfig.renderPass = engineSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		enginePipline = std::make_unique<EnginePipeline>(engineDevice, pipelineConfig, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv");

	}
	void App::createCommandBuffer()
	{
		commandBuffer.resize(engineSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = engineDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffer.size());

		if (vkAllocateCommandBuffers(engineDevice.device(), &allocInfo, commandBuffer.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffer !!!");
		}
	}
	void App::freeCommandBuffers()
	{
		vkFreeCommandBuffers(engineDevice.device(), engineDevice.getCommandPool(), static_cast<float>(commandBuffer.size()), commandBuffer.data());
		commandBuffer.clear();
	}
	void App::drawFrame()
	{
		uint32_t imageIndex;
		auto result = engineSwapChain->acquireNextImage(&imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("FAILED TO ACQUIR SWAP CHAIN IMAGE !!!");
		}
		recordCommandBuffer(imageIndex);
		result = engineSwapChain->submitCommandBuffers(&commandBuffer[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || veWindow.wasWindowResized()) {
			veWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO PRESENT SWAP CHAIN IMAGE !!!");
		}
	}
	void App::recreateSwapChain()
	{
		auto extent = veWindow.getExtend();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = veWindow.getExtend();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(engineDevice.device());
		if (engineSwapChain == nullptr) {
			engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent);

		}
		else {
			engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent,std::move(engineSwapChain));
			if (engineSwapChain->imageCount() != commandBuffer.size()) {
				freeCommandBuffers();
				createCommandBuffer();
			}
		}
		createPipeline();
	}
	void App::recordCommandBuffer(int imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("FAILD TO BEGIN RECORDING COMMAND BUFFER !!!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = engineSwapChain->getRenderPass();
		renderPassInfo.framebuffer = engineSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f,0.1f ,0.1f ,1.0f };
		clearValues[1].depthStencil = { 1.0f,0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(engineSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(engineSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0,0}, engineSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer[imageIndex], 0, 1, &scissor);

		enginePipline->bind(commandBuffer[imageIndex]);
		engineModel->bind(commandBuffer[imageIndex]);
		engineModel->draw(commandBuffer[imageIndex]);

		vkCmdEndRenderPass(commandBuffer[imageIndex]);
		if (vkEndCommandBuffer(commandBuffer[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO RECORD COMMAND BUFFER");
		}
	}
}


