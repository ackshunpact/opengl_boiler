#pragma once

#include <engine/ecs/System.h>
#include <engine/scene/Scene.h>
#include <engine/render/Model.h>
#include <engine/ecs/TransformComponent.h>
#include <engine/ecs/RenderComponent.h>
#include <engine/ecs/SpinSystem.h>
#include <engine/scene/FpsCameraController.h>
#include <cmath>
#include <glm/glm.hpp>
#include <imgui.h>

class CubeWaveScene : public engine::Scene {
public:
	using Scene::Scene;

protected:
	void onCreate() override {
		m_camera.move({ 0.f, 3.f, 12.f });
		m_sun = { {0.5f, 1.0f, 0.3f}, {1.f, 1.f, 1.f} };

		engine::Model& cube = m_ctx.assetManager.loadGltfModel(cubeModelPath);

		for (int j = -kGrid; j < kGrid; ++j) 
			for (int i = -kGrid; i < kGrid; ++i) {
				auto e = spawn();
				e.add<engine::ecs::TransformComponent>(glm::vec3(i , 0, j ), glm::quat(1.0f, 0, 0, 0), glm::vec3(1.f));
				e.add<engine::ecs::RenderComponent>(cube);

			}
		addSystem<engine::ecs::SpinSystem>();
	}

	void onDebugUI() override {
		ImGui::Begin("Scene");
		ImGui::DragFloat3("Sun direction", &m_sun.dir.x, 0.01f);
		ImGui::ColorEdit3("Sun color", &m_sun.color.x);
		ImGui::Checkbox("Paused", &m_pausedUI);
		ImGui::End();
	
	
	}


	void onUpdate(float dt) override {
		if (!m_pausedUI) {
			m_controller.update(m_ctx.input, m_camera, dt);
			const float t = time();
		
			world().forEach<engine::ecs::TransformComponent>(
				[t](engine::ecs::Entity, engine::ecs::TransformComponent& tr) {
					tr.position.y = 0.25f * (std::cos(t * tr.position.x) + std::sin(t * tr.position.z)) + 0.5;
				});

		}
	}


private:
	bool m_pausedUI = false;
	engine::FpsCameraController m_controller;
	int kGrid = 10;
	const char* cubeModelPath = "D:/dev/cpp/opengl_boiler/assets/cube2.glb";

};