#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <engine/assets/MeshData.h>

namespace engine::primitives {


	inline MeshData triangle() {
		const glm::vec3 up(0.0f, 1.0f, 0.0f);
		MeshData md;
		md.vertices = {
			Vertex(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
			Vertex(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
			Vertex(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
			Vertex(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f))
		};
		md.indices = { 
			0, 1, 2 , 
			0, 2, 3	
		};
		return md;
	}
}
