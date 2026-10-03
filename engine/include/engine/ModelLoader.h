#pragma once
#include <string>
#include <engine/MeshData.h>

// just gltf loader to begin with
namespace engine {
	class ModelLoader {
	public:
		ModelLoader() = default;
		~ModelLoader() = default;

		static ModelData loadGltf(const std::string& path);
	};
}