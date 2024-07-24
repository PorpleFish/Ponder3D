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

	float range = luminance  * saturation;
	float maxRGB = luminance;
	float minRGB = maxRGB - range;
	// Find the position along the gradient we'll be using for the RGB
	float huePrime = hue / 60.0;
	float gradient_up = fmod(huePrime, 1.0);
	float gradient_down = 1.0 - fmod(huePrime, 1.0);

	float r = 0;
	float g = 0;
	float b = 0;

	// Hue determines which 1/6th of the color wheel we're in 
	// Using this we can find which values to use for RGB! 
	if ((huePrime >= 0) && (huePrime < 1)) {
		r = maxRGB;
		g = (gradient_up * range) + minRGB;
		b = minRGB;
	}
	else if ((huePrime >= 1) && (huePrime < 2)) {
		r = (gradient_down * range) + minRGB;
		g = maxRGB;
		b = minRGB;
	}
	else if ((huePrime >= 2) && (huePrime < 3)) {
		r = minRGB;
		g = maxRGB;
		b = (gradient_up * range) + minRGB;
	}
	else if ((huePrime >= 3) && (huePrime < 4)) {
		r = minRGB;
		g = (gradient_down * range) + minRGB;
		b = maxRGB;
	}
	else if ((huePrime >= 4) && (huePrime < 5)) {
		r = (gradient_up * range) + minRGB;
		g = minRGB;
		b = maxRGB;
	}
	else if ((huePrime >= 5) && (huePrime < 6)) {
		r = maxRGB;
		g = minRGB;
		b = (gradient_down * range) + minRGB;
	}

	r *= 255;
	g *= 255;
	b *= 255;

	color_t returnColor = RGBAToColor(
		(unsigned char)r,
		(unsigned char)g,
		(unsigned char)b,
		a);

	printf("[COLOR]	R = %f\n[COLOR]	G = %f\n[COLOR]	B = %f\n", r, g, b );

	return returnColor;
}