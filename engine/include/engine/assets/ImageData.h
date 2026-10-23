#pragma once
#include <memory>
namespace engine {

	struct StbImageDeleter {
		void operator()(unsigned char* p) const;
	};


	struct ImageData {
		int w = 0, h = 0, nrChannels = 0;
		std::unique_ptr<unsigned char, StbImageDeleter> pixels;


	};
}