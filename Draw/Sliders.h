#ifndef SLIDERS_CLASS_H
#define SLIDERS_CLASS_H

#include "Camera.h"
#include "Color.h"
#include "Pen.h"

class Sliders {
public:
	Sliders(GLFWwindow* window);

	void addSliders(Color* color, Pen* pen, Camera* camera);
};

#endif