#ifndef STATS_CLASS_H
#define STATS_CLASS_H

#include <GLFW/glfw3.h>

class Stats
{
public:
	double prevTime;
	unsigned int counter;

	Stats();
	
	void showStats(GLFWwindow* window);
};
#endif