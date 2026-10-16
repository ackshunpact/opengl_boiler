#pragma once
#include <engine/ecs/ComponentManager.h>

namespace engine::ecs {
	class System {
	public:
		System() = default;
		virtual ~System() = default;
		virtual void update(ComponentManager& manager, float dt) = 0;

	};
}