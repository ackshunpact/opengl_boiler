#include <engine/Application.h>
#include <engine/Window.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


namespace engine {
	Application::Application(const AppConfig& config) {
		window_width = config.width; 
		window_height = config.height;
		window_title = config.title.c_str();
		m_window = new Window(window_width, window_height, window_title);



		
	}


	void Application::run() {
		onInit();
		double prev_time = glfwGetTime();

		while (!m_window->shouldClose()) {
			double current_time = glfwGetTime();
			float dt = (float)current_time - prev_time;
			prev_time = current_time;
			m_window->pollEvents();
			
			onUpdate(dt);
			onRender();
			
			m_window->swapBuffers();
		}
	}
}