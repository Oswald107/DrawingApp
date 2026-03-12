#ifndef PEN_CLASS_H
#define PEN_CLASS_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include"Vertex.h"
#include"Color.h"
#include"Camera.h"
#include"Layer.h"
#include"Stroke.h"
#include"LineStroke.h"

class Pen {

public:
	float prevMouseX;
	float prevMouseY;
	float radius = 5.0f;
	Color* color;
	bool held = false;
	std::unique_ptr<Stroke> currentLine;

	double minDelay = .02;
	double maxDelay = .3;
	double roc = .07;
	double undoDelay = maxDelay;
	double undoNextTrigger = glfwGetTime();
	double redoDelay = maxDelay;
	double redoNextTrigger = glfwGetTime();

	std::vector<std::vector<Vertex>> circles;
	std::vector<std::unique_ptr<Stroke>> redo;

	Pen(Color* c);
	//glm::vec2 screenPositionToWorldPosition(double x, double y, glm::vec3 cameraPosition);
	std::vector<Vertex> createCircle(GLFWwindow* window, Camera camera);
	void line(GLFWwindow* window, Camera camera, Layer* layer);
	void inputs(GLFWwindow* window, Camera camera, Layer* layer);
};


#endif