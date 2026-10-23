#include <imgui.h>
#include <engine/debug/FrameStats.h>
namespace engine {
	void drawStatsPanel(const FrameStats& s) {
		ImGui::SetNextWindowPos(ImVec2(10.f, 10.f), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.6f);
		const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

		if (ImGui::Begin("Stats", nullptr, flags)) {
			ImGui::Text("%.0f FPS (%.2f ms)", s.fps, s.frameMs);
			ImGui::Text("update %.2f ms    render %.2f ms", s.updateMs, s.renderMs);
			ImGui::PlotLines("##frametime", s.frameHistory.data(), (int)s.frameHistory.size(),
				(int)s.historyOffset, nullptr, 0.0f, 33.3f, ImVec2(220.0f, 50.0f));

			ImGui::Separator();
			ImGui::Text("draw calls  %zu", s.drawCalls);
			ImGui::Text("batches     %zu", s.batches);
			ImGui::Text("instances   %zu", s.instances);
			ImGui::Text("entities    %zu", s.entities);
		}
		ImGui::End();
	}
}