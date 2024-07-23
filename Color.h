#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <math.h>

typedef struct {
	uint32_t color; 
}color_t;

color_t RGBAToColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
color_t HSLAToColor(float h, float s, float l, unsigned char a);

const char* RGBAcolorToString(color_t color);
const char* HSLAcolorToString(color_t color);

color_t blendNormal(color_t colorA, color_t colorB);

// TODO: More color blending modes: 
//color_t blendLinearMix(color_t colorA, color_t colorB);
//color_t blendDissolve(color_t colorA, color_t colorB);
//color_t blendMultiply(color_t colorA, color_t colorB);
//color_t blendScreen(color_t colorA, color_t colorB);
//color_t blendOverlay(color_t colorA, color_t colorB);

#endif