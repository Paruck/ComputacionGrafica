#pragma once

#include <iostream>
#include "Object3D.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "Plane.h"
#include "GLFW\glfw3.h"

class Application {
public:
	Application();
	~Application();
	void update();
	void setup();
	void display();
	void reshape(int w, int h);
	void keyboard(int key, int scancode, int action, int mods);
	void cursor_position(double xpos, double ypos);

private:	
	Plane oPlane;
	float time;
	glm::mat4 transform, rotateY;
	glm::vec3 eye;
	glm::vec3 target;
	glm::vec3 up;
	glm::mat4 lookAt;
	glm::mat4 perspective;
	glm::mat4 newPosis;
	double x, y;
};

