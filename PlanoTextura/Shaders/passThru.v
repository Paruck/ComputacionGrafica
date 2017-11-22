#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec2 vTexture;

uniform mat4 mTransform;
uniform float fTime;

vec4 newPos;
out vec4 color;
out vec2 textureCoords;

void main()
{
	newPos = vPosition;	 //OJO: NO LO ESTAMOS USANDO
	textureCoords = vTexture;

	gl_Position = mTransform * newPos;
}
	
	
	
	
