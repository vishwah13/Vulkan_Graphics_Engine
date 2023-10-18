#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace VulkanEngine {

	class VulkanEngineWindow {

	public:
		VulkanEngineWindow(int width, int height, std::string name);
		~VulkanEngineWindow();

		VulkanEngineWindow(const VulkanEngineWindow&) = delete;
		VulkanEngineWindow& operators(const VulkanEngineWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); };
		VkExtent2D getExtend() { return {static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		void initWindow();

		const int width;
		const int height;
		std::string windowName;
		GLFWwindow* window;
	};
}