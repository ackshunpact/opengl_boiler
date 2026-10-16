#pragma once
#include <unordered_map>
#include <vector>
#include <engine/render/Model.h>
#include <engine/render/Camera.h>
#include <engine/gl/Shader.h>


namespace engine {
	struct DirectionalLight {
		glm::vec3 dir{ -0.3f, -1.0f, -0.5f };
		glm::vec3 color{ 1.0f };
	};
	
	class Renderer {
	public:
		explicit Renderer(Shader& shader) : m_shader(shader) {}

		void beginFrame(const Camera& camera, const DirectionalLight& light);
		void submit(Model& model, const glm::mat4& transform);
		void endFrame();
		Shader& getShader() const { return m_shader; }
		size_t lastDrawCalls() const { return m_drawCalls; }
	private:

		struct Batch {
			Model* model;
			std::vector<InstanceData> instances;
		};

		Shader& m_shader;
		glm::mat4 m_view{ 1.f }, m_projection{ 1.f };
		glm::vec3 m_cameraPos{ 0.f };
		DirectionalLight m_light;

		std::vector<Batch> m_batches;
		std::unordered_map<const Model*, size_t> m_batchIndex;
		size_t m_drawCalls = 0;
	};
}