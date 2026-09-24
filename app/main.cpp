
#include <engine/Window.h>
#include <glad/glad.h>

int main() {
	engine::Window window(1280, 720, "opengl_boiler");
	while (!window.shouldClose()) {
		glClearColor(0.7f, 0.7f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		window.swapBuffers();
		window.pollEvents();
	}
}