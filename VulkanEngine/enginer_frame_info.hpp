#pragma once
#include <vulkan/vulkan.h>
#include "engine_Camera.hpp"

namespace VulkanEngine {
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandbuffer;
		EngineCamera& camera;
	};
}