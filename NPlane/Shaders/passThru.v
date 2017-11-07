#version 430 core

layout (location = 0) in vec4 vPosition;

uniform mat4 mTransform;
uniform float fTime;

vec4 newPos;
out vec4 color;

void main()
{
	newPos = vPosition;
	color = vec4(0.0, 0.7, 0.5, 1.0);
	newPos.y = 2.0 * sin (.005 * (newPos.x*newPos.x   +  newPos.z * newPos.z) - fTime);

	gl_Position = mTransform * newPos;
}
	
	
	
	
