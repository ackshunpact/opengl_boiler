#include <engine/core/Window.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

engine::Window::Window(int width, int height, const char* title)
{


	this->title = std::string(title);
	if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW.");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifndef NDEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	#endif:back

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

	if (GLFW_TRUE == glfwGetWindowAttrib(m_window, GLFW_OPENGL_DEBUG_CONTEXT)) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
		glDebugMessageCallback([](GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar* message, const void*)
			{
				std::cerr << "GL: " << message << std::endl;
			}, 0);
	}

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

void engine::Window::setCursorInputMode(int inputMode) {
	glfwSetInputMode(m_window, GLFW_CURSOR, inputMode);
}
