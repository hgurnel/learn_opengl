#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	// Normal vector expressed in world coordinates using 
	// a normal matrix, which is derived from the model matrix
	// Not multiplied directly by model matrix, otherwise
	// in case of non-uniform scaling (for instance), normals 
	// would not be perpendicular to the faces of the cube anymore
	// Performance issue: inverting a matrix is costly, so it 
	// should normally be done on the CPU and then sent to the shader via a uniform
	Normal = mat3(transpose(inverse(model))) * aNormal;
	
	// Fragment position expressed in world coordinates
	FragPos = vec3(model * vec4(aPos, 1.0));
}