#version 430 core

layout (location = 0) in vec4 vPosition;

uniform mat4 mTransform;
uniform mat4 camera;
uniform mat4 perspective;

uniform float fTime;
out vec4 observedPosition;

float F(float x, float z) 
{
	//return   2.0 * sin (.005 * (x*x   +  z*z) - fTime);

	return 2.0 * sin (.005 * (x*x + z*z) - fTime);
}

void main()
{
	vec4 vNewVec = vPosition;
	//calculamos la altura de este vertice
	vNewVec.y = F(vNewVec.x, vNewVec.z);

	//Esto va llegar interpolado
	observedPosition = vPosition;
	gl_Position = perspective * camera * mTransform * vNewVec;
}