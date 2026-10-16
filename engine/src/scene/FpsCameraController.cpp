#include <engine/scene/FpsCameraController.h>
#include <cmath>
#include <algorithm>


engine::FpsCameraController::FpsCameraController(Mode mode)
{
	m_mode = mode;
}

void engine::FpsCameraController::update(Input& input, Camera& camera, float dt)
{
	handleCapture(input);
	handleSpeedScroll(input);
	
	if (!m_captured) return;

	handleLook(input, camera);
	handleMove(input, camera, dt);
}

void engine::FpsCameraController::handleCapture(Input& input)
{
	m_captured = true;
}

void engine::FpsCameraController::handleLook(const Input& input, Camera& camera)
{
	const glm::vec2 d = input.mouseDelta();
	if (d.x == 0.0f && d.y == 0.0f) {
		return;
	}
	camera.rotate(d.x * m_sensitivity, -d.y * m_sensitivity);
}

void engine::FpsCameraController::handleMove(const Input& input, Camera& camera, float dt)
{
	static const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 forward = camera.getFront();
	if (m_mode == Mode::Walk) {
		forward.y = 0.0f;
		if (glm::length(forward) > 0.0f) forward = glm::normalize(forward);
	}

	const glm::vec3 right = camera.getRight();
	glm::vec3 dir(0.0f);
	if (input.isKeyDown(GLFW_KEY_W)) dir += forward;
	if (input.isKeyDown(GLFW_KEY_S)) dir -= forward;

	if (input.isKeyDown(GLFW_KEY_D)) dir += right;
	if (input.isKeyDown(GLFW_KEY_A)) dir -= right;

	if (input.isKeyDown(GLFW_KEY_SPACE)) dir += worldUp;
	if (input.isKeyDown(GLFW_KEY_LEFT_CONTROL)) dir -= worldUp;

	if (glm::length(dir) == 0.0f) return;
	dir = glm::normalize(dir);

	float speed = m_moveSpeed;
	if (input.isKeyDown(GLFW_KEY_LEFT_SHIFT)) speed *= m_sprintMult;

	camera.move(dir * speed * dt);

}

void engine::FpsCameraController::handleSpeedScroll(const Input& input)
{
	const float scroll = input.scrollDelta();
	if (scroll == 0.0f) return;
	m_moveSpeed = std::clamp(m_moveSpeed * std::pow(1.1f, scroll), m_minSpeed, m_maxSpeed);
}
