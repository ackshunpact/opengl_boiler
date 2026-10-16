#pragma once
#include <cstdint>
#include <limits>

namespace engine::ecs {
	
	using EntityId = std::uint32_t;
	inline constexpr EntityId kNullEntity = std::numeric_limits<EntityId>::max();
	class ComponentManager;

	struct Entity {
		Entity() = default;
		Entity(EntityId id, ComponentManager* manager) : m_id(id), m_manager(manager) {};

		EntityId id() const { return m_id; }
		bool valid() const;


		template <typename T, typename... Args>
		T& add(Args&&... args);

		template <typename T> 
		T& get() const;

		template <typename T> 
		bool has() const;

		template <typename T>
		void remove();

		void destroy();
		//bool operator==(const EntityId&) const = default;


	private:
		EntityId m_id = kNullEntity;
		ComponentManager* m_manager = nullptr;

	};
}