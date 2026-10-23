#pragma once

#include <engine/debug/FrameStats.h>
#include <imgui.h>

namespace engine {
	struct FrameStats;
	void drawStatsPanel(const FrameStats& stats);
}