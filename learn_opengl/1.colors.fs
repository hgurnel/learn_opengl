#version 330 core

in vec3 Normal;
// This input variable will be interpolated from the 3 world-position vectors of 
// the triangle to form the FragPos vector that is the per-fragment world position.
in vec3 FragPos;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    // ----- AMBIENT 

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // ----- DIFFUSE 

    // Computation of the light's direction vector, ie the direction vector between
    // the light source and the fragment's position
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal); 

    // Diffuse impact of the light on the current fragment
    // If the angle between both vectors is greater than 
    // 90 degrees then the result of the dot product will 
    // actually become negative and we end up with a negative 
    // diffuse component. For that reason we use the max function.
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // ----- RESULT 

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}