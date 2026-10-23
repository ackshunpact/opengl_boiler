#pragma once 
#include <engine/gl/Texture2D.h>
#include <engine/gl/Shader.h>
#include <glm/glm.hpp>
#include <vector>

namespace engine {
	struct TextureSlot {
		std::string uniform; // sampler name
		const Texture2D* texture;
	};

	struct Material {
		const Shader* shader = nullptr;		// nullptr -> default shader
		std::vector<TextureSlot> textures;	// index here is the texture unit
		glm::vec4 baseColor{ 1.0f };
		glm::vec2 uvScale{ 1.0f };
		std::vector<std::pair<std::string, float>> floats;	// shader-specifics
	};
}