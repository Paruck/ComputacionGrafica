#version 430 core

out vec4 fColor;
in vec4 color;
uniform  sampler2D texture0;
uniform  sampler2D texture1;
in vec2 textureCoords;

void main()
{
	vec4 textura0 = texture(texture0, textureCoords);
    vec4 textura1 = texture(texture1, textureCoords);
    fColor = mix(textura0, textura1, 0.5);
}
