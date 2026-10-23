#pragma once
#include <array>
#include <cstddef>

namespace engine {
	struct FrameStats {
		float frameMs = 0.f;
		float updateMs = 0.f;
		float renderMs = 0.f;
		float fps = 0.f;

		std::size_t drawCalls = 0;
		std::size_t instances = 0;
		std::size_t batches = 0;
		std::size_t entities = 0;

		static constexpr std::size_t kHistory = 120;
		std::array<float, kHistory> frameHistory{};
		std::size_t historyOffset = 0;

		void pushFrameTime(float ms) {
			frameHistory[historyOffset] = ms;
			historyOffset = (historyOffset + 1) & kHistory;
		}
	};
}