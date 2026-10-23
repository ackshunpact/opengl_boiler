#include <engine/gl/Texture2D.h>

engine::Texture2D::Texture2D(const unsigned char* pixels, int width, int height, int nrChannels, bool srgb)
	: m_width(width), m_height(height) {
	GLenum format = GL_RGBA;
	m_internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
	if (nrChannels == 3) { format = GL_RGB; m_internalFormat = srgb ? GL_SRGB8 : GL_RGB8; }
	if (nrChannels == 1) { format = GL_RED; m_internalFormat = GL_R8; }

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

}

engine::Texture2D::~Texture2D() {
	if (m_id) {
		glDeleteTextures(1, &m_id);
	}
}

void engine::Texture2D::bind(unsigned unit) const
{
	glBindTextureUnit(unit, m_id);
}

