
#include "engine/assets/AssetManager.h"
#include <stdexcept>
#include <engine/assets/ImageData.h>
#include <engine/assets/ImageLoader.h>

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



	engine::Shader& AssetManager::loadShader(const std::string& vsPath, const std::string& fsPath)
	{
		std::string key = vsPath + '|' + fsPath;
		auto it = m_shaders.find(key);
		if (it == m_shaders.end()) {
			it = m_shaders.emplace(key, std::make_unique<Shader>(vsPath.c_str(), fsPath.c_str())).first;
		}
		return *it->second;
	}

	const Texture2D& AssetManager::loadTexture(const std::string& filepath, bool srgb)
	{
		if (auto it = m_textures.find(filepath); it != m_textures.end()) {
			return *it->second;
		}
		ImageData img = ImageLoader::loadPng(filepath, true);
		if (!img.pixels) {
			throw std::runtime_error("Failed to load texture2D: " + filepath);
		}

		auto [it, inserted] = m_textures.emplace(filepath, std::make_unique<Texture2D>(img.pixels.get(), img.w, img.h, img.nrChannels, srgb));
		return *it->second;
	}

	//std::vector<std::unique_ptr<engine::Model>> AssetManager::getModels() {
	//	return m_handleModelIndex;
	//}
}
