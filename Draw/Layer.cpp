#include "Layer.h"

#include "glm/gtx/string_cast.hpp"

Layer::Layer(int width, int height, glm::vec2 position)
{
	Layer::width = width;
	Layer::height = height;
	Position = glm::vec3(position.x, position.y, 1);
	update();
}

glm::vec2 Layer::getLayerRelativePosition(Camera camera, float curX, float curY) {
	float x = camera.Position.x + (curX - camera.width / 2.0f) / camera.scale - Position.x;
	float y = camera.Position.y - (curY - camera.height / 2.0f) / camera.scale - Position.y;
	return glm::vec2(x, y);
}

void Layer::update() {
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	//std::cout << glm::to_string(projection) << std::endl;
	projection = glm::ortho(
		-width / 2.0f, width / 2.0f,
		-height / 2.0f, height / 2.0f,
		-1.0f, 1.0f
	);
	//std::cout << glm::to_string(projection) << std::endl;
	layerMatrix = projection * view;
}

void Layer::matrix(Shader& shader, const char* uniform)
{
	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(layerMatrix));
}

void Layer::printPoint(float x, float y) {
	glm::vec4 a = layerMatrix * glm::vec4(x, y, 0.0f, 1.0f);
	std::cout << "Layer point normalized: " << a.x << " " << a.y << std::endl;
}