#include "app.hpp"

namespace VulkanEngine {

	void App::run()
	{
		while (!veWindow.shouldClose())
		{
			glfwPollEvents();
		}
	}
}


