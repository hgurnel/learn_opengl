#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
// Gouraud Shading
out vec4 FragCol;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// Gouraud Shading  
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	// gl_position = the clip-space position of the current vertex 
	// clip space = normalized device coordinates (NDC), between -1 and 1
	// It is a predefined output of the vertex shader
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	// Normal vector expressed in world coordinates using 
	// a normal matrix, which is derived from the model matrix.
	// Not multiplied directly by the model matrix, otherwise
	// in case of non-uniform scaling for instance, normals 
	// would not be perpendicular to the faces of the cube anymore.
	// Performance issue: inverting a matrix is costly, so it 
	// should normally be done on the CPU and then sent to the shader via a uniform
	Normal = mat3(transpose(inverse(view * model))) * aNormal;
	
	// Fragment position expressed in world coordinates
	FragPos = vec3(model * vec4(aPos, 1.0));

	// ----- GOURAUD SHADING

	// AMBIENT 

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // DIFFUSE 

    // Computation of the light's direction vector, ie the direction vector between
    // the light source and the fragment's position
    vec3 lightDir = normalize(lightPos - FragPos);
    
    vec3 normalVec = normalize(Normal); 

    // Diffuse impact of the light on the current fragment
    // If the angle between both vectors is greater than 
    // 90 degrees then the result of the dot product will 
    // actually become negative and we'll end up with a negative 
    // diffuse component. To avoid this, we use the max function.
    float diff = max(dot(normalVec, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // SPECULAR

    // Specular intensity value to give the specular highlight
    // a medium-bright color so that it doesn't have too much of an impact
    float specularStrength = 3;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalVec);

    // This 32 value is the shininess value of the highlight. 
    // The higher the shininess value of an object, the more it properly reflects the light
    // instead of scattering it all around and thus the smaller the highlight becomes.
    int shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // RESULT 

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragCol = vec4(result, 1.0);
}