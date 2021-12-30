#pragma once
#ifndef CAMERA
#define CAMERA

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

const float YAW = -90.0f;
const float PITCH = 45.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum class Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 worldUp;

	void UpdateVectors();

public:

	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	glm::vec3 CameraPosition() { return cameraPosition; }
	glm::vec3 CameraDirection() { return cameraDirection; }
	glm::vec3 CameraRight() { return cameraRight; }
	glm::vec3 CameraUp() { return cameraUp; }
	glm::vec3 WorldUp() { return worldUp; }

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseScroll(double yoffset);
	void PorcessMousemovment(float xoffset, float yoffset, GLboolean contraintPitch = true);

	Camera(glm::vec3 cameraPosition_ = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _worldUp = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 _cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f), float _yaw = YAW, float _pitch = PITCH);
};
#endif