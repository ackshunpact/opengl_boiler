#include <engine/core/Application.h>
#include <engine/core/Window.h>
#include <engine/core/Input.h>
#include <engine/scene/TriangleScene.h>
#include <engine/scene/TriangleGridScene.h>
#include <engine/scene/HeightMapScene.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace engine {
	Application::Application(const AppConfig& config) {
		window_width = config.width; 
		window_height = config.height;
		window_title = config.title.c_str();
		m_window = std::make_unique<Window>(window_width, window_height, window_title);
		glClearColor(0.2f, 0.2f, 0.9f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		//glfwSetInputMode(m_window->handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		m_input = std::make_unique<Input>(m_window.get()->handle());
		m_input->setCursorCaptured(true);
		m_assets = std::make_unique<engine::AssetManager>();
		m_imgui = std::make_unique<engine::ImGuiLayer>(m_window->handle());
		// refactor into a function to add shader to renderer by paths
		// renderer->addShader(vsPath, fsPath) etc
		const char* vsPath = "D:/dev/cpp/opengl_boiler/app/default.vert";
		const char* fsPath = "D:/dev/cpp/opengl_boiler/app/default.frag";
		m_shader = std::make_unique<engine::Shader>(vsPath, fsPath);
		m_shader->use();
		m_renderer = std::make_unique<engine::Renderer>(*m_shader);
		loadScene<engine::HeightMapScene>();
		//loadScene<engine::TriangleGridScene>();
		//loadScene<engine::TriangleScene>();
	}

	engine::Application::~Application() = default;
	void Application::run() {
		onInit();
		double prev_time = glfwGetTime();
		double fpsTimer = 0.0;
		int frameCount = 0;

		if (m_nextScene) {
			if (m_scene) {
				m_scene->onDestroy();
			}

			m_scene = std::move(m_nextScene);
			m_scene->onCreate();
		}


		while (!m_window->shouldClose()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			double current_time = glfwGetTime();
			float dt = (float)(current_time - prev_time);
			prev_time = current_time;

			// count FPSes
			fpsTimer += dt;
			++frameCount;
			if (fpsTimer >= 0.5) {
				char title[128];
				std::snprintf(title, sizeof(title), "%s | %.0f FPS (%.2f ms)", window_title, frameCount / fpsTimer, 1000.0 * fpsTimer / frameCount);
				glfwSetWindowTitle(m_window->handle(), title);
				fpsTimer = 0.0;
				frameCount = 0;
			}




			m_input->update();
			m_window->pollEvents();

			m_imgui->beginFrame();
			m_input->setBlocked(m_imgui->wantsMouse() || m_imgui->wantsKeyboard());

			double t0 = glfwGetTime();
			m_scene->update(dt);
			double t1 = glfwGetTime();
			m_scene->render(*m_renderer);
			double t2 = glfwGetTime();
			m_scene->onDebugUI();

			constexpr float a = 0.1f;
			const float frameMs = dt * 1000.0f;
			m_stats.frameMs += a * (frameMs - m_stats.frameMs);
			m_stats.updateMs += a * (float((t1 - t0) * 1000.0) - m_stats.updateMs);
			m_stats.renderMs += a * (float((t2 - t1) * 1000.0) - m_stats.renderMs);
			m_stats.fps = m_stats.frameMs > 0.0f ? 1000.0f / m_stats.frameMs : 0.0f;
			m_stats.pushFrameTime(frameMs);                 // graph shows raw, unsmoothed spikes

			m_stats.drawCalls = m_renderer->lastDrawCalls();
			m_stats.instances = m_renderer->lastInstanceCount();
			m_stats.batches = m_renderer->lastBatchCount();
			m_stats.entities = m_scene->world().entityCount();

			//drawStatsPanel(m_stats);
			m_imgui->endFrame();
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