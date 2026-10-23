#pragma once

#include <engine/assets/ImageData.h>
#include <string>
#include <iostream>
namespace engine::ImageLoader {
		ImageData loadPng(const std::string& path, bool flipY);
	
}