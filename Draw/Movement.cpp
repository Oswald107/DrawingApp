#include"Movement.h"
#include <vector>
#include <cmath>
#include <iostream>



Movement::Movement(glm::vec3 position)
{
	Position = position;
}

//void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
//{
//	// Initializes matrices since otherwise they will be the null matrix
//	glm::mat4 view = glm::mat4(1.0f);
//	glm::mat4 projection = glm::mat4(1.0f);
//
//	// Makes camera look in the right direction from the right position
//	view = glm::lookAt(Position, Position + Orientation, Up);
//	// Adds perspective to the scene
//	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
//
//	// Sets new camera matrix
//	cameraMatrix = projection * view;
//}
//
//void Camera::Matrix(Shader& shader, const char* uniform)
//{
//	// Exports camera matrix
//	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
//}



void Movement::Inputs(GLFWwindow* window, std::vector<std::vector<float>> circles)
{
	//// Handles key inputs
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//{
	//	Position += speed * Orientation;
	//}
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//{
	//	Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	//}
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//{
	//	Position += speed * -Orientation;
	//}
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//{
	//	Position += speed * glm::normalize(glm::cross(Orientation, Up));
	//}
	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	//{
	//	Position += speed * Up;
	//}
	//if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	//{
	//	Position += speed * -Up;
	//}
	//if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	//{
	//	speed = 0.4f;
	//}
	//else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	//{
	//	speed = 0.1f;
	//}


	// Handles mouse inputs
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	//{
	//	// Hides mouse cursor
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//	// Stores the coordinates of the cursor
	//	double mouseX;
	//	double mouseY;
	//	// Fetches the coordinates of the cursor
	//	glfwGetCursorPos(window, &mouseX, &mouseY);

	//	std::vector<float> vertices = drawCircle(mouseX, mouseY, 10, 5);
	//}
}

std::vector<float> drawCircle(float x, float y, float radius, int sides) {
	std::vector<float> vertices;
	for (int i = 0; i < sides; ++i) {
		float theta = 2.0f * 3.1415926f * float(i) / float(sides);
		float vx = x + radius * cosf(theta);
		float vy = y + radius * sinf(theta);
		vertices.push_back(vx);
		vertices.push_back(vy);
	}
	//std::cout << "First vertex coordinates:" << std::endl;
	//std::cout << "(" << vertices[0] << ", " << vertices[1] << ")" << std::endl;

	return vertices;
}