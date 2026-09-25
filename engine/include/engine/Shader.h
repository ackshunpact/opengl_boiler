#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace engine {
	class Shader {
	private:
		GLuint m_id;
	public:
		Shader(const char* vertexPath, const char* fragmentPath);
		~Shader() = default;

		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
	};
}