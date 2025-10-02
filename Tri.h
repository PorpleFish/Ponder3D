#ifndef TRI_H
#define TRI_H

#include "stdint.h"
#include "vector.h"
#include "Display.h"
#include "Color.h"
#include "Texture.h"

typedef struct { // Stores the index of the verts in the mesh array
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	color_t color;
} face_t;

typedef struct { // Stores the projected position of the vec2 points of the triangle on the screen
	vec2_t points[3];
	tex2_t texcoords[3];
	color_t color;
	float depth;
} tri_t;

void drawTri_flat(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void drawTri_textured(
	int x0, int y0, float u0, float v0,
	int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2,
	uint32_t* texture
	);

#endif