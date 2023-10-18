#pragma once

#include <memory>
#include <vector>
#include "engine_window.hpp"
#include "engine_pipeline.hpp"
#include "engine_device.hpp"
#include "engine_swap_chain.hpp"


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
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffer();
		void drawFrame();

		VulkanEngineWindow veWindow{WIDTH, HEIGHT, "HELLO VULKAN !"};
		EngineDevice engineDevice{ veWindow };
		EngineSwapChain engineSwapChain{ engineDevice, veWindow.getExtend() };
		std::unique_ptr<EnginePipeline> enginePipline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffer;
		
	};
}