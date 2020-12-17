#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light 
{
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // AMBIENT 

    vec3 ambient = material.ambient * light.ambient;

    // DIFFUSE 

    // Computation of the light's direction vector, ie the direction vector between
    // the light source and the fragment's position
    vec3 lightDir = normalize(light.position - FragPos);
    
    vec3 normalVec = normalize(Normal); 

    // Diffuse impact of the light on the current fragment
    // If the angle between both vectors is greater than 
    // 90 degrees then the result of the dot product will 
    // actually become negative and we'll end up with a negative 
    // diffuse component. To avoid this, we use the max function.
    float diff = max(dot(normalVec, lightDir), 0.0);
    vec3 diffuse = (material.diffuse * diff) * light.diffuse;

    // SPECULAR

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalVec);

    // This 32 value is the shininess value of the highlight. 
    // The higher the shininess value of an object, the more it properly reflects the light
    // instead of scattering it all around and thus the smaller the highlight becomes.
    int shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;

    // RESULT 

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}