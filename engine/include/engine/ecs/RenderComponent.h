#pragma once
#include <engine/ecs/Component.h>
#include <glm/glm.hpp>

// forward 
namespace engine {
	class Model;
	class Material;
}

namespace engine::ecs {

	struct RenderComponent : Component {
		Model* model = nullptr; // non-ownership
		const Material* material = nullptr;
		bool visible = true;
		glm::vec3 baseColor{ 1.0f };
		RenderComponent() = default;
		explicit RenderComponent(Model& m) : model(&m) { assert(model && "RenderComponent given a null model"); };
	};



}