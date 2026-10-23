#pragma once

struct GLFWwindow;

namespace engine {
	class ImGuiLayer {
	public:
		explicit ImGuiLayer(GLFWwindow* window);
		~ImGuiLayer();

		ImGuiLayer(const ImGuiLayer&) = delete;
		ImGuiLayer& operator=(const ImGuiLayer&) = delete;

		void beginFrame();
		void endFrame();

		bool wantsMouse() const;
		bool wantsKeyboard() const;

	};
}