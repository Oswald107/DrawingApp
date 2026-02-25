#include"Pen.h"
#include "glm/gtx/string_cast.hpp"

Pen::Pen(Color* c) {
	prevMouseX = NULL;
	prevMouseY = NULL;
	color = c;
}

//glm::vec2 screenPositionToWorldPosition(double x, double y, glm::vec3 cameraPosition) {
//	return glm::vec2(cameraPosition.x + x, cameraPosition.y + y);
//}

std::vector<Vertex> Pen::createCircle(GLFWwindow* window, Camera camera) {
	std::vector<Vertex> v;

	double curMouseX, curMouseY;
	glfwGetCursorPos(window, &curMouseX, &curMouseY);

	if (prevMouseX != NULL) {
		float x1 = camera.Position.x + (prevMouseX - camera.width / 2.0f) / camera.scale;
		float x2 = camera.Position.x + (curMouseX - camera.width / 2.0f) / camera.scale;
		float y1 = camera.Position.y - (prevMouseY - camera.height / 2.0f) / camera.scale;
		float y2 = camera.Position.y - (curMouseY - camera.height / 2.0f) / camera.scale;

		//std::cout << camera.Position.x << " " << camera.Position.y << " " << curMouseX << " " << curMouseY << " " << camera.width << " " << camera.height << " " << x2 << " " << y2 << "\n";
		std::cout << curMouseX << " " << x2 << "\n";
		
		int circlesNeeded = std::max(static_cast<int>(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) / radius), 1);

		for (int c = 0; c < circlesNeeded; c++) {
			float t = static_cast<float>(c) / circlesNeeded;
			float x = x1 * (1 - t) + x2 * t;
			float y = y1 * (1 - t) + y2 * t;

			for (int i = 0; i < 16; ++i) {
				float theta = 2.0f * 3.1415926f * float(i) / float(16);
				float vx = x + radius * cosf(theta);
				float vy = y + radius * sinf(theta);
				v.push_back(Vertex{ glm::vec2(vx, vy), glm::vec4((*color).red, (*color).green, (*color).blue, (*color).alpha) });
			}
		}
	}
	
	prevMouseX = curMouseX;
	prevMouseY = curMouseY;
	return v;
}

void testMatrix(Camera camera, float x, float y) {
	std::cout << glm::to_string(camera.cameraMatrix * glm::vec4(x, y, 0.0f, 0.0f)) << "\n";
}

void Pen::line(GLFWwindow* window, Camera camera) {
	double curMouseX, curMouseY;
	glfwGetCursorPos(window, &curMouseX, &curMouseY);
	if (prevMouseX != NULL and (prevMouseX != curMouseX or prevMouseY != curMouseY)) {
		float x1 = camera.Position.x + (prevMouseX - camera.width / 2.0f) / camera.scale;
		float x2 = camera.Position.x + (curMouseX - camera.width / 2.0f) / camera.scale;
		float y1 = camera.Position.y - (prevMouseY - camera.height / 2.0f) / camera.scale;
		float y2 = camera.Position.y - (curMouseY - camera.height / 2.0f) / camera.scale;

		/*float x1 = prevMouseX - 400;
		float x2 = curMouseX - 400;
		float y1 = 400 - prevMouseY;
		float y2 = 400 - curMouseY;*/
		//std::cout << camera.Position.x << " " << camera.Position.y << " " << curMouseX << " " << curMouseY << " " << camera.width << " " << camera.height << " " << x1 << " " << y1 << "\n";
		//std::cout << x2 << " " << y2 << "\n";
		testMatrix(camera, x2, y2);

		float dx = x2 - x1;
		float dy = y2 - y1;
		float len = sqrt(dx * dx + dy * dy);

		currentLine.push_back(Vertex { glm::vec2(x2 - dy / len * radius, y2 + dx / len * radius), glm::vec4((*color).red, (*color).green, (*color).blue, (*color).alpha) });
		currentLine.push_back(Vertex{ glm::vec2(x2 + dy / len * radius, y2 - dx / len * radius), glm::vec4((*color).red, (*color).green, (*color).blue, (*color).alpha) });
	}

	prevMouseX = curMouseX;
	prevMouseY = curMouseY;
}

void Pen::Inputs(GLFWwindow* window, Camera camera) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		held = true;
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		//circles.push_back(pen.createCircle(window, camera));
		line(window, camera);
	}
	else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		held = true;
		erase.push_back(createCircle(window, camera));
	}
	else {
		if (held) {
			held = false;
			lines.push_back(currentLine);
			currentLine = std::vector<Vertex>{};
		}
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		prevMouseX = NULL;
		prevMouseY = NULL;
	}

	// UNDO
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS and lines.size() > 0)
	{
		redo.push_back(lines.back());
		lines.pop_back();
	}

	// REDO
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS and redo.size() > 0)
	{
		lines.push_back(redo.back());
		redo.pop_back();
	}
}