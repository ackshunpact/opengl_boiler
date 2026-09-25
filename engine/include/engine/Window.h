#pragma once

struct GLFWwindow;

namespace engine {

	class Window {
	public:
		Window(int width, int height, const char* title);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();

		bool shouldClose() const;
		void swapBuffers();
		void pollEvents();

		GLFWwindow* handle() const { return m_window; }
	private:
		GLFWwindow* m_window = nullptr;

	};
}