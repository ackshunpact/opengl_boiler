#pragma once
#include <engine/assets/MeshData.h>
#include <engine/scene/Scene.h>
#include <engine/assets/ImageData.h>
#include <engine/assets/ImageLoader.h>
#include <algorithm>
#include <engine/primitives/TriangleGrid.h>
#include <engine/debug/ImGuiLayer.h>

namespace engine {
	class HeightMapScene : public engine::Scene {

	public:
		using Scene::Scene;

	protected:
		void onCreate() override {
			m_camera.move({ 0.f, 3.f, 30.f });
			m_sun = { {5.5f, 10.0f, 5.3f}, {1.f, 1.f, 1.f} };
			auto& assets = m_ctx.assetManager;
			const char* vsPath = "D:/dev/cpp/opengl_boiler/app/default.vert";
			const char* fsPath = "D:/dev/cpp/opengl_boiler/app/terrain.frag";


			m_heightMapImageData = ImageLoader::loadPng("D:/dev/cpp/opengl_boiler/assets/heightmap.png", false);
			m_terrainMat.shader = &assets.loadShader(vsPath, fsPath);
			m_terrainMat.textures = {
				{"uGrass", &assets.loadTexture("D:/dev/cpp/opengl_boiler/assets/Grass001_1KJPG/Grass001_1K-JPG_Color.jpg")},
				{"uGround", &assets.loadTexture("D:/dev/cpp/opengl_boiler/assets/Ground085_1KJPG/Ground085_1KJPG_Color.jpg")},
				{"uSnow", &assets.loadTexture("D:/dev/cpp/opengl_boiler/assets/Snow014_1KJPG/Snow014_1K-JPG_Color.jpg")}
			};
			m_terrainMat.uvScale = glm::vec2(0.1f);
			m_terrainMat.floats = { {"uSnowHeight", 15.0f}, {"uBlend", 2.0f} };

			m_tp = {
				 0.006f, 0.45f, 0.65f, 2.0f, 60.0f, 2.0f, 5, 4.0f
			};

			engine::ModelData data;
			MeshData md = engine::primitives::triangleGrid(201, m_tp);
			//MeshData md = createMapMesh();
			



			data.meshes.push_back(md);
			m_heightMapTerrain = std::make_unique <engine::Model>(data);


			m_terrain = spawn();
			m_terrain.add<engine::ecs::TransformComponent>(glm::vec3(0, 0, 0), glm::quat(1.0f, 0, 0, 0), glm::vec3(1.f));
			auto& rc = m_terrain.add<engine::ecs::RenderComponent>(*m_heightMapTerrain);
			rc.material = &m_terrainMat;
		}

		void onDebugUI() override {
			ImGui::Begin("Terrain");
			bool changed = false;
			changed |= ImGui::SliderFloat("Mask Frequency", &m_tp.maskFreq, 0.001f, 0.02f, "%.4f");
			changed |= ImGui::DragFloatRange2("Mask Range", &m_tp.maskLow, &m_tp.maskHigh, 0.01f, 0.0f, 1.0f);
			changed |= ImGui::SliderFloat("Plains Height", &m_tp.plainsHeight, 0.0f, 10.0f);
			changed |= ImGui::SliderFloat("Mountain Height", &m_tp.mountainHeight, 0.0f, 150.0f);
			changed |= ImGui::SliderFloat("Peak Exponent", &m_tp.peakExp, 1.0f, 4.0f);
			changed |= ImGui::SliderInt("Ridged Octaves", &m_tp.ridgedOctaves, 1, 8);
			changed |= ImGui::DragFloatRange2("Detail (low/high)", &m_tp.detailLow, &m_tp.detailHigh, 0.05f, 0.0f, 10.0f);

			ImGui::Separator();
			for (auto& [name, value] : m_terrainMat.floats) {
				ImGui::DragFloat(name.c_str(), &value, 0.1f);
			}
			ImGui::End();

			if (changed) m_terrainDirty = true;
		}

		void onUpdate(float dt) override {
			m_controller.update(m_ctx.input, m_camera, dt);
			if (m_terrainDirty) {
				m_terrainDirty = false;
				engine::ModelData data;
				data.meshes.push_back(engine::primitives::triangleGrid(201, m_tp));
				m_heightMapTerrain = std::make_unique<engine::Model>(data);
				m_terrain.get<engine::ecs::RenderComponent>().model = m_heightMapTerrain.get();
			}
		}

		



	private:
		bool m_pausedUI = false;
		engine::FpsCameraController m_controller;
		std::unique_ptr<Model> m_heightMapTerrain;
		engine::Material m_quadMaterial;
		ImageData m_heightMapImageData;
		Material m_terrainMat;
		engine::primitives::TerrainParams m_tp;
		bool m_terrainDirty = false;
		engine::ecs::Entity m_terrain;

		MeshData createMapMesh() {
			MeshData md;
			int mapH = m_heightMapImageData.h;
			int mapW = m_heightMapImageData.w;
			int ch = m_heightMapImageData.nrChannels;
			const float maxH = 20.0f;

			const unsigned char* pixels = m_heightMapImageData.pixels.get();
			

			for (int j = 0; j < mapH; ++j) {
				for (int i = 0; i < mapW; ++i) {
					float h = pixels[(j * mapW + i) * ch] / 255.0f * maxH;
					md.vertices.push_back(Vertex{ glm::vec3{i,h, j}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2{float(i), float(j) } });
				}
			}

			for (int j = 0; j < mapH - 1; ++j) {
				for (int i = 0; i < mapW - 1; ++i) {
					uint32_t tl = j * mapW + i;
					uint32_t tr = tl + 1;
					uint32_t bl = tl + mapW;
					uint32_t br = bl + 1;

					md.indices.push_back(tl);
					md.indices.push_back(bl);
					md.indices.push_back(tr);

					md.indices.push_back(tr);
					md.indices.push_back(bl);
					md.indices.push_back(br);
				}
			}
			auto heightAt = [&](int i, int j) {
				i = std::clamp(i, 0, mapW - 1);
				j = std::clamp(j, 0, mapH - 1);
				return md.vertices[j * mapW + i].position.y;
				};

			const float space = 1.0f;
			for (int j = 0; j < mapH; ++j) {
				for (int i = 0; i < mapW; ++i) {
					float hxm = heightAt(i - 1, j), hxp = heightAt(i + 1, j);
					float hzm = heightAt(i, j - 1), hzp = heightAt(i, j + 1);
					/*			float hxm = height(i - 1, j), hxp = height(i + 1, j);
								float hzm = height(i, j - 1), hzp = height(i, j + 1);*/
					glm::vec3 n(hxm - hxp, 2.0f * space, hzm - hzp);
					md.vertices[j * mapW + i].normal = glm::normalize(n);
				}
			}
			return md;
		
	};

	};
}