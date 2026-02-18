#include"Camera.h"
#include "glm/gtx/string_cast.hpp"


Camera::Camera(int width, int height, glm::vec2 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = glm::vec3(position.x, position.y, 1);

	update();
}

void Camera::updateDimensions(int w, int h, float s) {
	if (w == width and h == height and s == scale)
	{
		return;;
	}
	
	width = w;
	height = h;
	scale = s;

	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Sets transformation between screen with bounds 2x2 (in the range -1 to 1) and the drawing space (I set it to follow the resolution so like 1920 X 1080 at fullscreen from range -960 to 960 ...) 
	projection[0][0] = scale * 2.0f / width;
	projection[1][1] = scale * 2.0f / height;
	projection[2][2] = -1.0f;
	projection[3][3] = 1.0f;

	cameraMatrix = projection * view;
}

void Camera::update() {
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Sets transformation between screen with bounds 2x2 (in the range -1 to 1) and the drawing space (I set it to follow the resolution so like 1920 X 1080 at fullscreen from range -960 to 960 ...) 
	projection[0][0] = scale * 2.0f / width;
	projection[1][1] = scale * 2.0f / height;
	projection[2][2] = -1.0f;
	projection[3][3] = 1.0f;

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::printMat()
{
	std::cout << glm::to_string(cameraMatrix) << std::endl;
}


void Camera::Inputs(GLFWwindow* window)
{
	float speed = 1.0f;
	bool shouldUpdateCamera = false;
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		std::cout << "Hello";
		Position += speed * glm::vec3(0, 1, 0);
		shouldUpdateCamera = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * glm::vec3(-1, 0, 0);
		shouldUpdateCamera = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * glm::vec3(0, -1, 0);
		shouldUpdateCamera = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::vec3(1, 0, 0);
		shouldUpdateCamera = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
	}
	//std::cout << Position.x << " " << Position.y << "\n";

	if (shouldUpdateCamera) {
		update();
	}

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{ /*
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float moveX = -sensitivity * .1 * (float)(mouseY - (height / 2)) / height;
		float moveY = -sensitivity * .1 * (float)(mouseX - (width / 2)) / width;

		glm::vec3 relativeSide = glm::normalize(glm::cross(Orientation, Up));
		glm::vec3 relativeUp = glm::normalize(glm::cross(Orientation, relativeSide));
		// Rotates the Orientation left and right
		Position += moveY * relativeSide;
		Position += moveX * relativeUp;

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true; */
	}
}