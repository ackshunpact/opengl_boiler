#pragma once
#include <unordered_map>
#include <vector>
#include <engine/render/Model.h>
#include <engine/render/Material.h>
#include <engine/render/Camera.h>
#include <engine/gl/Shader.h>


namespace engine {
	struct DirectionalLight {
		glm::vec3 dir{ -0.3f, -1.0f, -0.5f };
		glm::vec3 color{ 1.0f };
	};
	
	class Renderer {
	public:
		explicit Renderer(Shader& shader) : m_defaultShader(shader), m_whiteTexture(kWhite, 1, 1, 4, false) {}

		void beginFrame(const Camera& camera, const DirectionalLight& light);
		void submit(Model& model, const Material* material, const glm::mat4& transform, const glm::vec4& color);
		void endFrame();
		void applyMaterial(const Shader& s, const Material& mat);
		void setFrameUniforms(const Shader& s) const;
		
		const Shader* shaderFor(const Material& m) const;
		size_t lastDrawCalls() const { return m_drawCalls; }

		
		size_t lastInstanceCount();
		size_t lastBatchCount();
		static constexpr unsigned char kWhite[4] = { 255, 255, 255, 255 };


	private:

		struct Batch {
			Model* model;
			const Material* material;
			std::vector<InstanceData> instances;
		};

		struct BatchKey {
			const Model* model;
			const Material* material;
			bool operator==(const BatchKey&) const = default;
		};

		struct BatchKeyHash {
			size_t operator()(const BatchKey& k) const noexcept {
				size_t h1 = std::hash<const void*>{}(k.model);
				size_t h2 = std::hash<const void*>{}(k.material);
				return h1 ^ (h2 + 0x9e3779b97f4a7c15ull + (h1 << 6) + (h1 >> 2));
			}
		};

		Shader& m_defaultShader;
		glm::mat4 m_view{ 1.f }, m_projection{ 1.f };
		glm::vec3 m_cameraPos{ 0.f };
		DirectionalLight m_light;

		std::vector<Batch> m_batches;
		std::vector<size_t> m_order;	// hold batch indices for sorting
		std::unordered_map<BatchKey, size_t, BatchKeyHash> m_batchIndex;


		Texture2D m_whiteTexture;
		Material m_defaultMaterial;
		size_t m_drawCalls = 0;
		size_t m_instanceCount = 0;
		size_t m_batchCount = 0;
	};
}