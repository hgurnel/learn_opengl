#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D fsTexture1;
uniform sampler2D fsTexture2;

void main()
{
    // Mix of 0.2: 80% texture1 / 20% texture2
    FragColor = mix(texture(fsTexture1, TexCoord), texture(fsTexture2, TexCoord*vec2(-1.0, 1.0)), 0.2);
}