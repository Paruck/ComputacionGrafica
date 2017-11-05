#version 430 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

out vec4 color;
uniform float angle;
uniform mat4 transform;

void main()
{
	color = vColor;
	gl_Position = transform * vPosition;
}