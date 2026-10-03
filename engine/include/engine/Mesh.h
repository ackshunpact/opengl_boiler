#pragma once
#include <engine/MeshData.h>
#include <engine/VertexArray.h>
#include <memory>

namespace engine {
	class Mesh {
	public:
		explicit Mesh(const MeshData& data) 
			: m_vao(), m_ibo(data.indices.data(), data.indices.size() * sizeof(uint32_t))
			, m_vbo(data.vertices.data(), data.vertices.size() * sizeof(engine::Vertex))
			, m_indexCount(static_cast<GLsizei>(data.indices.size()))
			,
			m_materialIndex(data.materialIndex)
		{
			m_vao.setIndexBuffer(m_ibo);
			m_vao.addVertexBuffer(m_vbo, Vertex::layout());
		}
			

		
		void draw() const {
			m_vao.bind();
			glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
		}
	private:
		VertexArray m_vao;
		IndexBuffer m_ibo;
		VertexBuffer m_vbo;
		GLsizei m_indexCount;
		int m_materialIndex;
	};

	class Model {
	public:
		explicit Model(const ModelData& data) {
			for (const auto& m : data.meshes) {
				m_meshes.push_back(std::make_unique<Mesh>(m));
			}
		}
		void draw() const {
			for (const auto& m : m_meshes) {
				m->draw();
			}
		}
	private:
		std::vector<std::unique_ptr<Mesh>> m_meshes;
	};

}