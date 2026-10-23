#pragma once

#include <assets/ImageLoader.h>
#include <glad/glad.h>
#include <unordered_map>

namespace engine {
	struct TextureOptions {
		GLuint glTexWrapS = GL_REPEAT;
		GLuint glTexWrapT = GL_REPEAT;
		GLuint glTexMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		GLuint glTexMagFilter = GL_LINEAR;
	};

	class TextureManager {
	
	public:
		Texture() = default;
		~Texture() = default;

		

	private:
		GLuint m_id;
		std::unordered_map<std::string, GLuint> m_textureIndex;


	};
}