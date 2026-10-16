#pragma once
#include <engine/ecs/Component.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine::ecs {
	struct TransformComponent : Component {

		using Component::Component;
		
		glm::vec3 position{ 0.0f };
		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f };


		TransformComponent() = default;
		explicit TransformComponent(
			const glm::vec3& pos,
			const glm::quat& rot,
			const glm::vec3& scale) :
			position(pos), rotation(rot), scale(scale) {}

		glm::mat4 matrix() const {
			return glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
		}

	};
}