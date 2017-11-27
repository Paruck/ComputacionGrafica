
#include "Application.h"
#include <iostream>
#include <vector>
#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ShaderFuncs.h"
#include "SOIL.h"

#define __NUM_VIEWS_ 2

Application::Application():oPlane() , views(0),
							mouseRotation(glm::mat4(1.0f))
{
	eye = glm::vec3(0.0f, 50.0f, 100.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	time = 0;
}

Application::~Application() {}

void Application::update(){
	time += 5.0;

	lookAt = glm::lookAt(eye, target, up);
	perspective = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 200.0f);
	transform = perspective * lookAt * mouseRotation;
	glUniformMatrix4fv(oPlane.uTransform, 1, false, glm::value_ptr(transform));

	//parametro de fase para shaders
	glUniform1f(oPlane.uTime, glm::radians(time));
}

GLuint Application::setupTexture(const std::string& name) 
{
	int width, height, channels;
	unsigned char* img = SOIL_load_image(name.c_str(), &width, &height, &channels, SOIL_LOAD_RGB);

	GLuint texID = 0;
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	
	SOIL_free_image_data(img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void Application::setup(){
	oPlane.createPlane(100);
	oPlane.texture_id[0] = setupTexture("Map.png");
	//oPlane.texture_id[1] = setupTexture("Azul.png");


	
	std::string strVertexShader = loadTextFile("Shaders/passThru.v");
	std::string strFragmentShader = loadTextFile("Shaders/passThru.f");
	InitializeProgram(oPlane.shader, strVertexShader, strFragmentShader);
	oPlane.uTransform = glGetUniformLocation(oPlane.shader, "mTransform");
	oPlane.uTime = glGetUniformLocation(oPlane.shader, "fTime");
	//oPlane.uTexture = glGetUniformLocation(oPlane.shader, "texture0");

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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(oPlane.getVertexSizeInBytes()));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnable(GL_DEPTH_TEST);
}

void Application::displayPerspective() 
{
	//std::cout << "PERSPECTIVE\n";
	//Borramos el buffer de color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Seleccionamos los shaders a usar
	glUseProgram(oPlane.shader);	
	
	//activamos el vertex array object y dibujamos
	glBindVertexArray(oPlane.vao);

	glBindTexture(GL_TEXTURE_2D, oPlane.texture_id[0]);
	glUniform1i(oPlane.uTexture[0], 0);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, oPlane.texture_id[1]);
	glUniform1i(oPlane.uTexture[1], 1);
	glActiveTexture(GL_TEXTURE1);

	
	//glUniform3fv(oPlane.uEye, 1, glm::value_ptr(eye));
	glDrawArrays(GL_TRIANGLES, 0, oPlane.getNumVertex());
}

void Application::displayOrthogonal()
{
	//std::cout << "ORTHOGONAL\n";
}

void Application::display()
{
	switch (views)
	{
	case 0:
		displayPerspective(); break;
	case 1: 
		displayOrthogonal(); break;
	}
}

void Application::reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void Application::keyboard(int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		exit(0);
	}
	

	if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
		views = (++views ) % __NUM_VIEWS_;
	}
}

void Application::mousePosition(double xpos, double ypos)
{
	std::cout << xpos << ", " << ypos << "\n";
	mouseRotation = glm::rotate(glm::mat4(1.0f), (float)glm::radians(xpos), glm::vec3(0.0f, 1.0f, 0.0f));
}