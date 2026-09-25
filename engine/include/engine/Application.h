#pragma once
#include <string>

namespace engine {
	struct AppConfig {
		int width = 1280;
		int height = 720;
		std::string title = "Untitled";
	};
	class Window;
	
	class Application {
	public:
		Application() = delete;
		Application(const AppConfig& config);
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		~Application() = default;

	

		void run();
		int r = 10, b = 122, g = 255;
		int dr = 1;
		int dg = 1;
		int db = 1;

	private:
		int window_width; 
		int window_height;
		const char* window_title;
		engine::Window* m_window;

	protected:
		// compiling shaders, creating VAOs and VBOs, 
		// loading textures, and one-time state like 
		// glEnable(GL_DEPTH_TEST)
		virtual void onInit() {}

		// runs every frame and changes the state of 
		// your world: reading input, moving the camera, 
		// rotating objects, running game logic. dt is 
		// the seconds since the last frame, e.g. 0.0166 
		// at 60 FPS. Multiply movement by dt, as in 
		// pos += speed * dt, so motion is the same speed 
		// on a 60 Hz and a 144 Hz monitor.
		virtual void onUpdate(float) {}

		// runs every frame and draws the current state: 
		// clear the screen, bind shaders, set uniforms, 
		// issue draw calls. It shouldn't change anything. 
		// Keeping it separate from onUpdate pays off later. 
		// Pausing becomes "skip update but keep rendering," 
		// and you can move to fixed-step physics (update 
		// at a steady 60 Hz while rendering as fast as 
		// possible) without touching your drawing code.
		virtual void onRender() {}
		virtual void onShutDown() {}
	};
}