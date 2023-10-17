#pragma once

#include "engine_window.hpp"
#include "engine_pipeline.hpp"
#include "engine_device.hpp"


namespace VulkanEngine {

	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		VulkanEngineWindow veWindow{WIDTH, HEIGHT, "HELLO VULKAN !"};
		EngineDevice engineDevice{ veWindow };
		EnginePipeline enginePipline{engineDevice,EnginePipeline::defaultPipelineConfigurationInfo(WIDTH,HEIGHT), "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
		
	};
}