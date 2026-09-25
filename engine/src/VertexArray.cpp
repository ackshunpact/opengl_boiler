#include "engine/VertexArray.h"

engine::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

void engine::VertexArray::bind() const {
	glBindVertexArray(m_id);
}

void engine::VertexArray::unbind() {
	glBindVertexArray(0);
}

void engine::VertexArray::addVertexBuffer(const VertexBuffer& vbo, const VertexLayout& layout)
{
	bind();
	vbo.bind();

	for (auto& a : layout.attributes()) {
		AttributeInfo attribInfo = attributeInfo(a.type);
		glEnableVertexAttribArray(m_next_index);

		if (attribInfo.isInt) {
			glVertexAttribIPointer(m_next_index, attribInfo.count, attribInfo.glType, layout.stride(), (void*)(a.offset));
		}
		else {
			glVertexAttribPointer(m_next_index, attribInfo.count, attribInfo.glType, attribInfo.normalized ? GL_TRUE : GL_FALSE, layout.stride(), (void*)(a.offset));
		}
		m_next_index++;
	}
}

