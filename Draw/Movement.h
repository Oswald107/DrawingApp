#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <vector>

#include"shaderClass.h"

class Movement
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Camera constructor to set up initial values
	Movement(glm::vec3 position);

	// Handles camera inputs
	void Inputs(GLFWwindow* window, std::vector<std::vector<float>> circles);
};
#endif