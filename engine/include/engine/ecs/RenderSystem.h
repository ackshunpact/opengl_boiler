#pragma once

#include <engine/ecs/ComponentManager.h>
#include <engine/ecs/TransformComponent.h>
#include <engine/ecs/RenderComponent.h>
#include <engine/render/Renderer.h>

namespace engine::ecs {
	struct RenderSystem {
	public:
		void render(ComponentManager& components, Renderer& renderer) {
			components.forEach<TransformComponent, RenderComponent>(
				[&](Entity, TransformComponent& t, RenderComponent& r) {
					if (r.visible && r.model) {
						renderer.submit(*r.model, t.matrix());
					}
				});


		}
	};
}