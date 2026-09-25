#pragma once

#include <glad/glad.h>
#include <utility>

namespace engine {
	class VertexBuffer {
	public:
		VertexBuffer(const void* data, size_t size);
		~VertexBuffer() { glDeleteBuffers(1, &m_id); }

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&& o) noexcept : m_id(std::exchange(o.m_id, 0)) {}

		void bind() const;
		GLuint id() const { return m_id; }

	private:
		GLuint m_id = 0;
	};
}