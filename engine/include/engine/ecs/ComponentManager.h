#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <stdexcept>
#include <string>

#include <engine/ecs/ComponentArray.h>
#include <engine/ecs/Entity.h>

namespace engine::ecs {

	class ComponentManager {
	public:
		Entity createEntity() {
			EntityId id;
			if (!m_freeIds.empty()) {
				id = m_freeIds.back();
				m_freeIds.pop_back();
			}
			else {
				id = m_nextId++;
			}
			if (id >= m_alive.size()) {
				m_alive.resize(static_cast<std::size_t>(id) + 1, false);
			}
	
			m_alive[id] = true;
			return Entity(id, this);
		}

		void destroyEntity(EntityId id) {
			if (!isAlive(id)) return;
			for (auto& [type, array] : m_arrays) {
				array->remove(id);
			}

			m_alive[id] = false;
			m_freeIds.push_back(id);
		}

		bool isAlive(EntityId id) const {
			return id < m_alive.size() && m_alive[id];
		}

		template <ComponentType T, typename... Args>
		T& add(EntityId e, Args&&... args) {
			if (!isAlive(e)) {
				throw std::runtime_error("Entity not alive");
			}
			return array<T>().emplace(e, std::forward<Args>(args)...);
		}

		template <ComponentType T>
		T& get(EntityId e) {
			return array<T>().get(e);
		}

		template <ComponentType T>
		bool has(EntityId e) const {
			auto it = m_arrays.find(std::type_index(typeid(T)));
			return it != m_arrays.end() && it->second->has(e);
		}

		template <ComponentType T>
		void remove(EntityId e) {
			array<T>().remove(e);
		}

		template <ComponentType First, ComponentType... Rest, typename Func>
		void forEach(Func&& f) {
			auto& first = array<First>();
			auto ids = first.entities();
			for (std::size_t i = 0; i < ids.size(); i++) {
				const EntityId e = ids[i];
				if ((has<Rest>(e) && ...)) {
					f(engine::ecs::Entity(e, this), first.at(i), get<Rest>(e)...);
				}
			}
		}



	private:
		template <ComponentType T>
		ComponentArray<T>& array() {
			auto& slot = m_arrays[std::type_index(typeid(T))];
			if (!slot) {
				slot = std::make_unique<ComponentArray<T>>();
			}
			return static_cast<ComponentArray<T>&>(*slot);
		}


		std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> m_arrays;
		std::vector<bool> m_alive;
		std::vector<EntityId> m_freeIds;
		EntityId m_nextId = 0;
	};
	inline bool Entity::valid() const {
		return m_manager && m_manager->isAlive(m_id);
	}
	inline void Entity::destroy() {
		m_manager->destroyEntity(m_id);
	}

	template<typename T, typename ...Args>
	inline T& Entity::add(Args && ...args)
	{
		return m_manager->add<T>(m_id, std::forward<Args>(args)...);

	}

	template <typename T> 
	T& Entity::get() const { 
		return m_manager->get<T>(m_id); 
	}

	template <typename T>
	bool Entity::has() const {
		return m_manager->has<T>(m_id);
	}

	template <typename T>
	void Entity::remove() {
		m_manager->remove<T>(m_id);
	}
}