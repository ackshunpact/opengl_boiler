#pragma once
#include <vector>

#include <glm/glm.hpp>

#include <engine/gl/VertexLayout.h>

namespace engine {
	struct Vertex {
		glm::vec3 position{ 0.f };
		glm::vec3 normal{ 0.f };
		glm::vec2 uv{ 0.f };
		static const VertexLayout& layout() {
			static const VertexLayout vl = {
			{engine::AttributeType::Float3, "aPos"},
			{engine::AttributeType::Float3, "aNormal"},
			{engine::AttributeType::Float2, "aUV"},

			};
			return vl;
		}
	};

	struct InstanceData {
		glm::mat4 model{ 1.f };
		glm::vec4 color{ 1.0f };
		static const VertexLayout& layout() {
			static const VertexLayout vl = {
				{{AttributeType::Mat4, "aModel" }, // location 3-6 (mat4 is 4 vec4)
				{AttributeType::Float4, "aColor"}}, // location 7 single vec4
				InputRate::PerInstance
			};
			return vl;
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