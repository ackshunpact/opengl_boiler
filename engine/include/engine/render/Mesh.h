#pragma once
#include <engine/assets/MeshData.h>
#include <engine/gl/VertexArray.h>

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
		
		void attachInstanceBuffer(const VertexBuffer& instanceVbo, const VertexLayout& layout) {
			m_vao.addVertexBuffer(instanceVbo, layout);
		}
		void drawInstanced(GLsizei instanceCount) const {
			m_vao.bind();
			glDrawElementsInstanced(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
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
}
