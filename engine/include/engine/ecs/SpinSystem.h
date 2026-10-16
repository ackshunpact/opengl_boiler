#pragma once
#include <engine/ecs/System.h>
#include <engine/ecs/ComponentManager.h>
#include <engine/ecs/TransformComponent.h>

namespace engine::ecs {
	struct SpinComponent : Component {
		glm::vec3 axis{ 0.0f, 1.0f, 0.0f };
		float speed = glm::radians(90.0f);

		SpinComponent() = default;
		SpinComponent(const glm::vec3& a, float s) : axis(glm::normalize(a)), speed(s) {}
		
	};

	class SpinSystem final : public System {
	public:
		void update(ComponentManager& components, float dt) override {
			components.forEach<SpinComponent, TransformComponent>(
				[dt](Entity, SpinComponent& spin, TransformComponent& t) {
					t.rotation = glm::normalize(glm::angleAxis(spin.speed * dt, spin.axis) * t.rotation);
				});
		}
	};

}

