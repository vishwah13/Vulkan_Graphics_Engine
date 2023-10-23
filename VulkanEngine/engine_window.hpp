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
		bool wasWindowResized() {return frameBufferResized;}
		void resetWindowResizedFlag() { frameBufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);
		void initWindow();

		int width;
		int height;
		bool frameBufferResized = false;
		std::string windowName;
		GLFWwindow* window;
	};
}