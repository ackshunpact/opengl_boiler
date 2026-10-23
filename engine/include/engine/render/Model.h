#pragma once
#include <engine/render/Mesh.h>
#include <memory>
#include <vector>
#include <span>

namespace engine {
	class Model {
	public:
		Model() {
			m_instanceVbo = std::make_unique<VertexBuffer>(nullptr, 0, GL_DYNAMIC_DRAW);
			m_meshes = std::vector<std::unique_ptr<Mesh>>();
		}
		explicit Model(const ModelData& data) {
			m_instanceVbo = std::make_unique<VertexBuffer>(nullptr, 0, GL_DYNAMIC_DRAW);
			for (const auto& m : data.meshes) {
				auto mesh = std::make_unique<Mesh>(m);
				mesh->attachInstanceBuffer(*m_instanceVbo, InstanceData::layout());
				m_meshes.push_back(std::move(mesh));

			}
		}
		void draw(std::span<const InstanceData> instances) {
			if (instances.empty()) return;
			m_instanceVbo->setData(instances.data(), instances.size_bytes());
			for (const auto& m : m_meshes) {
				m->drawInstanced(static_cast<GLsizei>(instances.size()));
			}
		}

		void draw(const glm::mat4& transform) {
			InstanceData single{ transform };
			draw(std::span<const InstanceData>(&single, 1));
		}

		size_t meshCount() const {
			return m_meshes.size();
		}
	private:
		std::unique_ptr<VertexBuffer> m_instanceVbo;
		std::vector<std::unique_ptr<Mesh>> m_meshes;
	};
}
