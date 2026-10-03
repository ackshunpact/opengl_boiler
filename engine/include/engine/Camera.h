#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>

namespace engine {
	class Camera {
	private:
		glm::vec3 position, front, right, up;
		float yaw, pitch, fov, aspect;
		void updateVectors();
	public:
		Camera()
			: position(glm::vec3{})
			, front(glm::vec3(0.0f, 0.0f, -1.0f))
			, right(glm::vec3(1.0f, 0.0f, 0.0f))
			, up(glm::vec3(0.0f, 1.0f, 0.0f))
			, yaw(-90.0f)
			, pitch(0.0f)
			, fov(60.0f)
			, aspect(1080.f / 720.f)
		{
			updateVectors();
		}



		glm::mat4 viewMatrix();
		glm::mat4 projectionMatrix();
		void setAspect(float aspect);
		void rotate(float dYaw, float dPitch);
		void move(glm::vec3 offset);
		glm::vec3 getPosition() const;
		glm::vec3 getFront() const;
		glm::vec3 getRight() const;


	};

}