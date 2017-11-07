#version 430 core

	layout (location = 0) in vec4 vPosition;
	layout (location = 1) in vec4 vColor;
	out vec4 color;
	uniform mat4 transform;

	void main(){
	   color = vColor;
	   vec4 newPos = vPosition;
	   newPos = transform*newPos;
	   gl_Position = newPos;
	}