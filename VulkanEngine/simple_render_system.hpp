#pragma once

#include <memory>
#include <vector>
#include "engine_pipeline.hpp"
#include "engine_device.hpp"
#include "engine_model.hpp"
#include "engine_Camera.hpp"
#include "game_object.hpp"


namespace VulkanEngine {

	class SimpleRenderSystem {
	public:

		SimpleRenderSystem(EngineDevice& device,VkRenderPass renderpass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameobjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const EngineCamera& camera);


	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		EngineDevice& engineDevice;
	
		std::unique_ptr<EnginePipeline> enginePipline;
		VkPipelineLayout pipelineLayout;
	};
}