#include <engine/gl/VertexArray.h>

engine::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

engine::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
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
	GLuint div = layout.getRate() == InputRate::PerInstance ? 1 : 0;

	for (auto& a : layout.attributes()) {
		AttributeInfo attribInfo = attributeInfo(a.type);
		const GLuint slotSize = attribInfo.size / attribInfo.slots;

		for (GLuint slot = 0; slot < attribInfo.slots; slot++) {
			int offset = a.offset + slot * slotSize;
		
			glEnableVertexAttribArray(m_next_index);

			if (attribInfo.isInt) {
				glVertexAttribIPointer(m_next_index, attribInfo.count, attribInfo.glType, layout.stride(), (void*)(offset));
			}
			else {
				glVertexAttribPointer(m_next_index, attribInfo.count, attribInfo.glType, attribInfo.normalized ? GL_TRUE : GL_FALSE, layout.stride(), (void*)(offset));
			}
			glVertexAttribDivisor(m_next_index, div);
			m_next_index++;
		}
	}
	unbind();
}

void engine::VertexArray::setIndexBuffer(const IndexBuffer& ibo)
{
	bind();
	ibo.bind();
	unbind();
}

