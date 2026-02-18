#include"Stats.h"

#include<string>
// Constructor that generates a VAO ID
Stats::Stats()
{
	prevTime = 0.0;
	counter = 0;
}


void Stats::showStats(GLFWwindow* window) {
	double crntTime = glfwGetTime();
	double timeDiff = crntTime - prevTime;
	counter++;
	if (timeDiff >= 1.0 / 30.0)
	{
		std::string FPS = std::to_string((1.0 / timeDiff) * counter);
		std::string ms = std::to_string((timeDiff / counter) * 1000);
		std::string metricsHeader = "OpenGL App - " + FPS + "FPS / " + ms + "ms";
		glfwSetWindowTitle(window, metricsHeader.c_str());
		prevTime = crntTime;
	}
}
