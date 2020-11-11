#ifndef CAMERA_H
#define CAMERA_H

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	const float SPEED_MAGNITUDE = 2.5f;

public:
	Camera();
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);

	// Getters
	glm::mat4 getView();

	// Setters
	void setFront(glm::vec3 newFront);
	void setView();
	void setSpeed();
	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();

private:
	glm::mat4 m_view;

	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;

	float m_speed;
	float m_lastFrame;
};

#endif
