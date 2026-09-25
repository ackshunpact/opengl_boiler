#include <engine/Application.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <iostream>
#include <engine/VertexBuffer.h>
#include <engine/VertexArray.h>
#include <engine/Shader.h>

float vertices[]{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

//const char* vsPath = "D:/dev/cpp/opengl_boiler/app/default.vert";
//const char* fsPath = "D:/dev/cpp/opengl_boiler/app/default.frag";

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
	engine::VertexBuffer vbo = engine::VertexBuffer(vertices, sizeof(vertices));
	engine::VertexArray vao;

	engine::Shader shader = engine::Shader(vsPath, fsPath);

	void onInit() {
		
		
		vao.addVertexBuffer(vbo, vlo);
		shader.use();
	}

	void onUpdate(float dt) {

	}

	void onRender() {
		vao.bind();
		shader.use();
		glDrawArrays(GL_TRIANGLES, 0, 3);

	}

	void onShutdown() {

	}
};

int main() {
	Demo demo({1280, 720, "Demo"});
	demo.run();
}