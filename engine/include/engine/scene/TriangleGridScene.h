#pragma once
#include <engine/scene/Scene.h>
#include <engine/primitives/TriangleGrid.h>



namespace engine {
	class TriangleGridScene : public engine::Scene {
	public:
		using Scene::Scene;

	protected:
		void onCreate() override {
			m_camera.move({ 0.f, 3.f, 30.f });
			m_sun = { {5.5f, 10.0f, 5.3f}, {1.f, 1.f, 1.f} };

			//m_quadMaterial.albedo = &m_ctx.assetManager.loadTexture("D:/dev/cpp/opengl_boiler/assets/burnt_sand_brown.png", false);
			m_quadMaterial.uvScale = glm::vec2(0.1f);
			engine::ModelData data;

			engine::primitives::TerrainParams tp{};
			MeshData md = engine::primitives::triangleGrid(201, tp);




			data.meshes.push_back(md);
			m_triangleGrid = std::make_unique <engine::Model>(data);


			auto e = spawn();
			e.add<engine::ecs::TransformComponent>(glm::vec3(0, 0, 0), glm::quat(1.0f, 0, 0, 0), glm::vec3(1.f));
			auto& rc = e.add<engine::ecs::RenderComponent>(*m_triangleGrid);
			rc.material = &m_quadMaterial;
		}

		void onDebugUI() override {

		}

		void onUpdate(float dt) override {
			m_controller.update(m_ctx.input, m_camera, dt);
		}




	private:
		bool m_pausedUI = false;
		engine::FpsCameraController m_controller;
		std::unique_ptr<Model> m_triangleGrid;
		engine::Material m_quadMaterial;

	};
}