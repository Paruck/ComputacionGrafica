#pragma once

#include <iostream>
#include "Object3D.h"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"


class Application {
public:
	Application();
	~Application();

	GLFWwindow* window;

	void keyboard(int key, int scancode, int actions, int mods);
	void setup();
	void update();
	void display();
	void reshape(int w, int h);


private:
	bool flagX = false;
	bool flagY = false;
	bool flagZ = false;

	Object3D triangle;
	glm::mat4 camera;
	glm::vec3 eye, target, angles;
	glm::mat4	transform,
				transform2,
				Rotacion;

	GLuint idTransform;
};

