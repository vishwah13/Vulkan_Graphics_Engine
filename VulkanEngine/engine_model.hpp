#pragma once

#include "engine_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>

namespace VulkanEngine {

	class EngineModel {

	public:

		struct Vertex
		{
			glm::vec2 position;

			static std::vector<VkVertexInputBindingDescription> getBindingDescription();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
		};
		EngineModel(EngineDevice& device, const std::vector<Vertex>& vertices);
		~EngineModel();

		EngineModel(const EngineModel&) = delete;
		EngineModel& operator=(const EngineModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		EngineDevice& engineDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexbufferMemory;
		uint32_t vertexCount;
	};
}