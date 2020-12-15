#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 translationVec;

mat4 translation = mat4(1.0, 0.0, 0.0, translationVec.x, 
						0.0, 1.0, 0.0, translationVec.y, 
						0.0, 0.0, 1.0, translationVec.z,  
						0.0, 0.0, 0.0, 1.0);

void main()
{
	gl_Position = projection * view * translation * model * vec4(aPos, 1.0);
}