#pragma once

#include <engine/ecs/ComponentManager.h>
#include <engine/ecs/System.h>
#include <engine/ecs/RenderSystem.h>
#include <engine/render/Renderer.h>
#include <engine/render/Camera.h>

#include <memory>
#include <vector>

namespace engine {
	class Input;
	class AssetManager;
	
	struct SceneContext {
		Input& input;
		AssetManager& assetManager;
	};


	class Scene {
	public:
		explicit Scene(SceneContext ctx) : m_ctx(ctx) {}
		virtual ~Scene() = default;

		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		virtual void onCreate() {};
		virtual void onDestroy() {};
		virtual void onDebugUI() {};
		void update(float dt) {
			if (m_paused) return;
			m_time += dt;
			onUpdate(dt);
			for (auto& system : m_systems) {
				system->update(m_world, dt);
				flushDestroyed();
			}
		}

		void render(Renderer& renderer) {
			renderer.beginFrame(m_camera, m_sun);
			m_renderSystem.render(m_world, renderer);
			renderer.endFrame();
		}


		ecs::Entity spawn() {
			return m_world.createEntity();
		}

		void destroyLater(ecs::Entity e) {
			m_pendingDestroy.push_back(e.id());
		}

		ecs::ComponentManager& world() {
			return m_world;
		}

		Camera& camera() {
			return m_camera;
		}

		float time() const {
			return m_time;
		}

		void setPaused(bool p) {
			m_paused = p;
		}


	protected:
		virtual void onUpdate(float) {};
		template <typename T, typename... Args>
		T& addSystem(Args&&... args) {
			auto sys = std::make_unique<T>(std::forward<Args>(args)...);
			T& ref = *sys;
			m_systems.push_back(std::move(sys));
			return ref;
		}

		SceneContext m_ctx;
		Camera m_camera;
		DirectionalLight m_sun;

	private:

		void flushDestroyed() {
			for (auto id : m_pendingDestroy) {
				m_world.destroyEntity(id);
			}
			m_pendingDestroy.clear();
		}
		ecs::ComponentManager m_world;
		std::vector<std::unique_ptr<ecs::System>> m_systems;
		ecs::RenderSystem m_renderSystem;
		std::vector<ecs::EntityId> m_pendingDestroy;
		bool m_paused = false;
		float m_time = 0.f;



















	};
}