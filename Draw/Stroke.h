#ifndef STROKE_CLASS_H
#define STROKE_CLASS_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include"Vertex.h"
#include"Color.h"
#include"Camera.h"

class Stroke {

public:
	std::vector<Vertex> vertices;

	Stroke() = default;
	virtual ~Stroke() = default;
	virtual void draw(GLuint VBO);
};


#endif