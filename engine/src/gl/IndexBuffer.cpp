#include <engine/gl/IndexBuffer.h>

engine::IndexBuffer::IndexBuffer(const void* indices, size_t size)
{
	glGenBuffers(1, &m_id);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	m_count = size / sizeof(GLuint);
}

engine::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void engine::IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

GLuint engine::IndexBuffer::count()
{
	return m_count;
}
