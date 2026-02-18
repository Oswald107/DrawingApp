#ifndef PEN_CLASS_H
#define PEN_CLASS_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include"Vertex.h"
#include"Color.h"
#include"Camera.h"

class Pen {

public:
	float prevMouseX;
	float prevMouseY;
	float radius = 5.0f;
	Color* color;

	Pen(Color* c);
	//glm::vec2 screenPositionToWorldPosition(double x, double y, glm::vec3 cameraPosition);
	std::vector<Vertex> createCircle(GLFWwindow* window, Camera camera);
};


#endif