#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexture;

uniform mat4 mTransform;
uniform float fTime;
uniform sampler2D texture1;

vec4 newPos;
out vec4 color;
out vec2 textureCoords;

void main()
{
	newPos = vPosition;	 //OJO: NO LO ESTAMOS USANDO
	textureCoords = vTexture;

	color = texture(texture1, textureCoords);
	newPos.y = 10.0* (0.2126* color.r + 0.7152 * color.g + 0.0722 * color.b);


	gl_Position = mTransform * newPos;
}
	
	
	
	
