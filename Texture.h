#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

typedef struct { // Stores the index of the verts in the mesh array
	float u;
	float v;
} tex2_t;

extern int texture_width;
extern int texture_height;

extern const uint8_t DEBUG_TEXTURE[];

extern uint32_t* current_texture;

#endif