#pragma once
#include <engine/render/Model.h>
#include <engine/assets/ModelLoader.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>


namespace engine {
	class AssetManager {
		// create a map from string to model
	private:
		std::unordered_map<std::string, unsigned int> m_pathHandleIndex;
		std::vector<std::unique_ptr<engine::Model>> m_handleModelIndex;
	
	public:
		AssetManager() = default;
		engine::Model& loadGltfModel(const std::string& filepath);
		engine::Model& getModelHandle(const std::string& filepath);
		//std::vector<std::unique_ptr<engine::Model>> getModels();

	};
}