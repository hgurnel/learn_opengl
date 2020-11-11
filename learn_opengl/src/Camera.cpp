#include "../header/Camera.h"

Camera::Camera() : m_view(glm::mat4(1.0f)), m_position(glm::vec3(0.0f, 0.0f, 0.0f)), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_up(glm::vec3(0.0f, 1.0f, 0.0f)), m_speed(0.0f), m_lastFrame(0.0f)
{

}

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up) : m_view(glm::mat4(1.0f)), m_position(pos), m_front(front), m_up(up), m_speed(0.0f), m_lastFrame(0.0f)
{
	
}

// Getters

glm::mat4 Camera::getView()
{
	return m_view;
}

// Setters

void Camera::setFront(glm::vec3 newFront)
{
	m_front = newFront;
}

void Camera::setView()
{
	m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::setSpeed()
{
	float currentFrame = glfwGetTime();
	float deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;

	m_speed = SPEED_MAGNITUDE * deltaTime;
}

void Camera::moveForward()
{
	m_position += m_speed * m_front;
}

void Camera::moveBackward()
{
	m_position -= m_speed * m_front;
}

void Camera::moveLeft()
{
	m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
}

void Camera::moveRight()
{
	m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed;
}