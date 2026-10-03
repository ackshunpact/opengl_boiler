#pragma once
#include <vector>

#include <glm/glm.hpp>

#include <engine/VertexLayout.h>

namespace engine {
	struct Vertex {
		glm::vec3 position{ 0.f };
		glm::vec3 normal{ 0.f };
		glm::vec2 uv{ 0.f };
		static const VertexLayout& layout() {
			static const VertexLayout s_layout = {
			{engine::AttributeType::Float3, "aPos"},
			{engine::AttributeType::Float3, "aNormal"},
			{engine::AttributeType::Float2, "aUV"},

			};
			return s_layout;
		}
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		int materialIndex = -1;
	};

	struct ModelData {
		std::vector<MeshData> meshes;
	};
}