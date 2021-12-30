#include "Camera.h"

Camera::Camera(glm::vec3 cameraPosition_, glm::vec3 _worldUp, glm::vec3 _cameraDirection, float _yaw, float _pitch) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
	cameraPosition = cameraPosition_;
	worldUp = _worldUp;
	cameraDirection = _cameraDirection;
	Yaw = _yaw;
	Pitch = _pitch;
	UpdateVectors();
}

void Camera::UpdateVectors() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	direction.y = sin(glm::radians(Pitch));
	direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	cameraDirection = glm::normalize(direction);

	cameraRight = glm::normalize(glm::cross(cameraDirection, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;

	switch (direction)
	{
	case Camera_Movement::FORWARD:
		cameraPosition += cameraDirection * velocity;
		break;
	case Camera_Movement::BACKWARD:
		cameraPosition -= cameraDirection * velocity;
		break;
	case Camera_Movement::LEFT:
		cameraPosition -= cameraRight * velocity;
		break;
	case Camera_Movement::RIGHT:
		cameraPosition += cameraRight * velocity;
		break;
	default:
		break;
	}
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
}


void Camera::ProcessMouseScroll(double yoffset) {
	Zoom -= yoffset;
	if (Zoom < 1.0) {
		Zoom = 1.0;
	}
	if (Zoom > 45.0) {
		Zoom = 45.0;
	}
}

void Camera::PorcessMousemovment(float xoffset, float yoffset, GLboolean constraintPitch) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constraintPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	UpdateVectors();
}