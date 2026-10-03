#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"

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
		void setIndexBuffer(const IndexBuffer& ibo);
	private:
		GLuint m_id;
		GLuint m_next_index = 0;
	};
}