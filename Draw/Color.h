#ifndef COLOR_CLASS_H
#define COLOR_CLASS_H

class Color
{
public:
	float red;
	float green;
	float blue;
	float hue;
	float saturation;
	float value;
	float alpha;

	Color();

	void recalculateHue();
	void recalculateSaturation();
	void recalculateValue();

};

#endif
