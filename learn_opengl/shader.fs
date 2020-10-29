#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D fsTexture1;
uniform sampler2D fsTexture2;

void main()
{
    // FragColor = mix(texture(fsTexture1, TexCoord), texture(fsTexture2, TexCoord*vec2(-1.0, 1.0)), 0.2);
    FragColor = mix(texture(fsTexture1, TexCoord), texture(fsTexture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2);
}