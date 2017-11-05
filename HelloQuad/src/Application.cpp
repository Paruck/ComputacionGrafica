
#include "Application.h"
#include <iostream>
#include <vector>

#include "GL\glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "ShaderFuncs.h"

GLfloat vertexPositions[] = {
	 1.0f, 1.0f, -0.0f, 1.0f,
	-1.0f, -1.0f, -0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f,

	-1.0f, 1.0f, -0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, -0.0f, 1.0f,

};


GLfloat vertexColors[] = {

	1.0f, 0.0f, 0.0f, 0.5f,
	1.0f, 0.0f, 0.0f, 0.5f,
	1.0f, 0.0f, 0.0f, 0.5f,
	1.0f, 0.0f, 0.0f, 0.5f,
	1.0f, 0.0f, 0.0f, 0.5f,
	1.0f, 0.0f, 0.0f, 0.5f,
};


Application::Application() : eye(0.0f, 0.0f, 6.0f), 
							target (0.0f, 0.0f, 0.0f),
							transform(glm::mat4(1.0f)), 
							transform2(glm::mat4(1.0f)),
							Rotacion(glm::mat4(1.0f)),
							angles(1.0f, 0.0f, 0.0f)
{}

Application::~Application() 
{}

void Application::update()
{
	if(flagX == true)
		angles.x += 0.1f;
	if(flagY)
		angles.y += 0.1f;
	if(flagZ)
		angles.z += 0.1f;

	camera = glm::lookAt(eye, target, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	transform = glm::perspective(45.0f, 1024.0f/758.0f , 0.1f, 10.0f) * 
				camera * transform;

}

void Application::keyboard(int key, int scancode, int actions, int mods)
{
	if (actions == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;

		case GLFW_KEY_X:
			flagX = true;
			break;

		case GLFW_KEY_Y:
			flagY = true;
			break;

		case GLFW_KEY_Z:
			flagZ = true;
			break;
		}
	}
	else if (actions == GLFW_RELEASE) {
		switch (key)
		{
		case GLFW_KEY_X:
			flagX = false;
			break;

		case GLFW_KEY_Y:
			flagY = false;
			break;

		case GLFW_KEY_Z:
			flagZ = false;
			break;
		}
	}
}

void Application::setup()
{
	std::string strVertexShader = loadTextFile("Shaders\\vertex.vs");
	std::string strFragmentShader = loadTextFile("Shaders\\fragment.fs");

	InitializeProgram(triangle.shader, strVertexShader, strFragmentShader);
	idTransform = glGetUniformLocation(triangle.shader, "transform");

	triangle.numVertex = 6;
	glGenVertexArrays(1, &triangle.vao);
	glBindVertexArray(triangle.vao);
	glGenBuffers(1, &triangle.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions)+sizeof(vertexColors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0/*offset*/, sizeof(vertexPositions), vertexPositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexPositions), sizeof(vertexColors), vertexColors);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vertexPositions));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

}


void Application::display()
{
	//Borramos el buffer de color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Seleccionamos los shaders a usar
	glUseProgram(triangle.shader);	
	glBindVertexArray(triangle.vao);

	glUniformMatrix4fv(idTransform, 1, GL_FALSE, glm::value_ptr(transform));
	glDrawArrays(GL_TRIANGLES, 0, triangle.numVertex);
	
	/*glUniformMatrix4fv(idTransform, 1, GL_FALSE, glm::value_ptr(transform2));
	glDrawArrays(GL_TRIANGLES, 0, triangle.numVertex);*/

	//activamos el vertex array object y dibujamos

}

void Application::reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}
