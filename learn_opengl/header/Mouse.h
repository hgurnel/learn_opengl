#ifndef MOUSE_H
#define MOUSE_H

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

#include "Camera.h"

// Settings
extern const unsigned int SCREEN_WIDTH;
extern const unsigned int SCREEN_HEIGHT;

class Mouse
{
public:
	Mouse(Camera* camera, float yaw, float pitch);
	~Mouse();
 
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

private:
	Camera* m_camera;

	bool m_isFirstMouse;
	
	float m_lastX;
	float m_lastY;

	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in 
	// a direction vector pointing to the right, so we initially rotate a bit to the left.
	float m_yaw;
	float m_pitch;
};

#endif