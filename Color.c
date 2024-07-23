#include "Color.h"

color_t RGBAToColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
	color_t color = { 0x00000000 };

	unsigned int red = (unsigned int)r << 24;
	unsigned int green = (unsigned int)g << 16;
	unsigned int blue = (unsigned int)b << 8;
	unsigned int alpha = (unsigned int)a;

	color.color = color.color | red;
	color.color = color.color | green;
	color.color = color.color | blue;
	color.color = color.color | alpha;

	return color;
}

float calculateHslX(float c, float hue) {
	float X;

	//X = fmod((hue / 60), 2);
	//X = abs(X - 1);
	//X = 1 - X;
	//X = c * X;

	X = (c * (1.0f - abs(hue / 60.0f)*fmod(2.0f, 1.0f)));
	return X / 2;
}

color_t HSLAToColor(float h, float s, float l, unsigned char a) {
	// Hue is represented as a rotation around the perimeter
	// So floor this to below 360! 
	float hue = fmod(h, 360.0f);
	float saturation = s / 100.0f;
	float luminance = l / 100.0f;

	float c = ((1.0f - abs(2.0f * luminance - 1.0f)) * saturation);
	float x = calculateHslX(c, hue); /*(c * (1.0f - abs(fmod((hue / 60.0f), 2.0f) - 1.0f)));*/
	float m = (luminance - c / 2.0f);

	// Initialize the RGB values to set here
	float r = 0;
	float g = 0;
	float b = 0;

	// Hue determines which 1/6th of the color wheel we're in 
	// Using this we can find which values to use for RGB! 
	if (hue > 0.0f && hue < 60.0f) {
		r = c;
		g = x;
		b = 0;
	}
	else if (hue >= 60.0f && hue <= 120.0f) {
		r = x;
		g = c;
		b = 0;
	}
	else if (hue >= 120.0f && hue <= 180.0f) {
		r = 0;
		g = c;
		b = x;
	}
	else if (hue >= 180.0f && hue <= 240.0f) {
		r = 0;
		g = x;
		b = c;
	}
	else if (hue >= 240.0f && hue <= 300.0f) {
		r = x;
		g = 0;
		b = c;
	}
	else if (hue >= 300.0f && hue <= 360.0f) {
		r = c;
		g = 0;
		b = x;
	}

	r = (r + m) * 255;
	g = (g + m) * 255;
	b = (b + m) * 255;

	color_t returnColor = RGBAToColor(
		(unsigned char)r,
		(unsigned char)g,
		(unsigned char)b,
		a);

	printf("[COLOR]	R = %u\n[COLOR]	G = %u\n[COLOR]	B = %u\n", 
		(unsigned char)r,
		(unsigned char)g,
		(unsigned char)b
	);

	printf("%f\n", c);

	return returnColor;
}