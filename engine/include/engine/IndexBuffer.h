#pragma once
#include <glad/glad.h>

namespace engine {
	class IndexBuffer {
	public:
		IndexBuffer(const void* indices, size_t size);
		~IndexBuffer();
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		void bind() const;
		GLuint count();
	private:
		GLuint m_id;
		GLuint m_count;
	};
}