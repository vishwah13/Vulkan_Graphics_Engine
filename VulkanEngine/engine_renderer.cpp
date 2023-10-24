#include "engine_renderer.hpp"

#include <stdexcept>
#include <vector>
#include <array>

namespace VulkanEngine {

	EngineRenderer::EngineRenderer(VulkanEngineWindow& window, EngineDevice& device) : veWindow(window), engineDevice(device)
	{
		recreateSwapChain();
		createCommandBuffer();
	}
	EngineRenderer::~EngineRenderer()
	{
		freeCommandBuffers();
	}
	
	void EngineRenderer::recreateSwapChain()
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
			engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, extent, std::move(engineSwapChain));
			if (engineSwapChain->imageCount() != commandBuffer.size()) {
				freeCommandBuffers();
				createCommandBuffer();
			}
		}
	}

	VkCommandBuffer EngineRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Cannot call beginFrame while already in progress");

		auto result = engineSwapChain->acquireNextImage(&currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("FAILED TO ACQUIR SWAP CHAIN IMAGE !!!");
		}
		isFrameStarted = true;
		auto commandBuffer = getCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("FAILD TO BEGIN RECORDING COMMAND BUFFER !!!");
		}
		return commandBuffer;
	}

	void EngineRenderer::endFrame()
	{
		assert(isFrameStarted && "can't call endFrame while frame is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO RECORD COMMAND BUFFER");
		}

		auto result = engineSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || veWindow.wasWindowResized()) {
			veWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO PRESENT SWAP CHAIN IMAGE !!!");
		}
		isFrameStarted = false;
	}

	void EngineRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "can't call beginSwapChainRenderPass  if frame is not in preogress");
		assert(commandBuffer == getCurrentCommandBuffer() && "can't begin render pass on command buffer from a different frame");
		
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = engineSwapChain->getRenderPass();
		renderPassInfo.framebuffer = engineSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f,0.01f ,0.01f ,1.0f };
		clearValues[1].depthStencil = { 1.0f,0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(engineSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(engineSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0,0}, engineSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void EngineRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "can't call endSwapChainRenderPass if frame is not in preogress");
		assert(commandBuffer == getCurrentCommandBuffer() && "can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	void EngineRenderer::createCommandBuffer()
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
	void EngineRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			engineDevice.device(),
			engineDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffer.size()),
			commandBuffer.data());
		commandBuffer.clear();
	}
}

