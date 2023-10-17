#pragma once

#include "engine_window.hpp"
#include "engine_pipeline.hpp"


namespace VulkanEngine {

	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		VulkanEngineWindow veWindow{WIDTH, HEIGHT, "HELLO VULKAN !"};
		EnginePipeline enginePipline{"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
		
	};
}