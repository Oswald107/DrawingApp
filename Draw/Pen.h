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
	bool held = false;
	std::vector<Vertex> currentLine = std::vector<Vertex>{};

	std::vector<std::vector<Vertex>> circles;
	std::vector<std::vector<Vertex>> lines;
	std::vector<std::vector<Vertex>> redo;
	std::vector<std::vector<Vertex>> erase;

	Pen(Color* c);
	//glm::vec2 screenPositionToWorldPosition(double x, double y, glm::vec3 cameraPosition);
	std::vector<Vertex> createCircle(GLFWwindow* window, Camera camera);
	void line(GLFWwindow* window, Camera camera);
	void Inputs(GLFWwindow* window, Camera camera);
};


#endif