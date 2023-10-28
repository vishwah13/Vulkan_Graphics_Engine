#include "engine_model.hpp"
#include <cassert>

namespace VulkanEngine {

	EngineModel::EngineModel(EngineDevice& device, const EngineModel::Builder& builder) : engineDevice(device)
	{
		createVertexBuffers(builder.vertices);
		createIndexBuffer(builder.indices);
	}

	EngineModel::~EngineModel()
	{
		vkDestroyBuffer(engineDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(engineDevice.device(), vertexbufferMemory, nullptr);

		if (hasIndexbuffer) {
			vkDestroyBuffer(engineDevice.device(), indexBuffer, nullptr);
			vkFreeMemory(engineDevice.device(), indexbufferMemory, nullptr);
		}
	}

	void EngineModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexbuffer) {
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void EngineModel::draw(VkCommandBuffer commandBuffer)
	{
		if (hasIndexbuffer) {
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		}
		else {
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
		}
		
	}

	void EngineModel::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		engineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory);

		void* data;
		vkMapMemory(engineDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(engineDevice.device(), stagingBufferMemory);

		engineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer,
			vertexbufferMemory);

		engineDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

		vkDestroyBuffer(engineDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(engineDevice.device(), stagingBufferMemory, nullptr);
	}

	void EngineModel::createIndexBuffer(const std::vector<uint32_t>& indices)
	{
		indexCount = static_cast<uint32_t>(indices.size());
		hasIndexbuffer = indexCount > 0;

		if (!hasIndexbuffer) {
			return;
		}

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		engineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory);

		void* data;
		vkMapMemory(engineDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(engineDevice.device(), stagingBufferMemory);

		engineDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer,
			indexbufferMemory);

		engineDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

		vkDestroyBuffer(engineDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(engineDevice.device(), stagingBufferMemory, nullptr);
	}

	std::vector<VkVertexInputBindingDescription> EngineModel::Vertex::getBindingDescription()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescription(1);
		bindingDescription[0].binding = 0;
		bindingDescription[0].stride = sizeof(Vertex);
		bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	std::vector<VkVertexInputAttributeDescription> EngineModel::Vertex::getAttributeDescription()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescription(2);
		attributeDescription[0].binding = 0;
		attributeDescription[0].location = 0;
		attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[0].offset = offsetof(Vertex, position);

		attributeDescription[1].binding = 0;
		attributeDescription[1].location = 1;
		attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[1].offset = offsetof(Vertex,color);
		return attributeDescription;
	}

}

