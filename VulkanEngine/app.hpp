#pragma once

#include <memory>
#include <vector>
#include "engine_window.hpp"
#include "engine_pipeline.hpp"
#include "engine_device.hpp"
#include "engine_renderer.hpp"
#include "engine_model.hpp"
#include "game_object.hpp"


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
		void loadGameobjects();
		void createPipelineLayout();
		void createPipeline();
		void renderGameobjects(VkCommandBuffer commandBuffers);

		VulkanEngineWindow veWindow{WIDTH, HEIGHT, "HELLO VULKAN !"};
		EngineDevice engineDevice{ veWindow };
		EngineRenderer engineRenderer{ veWindow,engineDevice };
		std::unique_ptr<EnginePipeline> enginePipline;
		VkPipelineLayout pipelineLayout;
		std::vector<GameObject> gameObjects;
		
	};
}