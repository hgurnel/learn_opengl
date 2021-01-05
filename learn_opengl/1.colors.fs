#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// NB: a texture is bound to a sampler
struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
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

    // texture() accesses the texture at a given position and returns the color in normalised RGBA 
    // (this is important, the output color in GLSL ranges from 0 to 1, not 0 to 255)
    // - sampler2D means that the texture is in texel space -> it ranges from 0 to its size.
    // - texcoord is the variation of that fragment. It corresponds to the coords at which the texture will be sampled
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

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
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    // SPECULAR

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalVec);

    // This 32 value is the shininess value of the highlight. 
    // The higher the shininess value of an object, the more it properly reflects the light
    // instead of scattering it all around and thus the smaller the highlight becomes.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // light.specular (vec3), spec (float), texture(sampler, texCoords) (vec3)
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // EMISSIVE

    vec3 emissive = vec3(0.0);
    if (texture(material.specular, TexCoords).r == 0.0)   //rough check for blackbox inside spec texture
    {
        //apply emissive texture
        emissive = vec3(texture(material.emissive, TexCoords));
    }

    // RESULT 

    vec3 result = ambient + diffuse + specular + emissive;
    FragColor = vec4(result, 1.0);
}