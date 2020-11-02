#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

// texture samplers, in order to pass texture objects (created in the main) to the frag shader
// A sampler is a built-in data type for texture objects
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// texture(textureSampler, textureCoord) = GLSL's built-in texture fct 
	// linearly interpolate between both textures (80% rectangular container, 20% smiley). 0.0 displays only texture1.
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}