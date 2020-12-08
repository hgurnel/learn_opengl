#version 330 core

in vec3 Normal;
// This input variable will be interpolated from the 3 world-position vectors of 
// the triangle to form the FragPos vector that is the per-fragment world position.
in vec3 FragPos;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // ----- AMBIENT 

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // ----- DIFFUSE 

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

    // ----- SPECULAR

    // Specular intensity value to give the specular highlight
    // a medium-bright color so that it doesn't have too much of an impact
    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalVec);

    // This 32 value is the shininess value of the highlight. 
    // The higher the shininess value of an object, the more it properly reflects the light
    // instead of scattering it all around and thus the smaller the highlight becomes.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // ----- RESULT 

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}