#include"Color.h"

#include<string>
// Constructor that generates a VAO ID
Color::Color()
{
	red = 1.0f;
	green = 0.8f;
	blue = 0.6f;
	hue = 1.0f;
	saturation = 1.0f;
	value = 1.0f;
	alpha = 1.0f;
}


void Color::recalculateHue() {
	float maxHue = 100.0f;
	int sections = 6;
	int currentSection = static_cast<int>(hue * sections / maxHue);
	switch (currentSection) {
	case 0:
		red = 1.0f;
		green = hue * sections / maxHue - currentSection;
		blue = 0.0f;
		break;
	case 1:
		red = 1.0 + currentSection - (hue * sections / maxHue);
		green = 1.0f;
		blue = 0.0f;
		break;
	case 2:
		red = 0.0f;
		green = 1.0f;
		blue = hue * sections / maxHue - currentSection;
		break;
	case 3:
		red = 0.0f;
		green = 1.0f + currentSection - (hue * sections / maxHue);
		blue = 1.0f;
		break;
	case 4:
		red = hue * sections / maxHue - currentSection;
		green = 0.0f;
		blue = 1.0f;
		break;
	case 5:
		red = 1.0f;
		green = 0.0f;
		blue = 1.0f + currentSection - (hue * sections / maxHue);
		break;

	}
}
void Color::recalculateSaturation() {
	float maxRGB = std::max(red, green);
	maxRGB = std::max(maxRGB, blue);
	float minRGB = std::min(red, green);
	minRGB = std::min(minRGB, blue);

	float diff = maxRGB * saturation / 100.0f;
	if (red == minRGB) {
		red = maxRGB - diff;
	}
	if (green == minRGB) {
		green = maxRGB - diff;
	}
	if (blue == minRGB) {
		blue = maxRGB - diff;
	}
}
void Color::recalculateValue() {
	float maxRGBBefore = std::max(red, green);
	maxRGBBefore = std::max(maxRGBBefore, blue);
	float maxRGBAfter = value / 100.0f;

	if (maxRGBBefore == 0.0f) {
		red = maxRGBAfter;
		green = maxRGBAfter;
		blue = maxRGBAfter;
	}
	else {
		red = red * maxRGBAfter / maxRGBBefore;
		green = green * maxRGBAfter / maxRGBBefore;
		blue = blue * maxRGBAfter / maxRGBBefore;
	}
}
