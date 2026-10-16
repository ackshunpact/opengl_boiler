#include <engine/core/Input.h>
engine::Input::Input(GLFWwindow* window) {

	m_window = window;
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int, int action, int) {
		if (key < 0 || key > GLFW_KEY_LAST) return;
		auto* self = static_cast<Input*>(glfwGetWindowUserPointer(w));
		if (action == GLFW_PRESS) {
			self->m_keys[key] = true;
		}
		if (action == GLFW_RELEASE) self->m_keys[key] = false;
		});
	glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
		auto* self = static_cast<Input*>(glfwGetWindowUserPointer(w));
		if (self->m_firstMouse) {
			self->m_lastCursor = { xpos, ypos };
			self->m_firstMouse = false;
			return;
		}
		self->m_mouseAccum += glm::dvec2(xpos, ypos) - self->m_lastCursor;
		self->m_lastCursor = { xpos, ypos };
		});
}

void engine::Input::update()
{

	m_prevKeys = m_frameKeys;
	m_frameKeys = m_keys;

	m_mouseDelta = m_mouseAccum;
	m_mouseAccum = {};

	if (isKeyDown(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(m_window, true);
	}

	if (isKeyDown(GLFW_KEY_P)) {
		if (glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

bool engine::Input::isKeyDown(int key) const
{
	return m_keys[key];
}

bool engine::Input::wasKeyPressed(int key) const
{
	return m_prevKeys[key];
}

bool engine::Input::isMouseDown(int button) const
{
	return false;
}

glm::vec2 engine::Input::mouseDelta() const
{
	return m_mouseDelta;
}

float engine::Input::scrollDelta() const
{
	return 0.0f;
}

void engine::Input::setCursorCaptured(bool captured)
{
}
