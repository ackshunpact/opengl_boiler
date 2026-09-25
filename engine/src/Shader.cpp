#include <engine/Shader.h>

engine::Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vsFile;
	std::ifstream fsFile;

	vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fsFile.exceptions(std::ifstream::failbit| std::ifstream::badbit);

	try {
		vsFile.open(vertexPath);
		fsFile.open(fragmentPath);
		std::stringstream vsStream, fsStream;

		vsStream << vsFile.rdbuf();
		fsStream << fsFile.rdbuf();

		vsFile.close();
		fsFile.close();
		vertexCode = vsStream.str();
		fragmentCode = fsStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERRORL::SHADER::FILE_NOT_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAIL\n" << infoLog << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAIL\n" << infoLog << std::endl;
	}

	m_id = glCreateProgram();
	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);
	glLinkProgram(m_id);

	glGetProgramiv(m_id, GL_LINK_STATUS, & success);
	if (!success)
	{
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);


}

void engine::Shader::use()
{
	glUseProgram(m_id);
}

void engine::Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void engine::Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void engine::Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}
