#pragma once
#include <glm/glm.hpp>
#include <engine/ecs/Component.h>
#include <engine/ecs/TransformComponent.h>
#include <engine/ecs/ComponentManager.h>
#include <engine/ecs/System.h>

namespace engine::ecs {
	struct PhysicsComponent : Component {
		float mass = 1.0f;
		glm::vec3 velocity{ 0.f };
		glm::vec3 acceleration{ 0.0f };

		PhysicsComponent() = default;
		PhysicsComponent(float m, glm::vec3 v, glm::vec3 a)
			: mass(m), velocity(v), acceleration(a) {
		};

		~PhysicsComponent() = default;
	};

	class PhysicsSystem : public System {
	public:
		void update(ComponentManager& components, float dt) override {
			components.forEach<PhysicsComponent, TransformComponent>(
				[dt](Entity, PhysicsComponent& p, TransformComponent& t) {
					//t.rotation = glm::normalize(glm::angleAxis(spin.speed * dt, spin.axis) * t.rotation);
					// update position
					float dTime = 1.f * dt;
					t.position = t.position + p.velocity * dTime + p.acceleration * (0.5f * dTime * dTime);
					p.velocity = p.velocity + p.acceleration * dTime;

				});

		};
	};
};