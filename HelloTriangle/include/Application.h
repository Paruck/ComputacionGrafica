#pragma once

#include <iostream>
#include "Object3D.h"

class Application {
public:
	Application();
	~Application();

	void setup();
	void display();
	void reshape(int w, int h);
	void update();


private:
	GLfloat angle;
	GLuint angleID;
	Object3D triangle;
};

