#ifndef MESH_H
#define MESH_H

#include "Vector.h"
#include "Tri.h"
#include <stdbool.h>

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6*2) // 6 cube faces with 2 tris each

extern vec3_t cubeVertices[N_CUBE_VERTICES];
extern face_t cubeFaces[N_CUBE_FACES];

typedef struct { // Stores an array for verts, faces and translation data for a single mesh
	vec3_t* verts; // Dynamic array of vertices
	face_t* faces; // Dynamic array of faces
	// vec3_t position;
	vec3_t rotation; // Currently Euler rotation
	// vec3_t scale;
} mesh_t;

extern mesh_t mesh;

void loadCubeMeshData(void);

bool loadObj(char* fileName);


#endif