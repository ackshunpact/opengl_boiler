#include "engine/Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

engine::Window::Window(int width, int height, const char* title)
{
	if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW.");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!m_window) throw std::runtime_error("Failed to create GLFW Window.");

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD.");
	}

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow*, int w, int h) {
		glViewport(0, 0, w, h);
		});

}

engine::Window::~Window()
{
	if (m_window) glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool engine::Window::shouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

void engine::Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

void engine::Window::pollEvents()
{
	glfwPollEvents();
}
