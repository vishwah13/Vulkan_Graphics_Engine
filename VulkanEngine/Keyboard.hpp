#pragma once

#include "game_object.hpp"
#include "engine_window.hpp"

namespace VulkanEngine {

	class Keyboard {
	public:
		struct KeyMapping
		{
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
		};

        void moveInPlaneXZ(GLFWwindow* window, float dt, GameObject& gameobject);

        KeyMapping keys{};
        float moveSpeed{ 3.f };
        float lookSpeed{ 1.5f };
	};
}