#pragma once

namespace engine {
	
	class Window;
	
	class Application {
	public:
		Application();
		Application(int width, int height, const char* title);
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		~Application();

		void onInit();
		void onUpdate();
		void onRender();

		void run();


	private:
		int window_width; 
		int window_height;
		const char* window_title;
		engine::Window* m_window;
	};
}