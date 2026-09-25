#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

namespace engine {
	enum class AttributeType { 
		Float, 
		Float2, 
		Float3, 
		Float4, 
		Int, 
		Int2, 
		Int3, 
		Int4 
	};
	
	struct AttributeInfo { 
		GLenum glType; 
		GLint count; 
		GLuint size; 
		bool normalized; 
		bool isInt;
	};

	inline AttributeInfo attributeInfo(AttributeType t) {
		switch (t) {
		case AttributeType::Float: return { GL_FLOAT, 1, 4, false, false };
		case AttributeType::Float2: return { GL_FLOAT, 2, 8, false, false };
		case AttributeType::Float3: return { GL_FLOAT, 3, 12, false, false };
		case AttributeType::Float4: return { GL_FLOAT, 4, 16, false, false };
		case AttributeType::Int: return { GL_FLOAT, 1, 4, false, true };
		case AttributeType::Int2: return { GL_FLOAT, 2, 8, false, true };
		case AttributeType::Int3: return { GL_FLOAT, 3, 12, false, true };
		case AttributeType::Int4: return { GL_FLOAT, 4, 16, false, true };
		default: return {};
		}
	}

	struct VertexAttribute {
		AttributeType type;
		std::string name;
		GLuint offset = 0;
	};

	class VertexLayout {
	private:
		std::vector<VertexAttribute> m_attributes;
		GLsizei m_stride = 0;

	public:
		VertexLayout(std::initializer_list<VertexAttribute> attribs) {
			m_attributes = attribs;
			GLuint offset = 0;
			for (int i = 0; i < m_attributes.size(); i++) {
				m_attributes[i].offset = offset;
				offset += attributeInfo(m_attributes[i].type).size;
			}
			m_stride = offset;
		}

		const std::vector<VertexAttribute>& attributes() const {
			return m_attributes;
		}

		GLsizei stride() const {
			return m_stride;
		}

	};
}	