
#include "Application.h"
#include <iostream>
#include <vector>
#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ShaderFuncs.h"

Application::Application():oPlane(), rotateY(glm::mat4(1.0f)) {
	eye = glm::vec3(0.0f, 50.0f, 100.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	time = 0;
}

Application::~Application() {}

void Application::setup(){
	oPlane.createPlane(100);

	std::string strVertexShader = loadTextFile("Shaders/passThru.v");
	std::string strFragmentShader = loadTextFile("Shaders/passThru.f");
	InitializeProgram(oPlane.shader, strVertexShader, strFragmentShader);

	oPlane.uTransform = glGetUniformLocation(oPlane.shader, "mTransform");
	oPlane.uTime = glGetUniformLocation(oPlane.shader, "fTime");

	//oPlane.uEye = glGetUniformLocation(oPlane.shader, "vEye");
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

void Application::update() {
	time += 1.0;

	lookAt = glm::lookAt(eye, target, up);
	perspective = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 200.0f);
	transform = perspective*lookAt * rotateY;


	
}

void Application::display(){
	//Borramos el buffer de color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Seleccionamos los shaders a usar
	glUseProgram(oPlane.shader);	

	//matriz de transformacion para variable uniform
	glUniformMatrix4fv(oPlane.uTransform, 1, false, glm::value_ptr(transform));

	//parametro de fase para shaders
	glUniform1f(oPlane.uTime, glm::radians(time));
	
	//activamos el vertex array object y dibujamos
	glBindVertexArray(oPlane.vao);
	
	//glUniform3fv(oPlane.uEye, 1, glm::value_ptr(eye));
	glDrawArrays(GL_TRIANGLES, 0, oPlane.getNumVertex());
}

void Application::reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void Application::keyboard(int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		exit(0);
	}
	if (action == GLFW_RELEASE) {
		return;
	}
}

void Application::cursor_position(double xpos, double ypos)
{
	if( xpos >= 512 && xpos <= 1024)
		rotateY = glm::rotate(glm::mat4(1.0f), glm::radians((float)xpos / 4), glm::vec3(0.0f, 1.0f, 0.0f));
	else if(ypos >= 384 && ypos <= 768)
		rotateY = glm::rotate(glm::mat4(1.0f), glm::radians((float)ypos / 2), glm::vec3(1.0f, 0.0f, 0.0f));
}
