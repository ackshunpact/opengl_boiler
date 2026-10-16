#include <engine/gl/VertexBuffer.h>

engine::VertexBuffer::VertexBuffer(const void* data, size_t size, GLenum usage)
{
	glGenBuffers(1, &m_id);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void engine::VertexBuffer::setData(const void* data, size_t size)
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void engine::VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}


