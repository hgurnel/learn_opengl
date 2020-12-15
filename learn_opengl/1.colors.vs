#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	// Normal vector expressed in VIEW coordinates using 
	// a normal matrix, which is derived from the model matrix
	// Not multiplied directly by model matrix, otherwise
	// in case of non-uniform scaling (for instance), normals 
	// would not be perpendicular to the faces of the cube anymore
	// Performance issue: inverting a matrix is costly, so it 
	// should normally be done on the CPU and then sent to the shader via a uniform
	Normal = mat3(transpose(inverse(view * model))) * aNormal;
	
	// Fragment position expressed in VIEW coordinates
	FragPos = vec3(view * model * vec4(aPos, 1.0));

	// Light position expressed in VIEW coordinates, knowing that it is currently in world space
	LightPos = vec3(view * vec4(lightPos, 1.0));
}