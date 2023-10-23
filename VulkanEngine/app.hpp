#pragma once

#include <memory>
#include <vector>
#include "engine_window.hpp"
#include "engine_pipeline.hpp"
#include "engine_device.hpp"
#include "engine_swap_chain.hpp"
#include "engine_model.hpp"


namespace VulkanEngine {

	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		App();
		~App();

		App(const App&) = delete;
		App &operator=(const App &) = delete;

		void run();
	private:
		void loadModel();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffer();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		VulkanEngineWindow veWindow{WIDTH, HEIGHT, "HELLO VULKAN !"};
		EngineDevice engineDevice{ veWindow };
		std::unique_ptr<EngineSwapChain> engineSwapChain;
		std::unique_ptr<EnginePipeline> enginePipline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffer;
		std::unique_ptr<EngineModel> engineModel;
		
	};
}