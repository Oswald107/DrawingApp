#ifndef LAYER_CLASS_H
#define LAYER_CLASS_H

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<vector>

#include"Camera.h"
#include"Vertex.h"
#include"Stroke.h"

class Layer
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	int width;
	int height;

	std::vector<std::unique_ptr<Stroke>> lines;
	//std::vector<Stroke*> lines;

	glm::mat4 layerMatrix = glm::mat4(1.0f);
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Camera constructor to set up initial values
	Layer(int width, int height, glm::vec2 position);

	glm::vec2 getLayerRelativePosition(Camera camera, float curX, float curY);

	// Exports the camera matrix to a shader
	void Matrix(Shader& shader, const char* uniform);
	void update();
	void printPoint(float x, float y);
};
#endif