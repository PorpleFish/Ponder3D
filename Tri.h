#ifndef TRI_H
#define TRI_H

#include "vector.h"

typedef struct { // Stores the index of the verts in the mesh array
	int a;
	int b;
	int c;
} face_t;

typedef struct { // Stores the projected position of the vec2 points of the triangle on the screen
	vec2_t points[3];
} tri_t;

#endif