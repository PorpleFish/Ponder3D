#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <math.h>

typedef struct {
	uint32_t color; 
}color_t;

color_t RGBAToColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
color_t HSVAToColor(float h, float s, float l, unsigned char a);

void RGBAcolorToString(color_t color);
const char* HSVAcolorToString(color_t color);

signed char get_r(color_t in);
signed char get_g(color_t in);
signed char get_b(color_t in);
signed char get_a(color_t in);

color_t blendNormal(color_t colorA, color_t colorB);

// TODO: More color blending modes: 
//color_t blendLinearMix(color_t colorA, color_t colorB);
//color_t blendDissolve(color_t colorA, color_t colorB);
//color_t blendMultiply(color_t colorA, color_t colorB);
//color_t blendScreen(color_t colorA, color_t colorB);
//color_t blendOverlay(color_t colorA, color_t colorB);

#endif