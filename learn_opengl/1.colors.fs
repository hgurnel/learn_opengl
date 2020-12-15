#version 330 core

// In view coords for the exercice
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    // ----- AMBIENT 

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // ----- DIFFUSE 

    // Computation of the light's direction vector, ie the direction vector between
    // the light source and the fragment's position
    vec3 lightDir = normalize(LightPos - FragPos);
    
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
    float specularStrength = 3;

    // The viewer is always at (0,0,0) in view-space, so viewDir is ((0,0,0) - FragPos) => (-FragPos)
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, normalVec);

    // This 32 value is the shininess value of the highlight. 
    // The higher the shininess value of an object, the more it properly reflects the light
    // instead of scattering it all around and thus the smaller the highlight becomes.
    int shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // ----- RESULT 

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}