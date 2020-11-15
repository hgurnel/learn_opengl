#include "../header/Mouse.h"

Mouse::Mouse(Camera* cam, float yaw, float pitch) : m_camera(cam), m_yaw(yaw), m_pitch(pitch), m_lastX(SCREEN_WIDTH / 2.0f), m_lastY(SCREEN_HEIGHT / 2.0f)
{
    
}

Mouse::~Mouse()
{
    // No memory was allocated in the constructor of Mouse. 
    // The memory was allocated when instantiating a Camera object in main.cpp
    // So here, we just reset the ptr to nullptr
    m_camera = nullptr;
}

void Mouse::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (m_isFirstMouse)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_isFirstMouse = false;
    }

    float xoffset = xpos - m_lastX;
    float yoffset = m_lastY - ypos; // reversed since y-coordinates range from bottom to top
    m_lastX = xpos;
    m_lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));    
    m_camera->setFront(glm::normalize(direction));
}