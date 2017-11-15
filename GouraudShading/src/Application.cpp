
#include "Application.h"
#include <iostream>
#include <vector>
#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ShaderFuncs.h"

Application::Application():oPlane() {
	eye = glm::vec3(0.0f, 50.0f, 100.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	time = 0;
	myLightPosition = glm::vec3(0.0f, 50.0f, -100.0f);
	transform2 = glm::mat4(1.0);
}

Application::~Application() {}

void Application::update(){
	time += 0.5;

	if (flagX == true)
		myLightPosition.x += 0.1f;
	if (flagX2 == true)
		myLightPosition.x -= 0.1f;
	if (flagY)
		myLightPosition.z -= 0.1f;
	if (flagY2)
		myLightPosition.z += 0.1f;
}

void Application::setup(){
	oPlane.createPlane(100);
	crearCubo();

	std::string strVertexShader = loadTextFile("Shaders/gouradPlane.v");
	std::string strFragmentShader = loadTextFile("Shaders/passThru.f");
	InitializeProgram(oPlane.shader, strVertexShader, strFragmentShader);

	mTransformID = glGetUniformLocation(oPlane.shader, "mTransform");
	fTimeID = glGetUniformLocation(oPlane.shader, "fTime");
	eyeID = glGetUniformLocation(oPlane.shader, "vEye");
	myLightPositionID = glGetUniformLocation(oPlane.shader, "myLightPosition");
	cameraID = glGetUniformLocation(oPlane.shader, "camera");
	perspectiveID = glGetUniformLocation(oPlane.shader, "perspective");

	glGenVertexArrays(1, &oPlane.vao);
	glBindVertexArray(oPlane.vao);
	glGenBuffers(1, &oPlane.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, oPlane.vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, oPlane.getVertexSizeInBytes() + oPlane.getTextureCoordsSizeInBytes(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, oPlane.getVertexSizeInBytes(), oPlane.plane);
	glBufferSubData(GL_ARRAY_BUFFER, oPlane.getVertexSizeInBytes(), oPlane.getTextureCoordsSizeInBytes(), oPlane.textureCoords);
	oPlane.cleanMemory();

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(oPlane.getVertexSizeInBytes-

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnable(GL_DEPTH_TEST);
}

void Application::display() {
	//Borramos el buffer de color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Seleccionamos los shaders a usar
	glUseProgram(oPlane.shader);

	//activamos el vertex array object y dibujamos
	glBindVertexArray(oPlane.vao);
	lookAt = glm::lookAt(eye, target, up);
	perspective = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 200.0f);
	transform = glm::mat4(1.0f) * rotateY;
	glUniformMatrix4fv(mTransformID, 1, false, glm::value_ptr(transform));
	glUniformMatrix4fv(cameraID, 1, false, glm::value_ptr(lookAt));
	glUniformMatrix4fv(perspectiveID, 1, false, glm::value_ptr(perspective));


	//parametro de fase para shaders
	glUniform1f(fTimeID, glm::radians(time));
	glUniform3fv(eyeID, 1, glm::value_ptr(eye));
	newLight = myLightPosition;
	//newLight.z = myLightPosition.z * cos(glm::radians(time));
	glUniform3fv(myLightPositionID, 1, glm::value_ptr(newLight));


	//glUniform3fv(oPlane.uEye, 1, glm::value_ptr(eye));
	glDrawArrays(GL_TRIANGLES, 0, oPlane.getNumVertex());

	glUseProgram(cube.shader);
	glBindVertexArray(cube.vao);

	transform2 = glm::translate(perspective* lookAt, newLight);

	glUniformMatrix4fv(idTransform, 1, GL_FALSE, glm::value_ptr(transform2));
	glDrawArrays(GL_TRIANGLES, 0, cube.numVertex);


}

void Application::reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void Application::keyboard(int key, int scancode, int action, int mods){
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;

		case GLFW_KEY_RIGHT:
			flagX = true;
			break;

		case GLFW_KEY_UP:
			flagY = true;
			break;

		case GLFW_KEY_DOWN:
			flagY2 = true;
			break;
		case GLFW_KEY_LEFT:
			flagX2 = true;
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key)
		{
		case GLFW_KEY_RIGHT:
			flagX = false;
			break;

		case GLFW_KEY_UP:
			flagY = false;
			break;

		case GLFW_KEY_DOWN:
			flagY2 = false;
			break;
		case GLFW_KEY_LEFT:
			flagX2 = false;
			break;
		}
	}
}

void Application::crearCubo()
{
	GLfloat vertexPositions[] = {
		// 1.0f, 1.0f, -0.0f, 1.0f,
		//-1.0f, -1.0f, -0.0f, 1.0f,
		// 1.0f, -1.0f, 0.0f, 1.0f,

		//-1.0f, 1.0f, -0.0f, 1.0f,
		//-1.0f, -1.0f, 0.0f, 1.0f,
		// 1.0f, 1.0f, -0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f, 1.0f,
		1.0f, 1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f, 1.0f,

		1.0f,-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f,-1.0f, 1.0f,
		1.0f, 1.0f,-1.0f, 1.0f, // triángulo 2 : comienza
		-1.0f,-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, 1.0f,

		-1.0f,-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f, 1.0f, // triángulo 1 : comienza
		-1.0f,-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,

		//Cara Amarilla


		// triángulo 1 : termina
		// triángulo 2 : termina

		1.0f,-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f, 1.0f,


		-1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f, 1.0f,
		1.0f,-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f, 1.0f,

		1.0f,-1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f, 1.0f,
	};
	GLfloat vertexColors[] = {

		1.0f, 0.0f, 0.0f, 0.5f,
		1.0f, 0.0f, 0.0f, 0.5f,
		1.0f, 0.0f, 0.0f, 0.5f,
		0.0f, 0.0f, 1.0f, 0.5f,
		0.0f, 0.0f, 1.0f, 0.5f,
		0.0f, 0.0f, 1.0f, 0.5f,

		0.0f, 1.0f, 0.0f, 0.5f,
		0.0f, 1.0f, 0.0f, 0.5f,
		0.0f, 1.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 0.0f, 0.5f,

		1.0f, 0.35f, 0.0f, 0.5f,
		1.0f, 0.35f, 0.0f, 0.5f,
		1.0f, 0.35f, 0.0f, 0.5f,
		1.0f, 0.35f, 0.0f, 0.5f,
		1.0f, 0.35f, 0.0f, 0.5f,
		1.0f, 0.35f, 0.0f, 0.5f,



		0.0f, 1.0f, 0.0f, 0.5f,
		0.0f, 1.0f, 0.0f, 0.5f,
		0.0f, 1.0f, 0.0f, 0.5f,
		0.0f, 0.0f, 1.0f, 0.5f,
		0.0f, 0.0f, 1.0f, 0.5f,
		0.0f, 0.0f, 1.0f, 0.5f,



		1.0f, 1.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 0.0f, 0.5f,
		1.0f, 0.0f, 0.0f, 0.5f,
		1.0f, 0.0f, 0.0f, 0.5f,
		1.0f, 0.0f, 0.0f, 0.5f,


		1.0f, 1.0f, 1.0f, 0.5f,
		1.0f, 1.0f, 1.0f, 0.5f,
		1.0f, 1.0f, 1.0f, 0.5f,
		1.0f, 1.0f, 1.0f, 0.5f,
		1.0f, 1.0f, 1.0f, 0.5f,
		1.0f, 1.0f, 1.0f, 0.5f,



	};

	const std::string strVertexShader(
	"#version 430 core\n"

	"layout(location = 0) in vec4 vPosition;"
	"layout(location = 1) in vec4 vColor;"

	"out vec4 color;"
	"uniform float angle;"
	"uniform mat4 transform;"

	"void main()"
	"{"
		"color = vColor;"
		"gl_Position = transform * vPosition;"
	"}"
	);


	const std::string strFragmentShader(
		"#version 430 core\n"

		"out vec4 fragmentColor;"
		"in vec4 color;"

		"void main()"
		"{"
			"fragmentColor = color;"
		"}"
		);

	InitializeProgram(cube.shader, strVertexShader, strFragmentShader);
	idTransform = glGetUniformLocation(cube.shader, "transform");

	cube.numVertex = 36;
	glGenVertexArrays(1, &cube.vao);
	glBindVertexArray(cube.vao);
	glGenBuffers(1, &cube.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube.vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions) + sizeof(vertexColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0/*offset*/, sizeof(vertexPositions), vertexPositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexColors), vertexColors);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertexPositions));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


}

void Application::cursor_position(double xpos, double ypos)
{
	if (xpos >= 0 && xpos <= 1024 && ypos >= 0 && ypos <= 360)
		rotateY = glm::rotate(glm::mat4(1.0f), glm::radians((float)xpos / 2), glm::vec3(0.0f, 1.0f, 0.0f));
	else if (ypos >= 384 && ypos <= 768)
		rotateY = glm::rotate(glm::mat4(1.0f), glm::radians((float)ypos / 1), glm::vec3(1.0f, 0.0f, 0.0f));
}