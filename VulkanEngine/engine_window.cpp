#include "engine_window.hpp"
#include <stdexcept>

namespace VulkanEngine {
	VulkanEngineWindow::VulkanEngineWindow(int width, int height, std::string name) : width(width), height(height), windowName(name) {

		initWindow();
	
	}
	VulkanEngineWindow::~VulkanEngineWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void VulkanEngineWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("FAILED TO CREATE SURFACE !!!");
		}
	}
	void VulkanEngineWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto engineWindow = reinterpret_cast<VulkanEngineWindow*>(glfwGetWindowUserPointer(window));
		engineWindow->frameBufferResized = true;
		engineWindow->width = width;
		engineWindow->height = height;
	}
	void VulkanEngineWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
	}
}