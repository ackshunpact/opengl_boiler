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
		Int4,
		Mat3,
		Mat4
	};
	
	struct AttributeInfo { 
		GLenum glType; 
		GLint count; 
		GLuint size; 
		bool normalized; 
		bool isInt;
		GLuint slots = 1;
	};

	inline AttributeInfo attributeInfo(AttributeType t) {
		switch (t) {
		case AttributeType::Float: return { GL_FLOAT, 1, 4, false, false, 1 };
		case AttributeType::Float2: return { GL_FLOAT, 2, 8, false, false, 1 };
		case AttributeType::Float3: return { GL_FLOAT, 3, 12, false, false, 1 };
		case AttributeType::Float4: return { GL_FLOAT, 4, 16, false, false, 1 };
		case AttributeType::Int: return { GL_INT, 1, 4, false, true, 1 };
		case AttributeType::Int2: return { GL_INT, 2, 8, false, true, 1 };
		case AttributeType::Int3: return { GL_INT, 3, 12, false, true, 1 };
		case AttributeType::Int4: return { GL_INT, 4, 16, false, true, 1 };
		case AttributeType::Mat3: return { GL_FLOAT, 3, 36, false, false, 3 };
		case AttributeType::Mat4: return { GL_FLOAT, 4, 64, false, false, 4 };
		default: return {};
		}
	}

	enum class InputRate { 
		PerVertex, 
		PerInstance };

	struct VertexAttribute {
		AttributeType type;
		std::string name;
		GLuint offset = 0;
	};


	class VertexLayout {
	private:
		std::vector<VertexAttribute> m_attributes;
		GLsizei m_stride = 0;
		InputRate m_rate;
	public:
		VertexLayout(std::initializer_list<VertexAttribute> attribs, InputRate rate = InputRate::PerVertex) {
			m_attributes = attribs;
			m_rate = rate;
			GLuint offset = 0;
			for (int i = 0; i < m_attributes.size(); i++) {
				m_attributes[i].offset = offset;
				offset += attributeInfo(m_attributes[i].type).size;
			}
			m_stride = offset;
		}

		InputRate getRate() const { return m_rate; }

		const std::vector<VertexAttribute>& attributes() const {
			return m_attributes;
		}

		GLsizei stride() const {
			return m_stride;
		}

	};
}	