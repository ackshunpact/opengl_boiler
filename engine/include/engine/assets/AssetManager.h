#pragma once
#include <engine/render/Model.h>
#include <engine/assets/ModelLoader.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>
#include <engine/gl/Texture2D.h>
#include <engine/gl/Shader.h>

namespace engine {



	class AssetManager {
		// create a map from string to model
	private:
		std::unordered_map<std::string, unsigned int> m_pathHandleIndex;
		std::vector<std::unique_ptr<engine::Model>> m_handleModelIndex;
		std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_textures;
	
		size_t hashShader(const std::string& vsPath, const std::string& fsPath) const;
	public:
		AssetManager() = default;
		engine::Model& loadGltfModel(const std::string& filepath);
		engine::Model& getModelHandle(const std::string& filepath);

		engine::Shader& loadShader(const std::string& vsPath, const std::string& fsPath);

		const Texture2D& loadTexture(const std::string& filepath, bool srgb = false);

	};
}