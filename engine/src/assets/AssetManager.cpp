
#include "engine/assets/AssetManager.h"
#include <stdexcept>

namespace engine {
	
	engine::Model& AssetManager::loadGltfModel(const std::string& filepath) {
		ModelData md = ModelLoader::loadGltf(filepath) ;
		std::unique_ptr<engine::Model> m = std::make_unique<engine::Model>(md);
		if (auto it = m_pathHandleIndex.find(filepath); it != m_pathHandleIndex.end())
			return *m_handleModelIndex[it->second];
		m_handleModelIndex.push_back(std::move(m));
		m_pathHandleIndex.insert({ filepath, m_handleModelIndex.size() - 1 });
		return *m_handleModelIndex.back();
	}

	engine::Model& AssetManager::getModelHandle(const std::string& filepath) {
		auto it = m_pathHandleIndex.find(filepath);
		if (it == m_pathHandleIndex.end()) {
			throw std::runtime_error("Model not loaded: " + filepath);
		}
		return *m_handleModelIndex[it->second];

	}

	//std::vector<std::unique_ptr<engine::Model>> AssetManager::getModels() {
	//	return m_handleModelIndex;
	//}
}
