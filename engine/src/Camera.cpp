#include <engine/Camera.h>

glm::mat4 engine::Camera::viewMatrix()
{
	//glm::mat4 view = glm::mat4(1.0f);
	//return glm::translate(view, position);
	return glm::lookAt(position, position + front, up);
}

glm::mat4 engine::Camera::projectionMatrix()
{
	return  glm::perspective(glm::radians(fov), aspect, 0.1f,
		100.0f);

}

void engine::Camera::setAspect(float aspect)
{
	aspect = aspect;
}

void engine::Camera::rotate(float dYaw, float dPitch)
{
	yaw += dYaw;
	pitch = glm::clamp(pitch + dPitch, -89.0f, 89.0f);
	updateVectors();
}

void engine::Camera::move(glm::vec3 offset)
{
	position = position + offset;
}

void engine::Camera::updateVectors()
{
	static const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

	const float yawRad = glm::radians(yaw);
	const float pitchRad = glm::radians(pitch);
	
	front.x = std::cos(yawRad) * std::cos(pitchRad);
	front.y = std::sin(pitchRad);
	front.z = std::sin(yawRad) * std::cos(pitchRad);
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

glm::vec3 engine::Camera::getPosition() const {
	return position;
}

glm::vec3 engine::Camera::getFront() const {
	return front;
}

glm::vec3 engine::Camera::getRight() const {
	return right;
}
