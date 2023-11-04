#pragma once

#include <memory>
#include <vector>
#include "engine_window.hpp"
#include "engine_device.hpp"
#include "engine_renderer.hpp"
#include "engine_model.hpp"
#include "game_object.hpp"
#include "engine_descriptors.hpp"


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
		

		VulkanEngineWindow veWindow{WIDTH, HEIGHT, "VULKAN GRAPHICS ENGINE"};
		EngineDevice engineDevice{ veWindow };
		EngineRenderer engineRenderer{ veWindow,engineDevice };

		//note: order of declaration matters
		std::unique_ptr<EngineDescriptorPool> globalPool{};
		std::vector<GameObject> gameObjects;
		
	};
}