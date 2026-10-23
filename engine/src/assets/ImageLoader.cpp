#include <engine/assets/ImageLoader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace engine {

	ImageData ImageLoader::loadPng(const std::string& path, bool flipY) {
		stbi_set_flip_vertically_on_load(flipY);
		ImageData img;
		unsigned char* data = stbi_load(path.c_str(), &img.w, &img.h, &img.nrChannels, 0);
		if (!data) {
			std::cerr << "Failed to load texture: " << path << std::endl;
		}

		img.pixels.reset(data);
		return img;
	}

	void StbImageDeleter::operator()(unsigned char* p) const {
		stbi_image_free(p);
	}

}