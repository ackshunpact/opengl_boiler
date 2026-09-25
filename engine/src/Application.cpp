#include <engine/Application.h>
#include <engine/Window.h>
#include <glad/glad.h>

engine::Application::Application() {
	window_width = 800;
	window_height = 600;
	window_title = "Untitled";
	m_window = new engine::Window(window_width, window_height, window_title);
}

engine::Application::Application(int width, int height, const char* title)
{
	window_width = width;
	window_height = height;
	window_title = title;
	m_window = new engine::Window(window_width, window_height, window_title);
}

engine::Application::~Application()
{
}

void engine::Application::onInit()
{

}

void engine::Application::onUpdate()
{
}

void engine::Application::onRender()
{
}

void engine::Application::run() {
	while (!m_window->shouldClose()) {
		glClearColor(0.7f, 0.7f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		m_window->swapBuffers();
		m_window->pollEvents();
	}
}