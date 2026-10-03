#include <engine/Application.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <memory>

#include <engine/VertexBuffer.h>
#include <engine/VertexArray.h>
#include <engine/IndexBuffer.h>
#include <engine/Shader.h>
#include <engine/MeshData.h>
#include <engine/ModelLoader.h>
#include <engine/Mesh.h>
#include <engine/Camera.h>
#include <engine/FpsCameraController.h>
#include <engine/Window.h>

float vertices[]{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

float vertices2[]{
0.5f, 0.5f, 0.0f, // top right
0.5f, -0.5f, 0.0f, // bottom right
-0.5f, -0.5f, 0.0f, // bottom left
-0.5f, 0.5f, 0.0f // top left
};


unsigned int indices[]{
	0, 1, 3,
	1, 2, 3
};




const char* vsPath = "D:/dev/cpp/opengl_boiler/app/default.vert";
const char* fsPath = "D:/dev/cpp/opengl_boiler/app/default.frag";

class Demo : public engine::Application {
public:
	
	using Application::Application;

	float m_time = 0.f;
	engine::VertexLayout vlo = {
			{engine::AttributeType::Float3, "aPos" } //,
			// { engine::AttributeType::Float3, "aNormal" },
			// {engine::AttributeType::Float2, "aUV" },
	};
	std::unique_ptr<engine::VertexBuffer> vbo = nullptr;
	std::unique_ptr<engine::IndexBuffer> ibo = nullptr;
	std::unique_ptr<engine::VertexArray> vao = nullptr;
	std::unique_ptr<engine::Shader> shader = nullptr;
	std::unique_ptr<engine::Model> m_car = nullptr;
	std::unique_ptr<engine::Camera> m_camera = nullptr;
	std::unique_ptr<engine::FpsCameraController> m_controller = nullptr;

	void onInit() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		const char* carModelPath = "D:/dev/cpp/opengl_boiler/assets/CarConcept.glb";
		m_controller = std::make_unique<engine::FpsCameraController>(engine::FpsCameraController::Mode::Fly);
		m_camera = std::make_unique<engine::Camera>();
		m_camera->move(glm::vec3{0, 1, 5});
		getWindow().setCursorInputMode(GLFW_CURSOR_DISABLED);

		m_car = std::make_unique<engine::Model>(engine::ModelLoader::loadGltf(carModelPath));
		
		shader = std::make_unique<engine::Shader>(vsPath, fsPath);
		shader->use();
	}

	void onUpdate(float dt) {

		m_controller->update(getInput(), *m_camera, dt);
	}

	void onRender() {
		//vao->bind();
		shader->use();
		shader->setMat4("uProjection", m_camera->projectionMatrix());
		shader->setMat4("uView", m_camera->viewMatrix());
		shader->setMat4("uModel", glm::mat4(1.0f));
		shader->setVec3("uLightPos", glm::vec3(5.f, 8.f, 5.f));
		shader->setVec3("uViewPos", m_camera->getPosition());
		shader->setVec3("uLightColor", glm::vec3(1.f, 1.f, 1.f));
		shader->setVec3("uObjectColor", glm::vec3(1.0f, 0.0f, 1.0f));
		m_car->draw();
	}

	void onShutdown() {

	}
};

int main() {
	// D:\dev\cpp\opengl_boiler\assets\CarConcept.glb

	Demo demo({1280, 720, "Demo"});
	demo.run();
}