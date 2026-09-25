#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"
#include "VertexLayout.h"

namespace engine {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray() = default;

		void bind() const;
		void unbind();
		void addVertexBuffer(const VertexBuffer& vbo, const VertexLayout& layout);
	private:
		GLuint m_id;
		GLuint m_next_index = 0;
	};
}