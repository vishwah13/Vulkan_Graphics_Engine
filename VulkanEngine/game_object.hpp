#pragma once

#include "engine_model.hpp"
#include <memory>
#include "glm/gtc/matrix_transform.hpp"

namespace VulkanEngine {

	struct TransformComponent 
	{
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f,1.f};
		glm::vec3  rotation;

		
		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};
	class GameObject {
	public:
		using id_t = unsigned int;

		static GameObject createGameObject() {
			static id_t currentId = 0;
			return GameObject{ currentId++ };
		}

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject &&) = default;
		GameObject& operator=(GameObject&&) = default;

		const id_t getId(){ return id; }

		std::shared_ptr<EngineModel> model{};
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		GameObject(id_t objId) : id(objId) {}
		id_t id;
	};
}