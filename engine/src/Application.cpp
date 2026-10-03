#include <engine/Application.h>
#include <engine/Window.h>
#include <engine/Input.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace engine {
	Application::Application(const AppConfig& config) {
		window_width = config.width; 
		window_height = config.height;
		window_title = config.title.c_str();
		m_window = std::make_unique<Window>(window_width, window_height, window_title);
		m_input = std::make_unique<Input>(m_window.get()->handle());		
	}

	engine::Application::~Application() = default;
	void Application::run() {
		onInit();
		double prev_time = glfwGetTime();

		while (!m_window->shouldClose()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			double current_time = glfwGetTime();
			float dt = (float)current_time - prev_time;
			prev_time = current_time;
			
			m_input->update();
			m_window->pollEvents();
			
			onUpdate(dt);
			onRender();
			
			m_window->swapBuffers();
		}
	}
	Input& engine::Application::getInput() const {
		return *m_input;
	}
	Window& Application::getWindow() const
	{
		return *m_window;
	}
}