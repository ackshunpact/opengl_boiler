#pragma once

#include <engine/render/Camera.h>
#include <engine/core/Input.h>

namespace engine {
	class Input;

	class FpsCameraController {
	public:
		enum class Mode {
			Fly,
			Walk
		};

		explicit FpsCameraController(Mode mode = Mode::Fly);
		void update(Input& input, Camera& camera, float dt);

		void setMode(Mode mode) { m_mode = mode; }
		void setMoveSpeed(float speed) { m_moveSpeed = speed; }
		void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }
		void setSprintMult(float sprintMult) { m_sprintMult = sprintMult; }
		float moveSpeed() const { return m_moveSpeed; }

	private:
		void handleCapture(Input& input);
		void handleLook(const Input& input, Camera& camera);
		void handleMove(const Input& input, Camera& camera, float dt);
		void handleSpeedScroll(const Input& input);

		Mode m_mode;
		bool m_captured = false;
		float m_moveSpeed = 5.0f;
		float m_sprintMult = 3.0f;
		float m_sensitivity = 0.1f;
		float m_minSpeed = 0.5f;
		float m_maxSpeed = 100.0f;




	};
}