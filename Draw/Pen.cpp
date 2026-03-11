#include"Pen.h"
#include "glm/gtx/string_cast.hpp"
#include"EraseStroke.h"


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
				v.push_back(Vertex{ glm::vec2(vx, vy), glm::vec4(1.0f, 1.0f, 1.0f, (*color).alpha) });
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

void Pen::line(GLFWwindow* window, Camera camera, Layer* layer) {
	double curMouseX, curMouseY;
	glfwGetCursorPos(window, &curMouseX, &curMouseY);
	if (prevMouseX != NULL and (prevMouseX != curMouseX or prevMouseY != curMouseY)) {

		glm::vec2 p1 = layer -> getLayerRelativePosition(camera, prevMouseX, prevMouseY);
		glm::vec2 p2 = layer -> getLayerRelativePosition(camera, curMouseX, curMouseY);

		float x1 = p1.x;
		float x2 = p2.x;
		float y1 = p1.y;
		float y2 = p2.y;
		//std::cout << camera.Position.x << " " << camera.Position.y << " " << curMouseX << " " << curMouseY << " " << camera.width << " " << camera.height << " " << x1 << " " << y1 << "\n";
		std::cout << x1 << " " << y1 << "\n";
		//testMatrix(camera, x2, y2);

		float dx = x2 - x1;
		float dy = y2 - y1;
		float len = sqrt(dx * dx + dy * dy);

		layer -> printPoint(x2 - dy / len * radius, y2 + dx / len * radius);
		if (currentLine == nullptr) {
			currentLine = std::make_unique<LineStroke>();
		}
		currentLine -> vertices.push_back(Vertex { glm::vec2(x2 - dy / len * radius, y2 + dx / len * radius), glm::vec4((*color).red, (*color).green, (*color).blue, (*color).alpha) });
		currentLine -> vertices.push_back(Vertex{ glm::vec2(x2 + dy / len * radius, y2 - dx / len * radius), glm::vec4((*color).red, (*color).green, (*color).blue, (*color).alpha) });
	}

	prevMouseX = curMouseX;
	prevMouseY = curMouseY;
}

void Pen::inputs(GLFWwindow* window, Camera camera, Layer* layer) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		held = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		line(window, camera, layer);
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		held = true;
		std::unique_ptr<Stroke> s = std::make_unique<EraseStroke>();
		std::vector<Vertex> v = createCircle(window, camera);
		if (v.size() > 0) {
			s->vertices = v;
			layer -> lines.push_back(std::move(s));
		}
	}
	else {
		if (held) {
			held = false;
			if (currentLine != nullptr) {
				layer -> lines.push_back(std::move(currentLine));
			}
			currentLine = nullptr;
		}
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		prevMouseX = NULL;
		prevMouseY = NULL;
	}
	
	// UNDO
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS and (*layer).lines.size() > 0)
	{
		double now = glfwGetTime();
		if (now >= undoNextTrigger) {
			redo.push_back(std::move(layer -> lines.back()));
			layer -> lines.pop_back();
			undoNextTrigger = now + undoDelay;
			undoDelay = std::max(minDelay, undoDelay - roc);
		}
	}
	else {
		undoDelay = maxDelay;
		undoNextTrigger = glfwGetTime();
	}

	// REDO
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS and redo.size() > 0)
	{
		double now = glfwGetTime();
		if (now >= redoNextTrigger) {
			layer->lines.push_back(std::move(redo.back()));
			redo.pop_back();
			redoNextTrigger = now + redoDelay;
			redoDelay = std::max(minDelay, redoDelay - roc);
		}
	}
	else {
		redoDelay = maxDelay;
		redoNextTrigger = glfwGetTime();
	}
}