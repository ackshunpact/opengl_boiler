#pragma once
#include <glad/glad.h>


namespace engine {

	class Texture2D {
	private:
		GLuint m_id = 0;
		int m_width = 0, m_height = 0;
		GLenum m_internalFormat = 0;
	public:

		Texture2D(const unsigned char* pixels, int width, int height, int nrChannels, bool srgb);
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		~Texture2D();
		void bind(unsigned unit) const;

	};
}