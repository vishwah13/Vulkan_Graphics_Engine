#pragma once
#pragma once

#include <memory>
#include <vector>
#include <cassert>
#include "engine_window.hpp"
#include "engine_device.hpp"
#include "engine_swap_chain.hpp"
#include "engine_model.hpp"


namespace VulkanEngine {

	class EngineRenderer {
	public:

		EngineRenderer(VulkanEngineWindow &window, EngineDevice &device);
		~EngineRenderer();

		EngineRenderer(const EngineRenderer&) = delete;
		EngineRenderer& operator=(const EngineRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return engineSwapChain->getRenderPass(); }
		bool isFrameInProgress() const {return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffer[currentImageIndex]; }

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	private:
		void createCommandBuffer();
		void freeCommandBuffers();
		void recreateSwapChain();

		VulkanEngineWindow& veWindow;
		EngineDevice& engineDevice;
		std::unique_ptr<EngineSwapChain> engineSwapChain;
		std::vector<VkCommandBuffer> commandBuffer;

		uint32_t currentImageIndex;
		bool isFrameStarted;
	};
}