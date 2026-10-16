#pragma once
#include <string>
#include <engine/assets/MeshData.h>

// just gltf loader to begin with
namespace engine {
	class ModelLoader {
	public:
		ModelLoader() = default;
		~ModelLoader() = default;

		static ModelData loadGltf(const std::string& path);
	};
}