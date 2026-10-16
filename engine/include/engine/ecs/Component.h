#pragma once
#include <type_traits>

namespace engine::ecs {
	class Component {

	};

	template <typename T>
	concept ComponentType = std::is_base_of_v<Component, T>;
}