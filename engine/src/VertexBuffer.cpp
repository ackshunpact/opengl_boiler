#include "engine/VertexBuffer.h"

engine::VertexBuffer::VertexBuffer(const void* data, size_t size)
{
	glGenBuffers(1, &m_id);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void engine::VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}


