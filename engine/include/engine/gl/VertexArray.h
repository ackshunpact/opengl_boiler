#pragma once

#include <glad/glad.h>
#include <engine/gl/VertexBuffer.h>
#include <engine/gl/VertexLayout.h>
#include <engine/gl/IndexBuffer.h>

namespace engine {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		void bind() const;
		void unbind();
		void addVertexBuffer(const VertexBuffer& vbo, const VertexLayout& layout);
		void addInstancedVertexBuffer(const VertexBuffer& instanceVbo, const VertexLayout& layout);
		void setIndexBuffer(const IndexBuffer& ibo);
	private:
		GLuint m_id;
		GLuint m_next_index = 0;
	};
}