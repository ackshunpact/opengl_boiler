#pragma once
#include <engine/scene/Scene.h>
#include <engine/primitives/Triangle.h>

namespace engine {
	class TriangleScene : public engine::Scene {
	public:
		using Scene::Scene;

	protected:
		void onCreate() override {
			m_camera.move({ 0.f, 3.f, 12.f });
			m_sun = { {0.5f, 1.0f, 0.3f}, {1.f, 1.f, 1.f} };
			// m_quadMaterial.albedo = &m_ctx.assetManager.loadTexture("D:/dev/cpp/opengl_boiler/assets/burnt_sand_brown.png", false);
			engine::ModelData data;
			data.meshes.push_back(engine::primitives::triangle());
			m_triangle = std::make_unique <engine::Model>(data);


			auto e = spawn();
			e.add<engine::ecs::TransformComponent>(glm::vec3(0, 0, 0), glm::quat(1.0f, 0, 0, 0), glm::vec3(1.f));
			auto& rc = e.add<engine::ecs::RenderComponent>(*m_triangle);
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
		std::unique_ptr<Model> m_triangle;
		engine::Material m_quadMaterial;
	};
}