#pragma once
#include <engine/ecs/Component.h>
#include <engine/ecs/Entity.h>

#include <cassert>
#include <stdexcept>
#include <cstddef>
#include <limits>
#include <span>
#include <utility>
#include <vector>


// based on enTT sparse sets

namespace engine::ecs {

	// main interface to inherit
	class IComponentArray {
	public:
		IComponentArray() = default;
		virtual ~IComponentArray() = default;
		virtual bool has(EntityId e) const = 0;
		virtual void remove(EntityId e) = 0;

	};

	template <ComponentType T>
	class ComponentArray : public IComponentArray {
	public:
		template <typename... Args>
		T& emplace(EntityId e, Args&&... args) {
			if (has(e)) {
				throw std::runtime_error("Already has that component.");
			}

			if (e >= m_sparse.size()) {
				m_sparse.resize(static_cast<std::size_t>(e) + 1, kNone);
			}
			m_sparse[e] = m_dense.size();
			m_entities.push_back(e);
			m_dense.emplace_back(std::forward<Args>(args)...);
			return m_dense.back();
		}

		// if the ID is less than the size of m_sparse it exists unless it doesn't!
		bool has(EntityId e) const override {
			return e < m_sparse.size() && m_sparse[e] != kNone;
		}

		T& get(EntityId e) {
			if (!has(e)) {
				throw std::runtime_error("ComponentArray does not have that entity.");
			}
			return m_dense[m_sparse[e]];
		}

		void remove(EntityId e) override {
			if (!has(e)) return;
			const std::size_t idx = m_sparse[e];
			const std::size_t last = m_dense.size() - 1;

			if (idx != last) {
				m_dense[idx] = std::move(m_dense[last]);
				m_entities[idx] = m_entities[last];
				m_sparse[m_entities[idx]] = idx;
			}
			m_dense.pop_back();
			m_entities.pop_back();
			m_sparse[e] = kNone;
		}

		std::size_t size() const { return m_dense.size(); }
		T& at(std::size_t i) { return m_dense[i]; }
		std::span<const EntityId> entities() const {
			return m_entities;
		}
	private:
		static constexpr std::size_t kNone = std::numeric_limits<std::size_t>::max();
		std::vector<T> m_dense;
		std::vector<EntityId> m_entities;
		std::vector<std::size_t> m_sparse;

	};


















}