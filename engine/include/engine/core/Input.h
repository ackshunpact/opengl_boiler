#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>
namespace engine {
	class Input {
	public:
		explicit Input(GLFWwindow* window);
		void update();
		bool isKeyDown(int key) const;
		bool wasKeyPressed(int key) const;
		bool isMouseDown(int button) const;

		glm::vec2 mouseDelta() const;
		float scrollDelta() const;

		void setCursorCaptured(bool captured);
		bool isCursorCaptured() const { return m_cursorCaptured;  }

		void setBlocked(bool b) { m_blocked = b; }
	private:

		GLFWwindow* m_window;
		std::array<bool, GLFW_KEY_LAST + 1> m_keys{}, m_prevKeys{}, m_frameKeys{};
		glm::dvec2 m_lastCursor{}, m_mouseDelta{}, m_mouseAccum{};
		bool m_firstMouse = true;
		float m_scroll = 0.f, m_scrollAccum = 0.f;
		double lastX = 640.0, lastY = 360.0;
		float sensitivity = 0.1f;
		bool m_blocked = false;
		bool m_cursorCaptured = false;
	};
}