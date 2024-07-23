#include <stdio.h>
#include <stdlib.h>
#include "Mesh.h"
#include "Array.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning (disable : 4996)

mesh_t mesh = {
	.verts = NULL,
	.faces = NULL,
	.rotation = { 0, 0, 0 }
};

vec3_t cubeVertices[N_CUBE_VERTICES] = {
   {.x = -1, .y = -1, .z = -1},
   {.x = -1, .y = 1, .z = -1},
   {.x = 1, .y = 1, .z = -1},
   {.x = 1, .y = -1, .z = -1},
   {.x = 1, .y = 1, .z = 1},
   {.x = 1, .y = -1, .z = 1},
   {.x = -1, .y = 1, .z = 1},
   {.x = -1, .y = -1, .z = 1}
};

face_t cubeFaces[N_CUBE_FACES] = {
	// Front
	{.a = 1, .b = 2, .c = 3 },
	{.a = 1, .b = 3, .c = 4 },
	// Right
	{.a = 4, .b = 3, .c = 5 },
	{.a = 4, .b = 5, .c = 6 },
	// Back
	{.a = 6, .b = 5, .c = 7 },
	{.a = 6, .b = 7, .c = 8 },
	// Left
	{.a = 8, .b = 7, .c = 2 },
	{.a = 8, .b = 2, .c = 1 },
	// Top
	{.a = 2, .b = 7, .c = 5 },
	{.a = 2, .b = 5, .c = 3 },
	// Bottom
	{.a = 6, .b = 8, .c = 1 },
	{.a = 6, .b = 1, .c = 4 }
};

void loadCubeMeshData(void) {
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		vec3_t cubeVert = cubeVertices[i];
		array_push(mesh.verts, cubeVert);
	}

	for (int i = 0; i < N_CUBE_FACES; i++) {
		face_t cubeFace = cubeFaces[i];
		array_push(mesh.faces, cubeFace);
	}
}

void loadVert(char* line) {
	vec3_t nextVert = { .x = 0, .y = 0, .z = 0 };
	
	int index = 0;
	int count = 0;

	while (line[index] != '\n') {
		if (line[index] == ' ') {
			char numberToAdd[16] = { 0 };
			index += 1;
			int f_index = 0;

			while (line[index] != ' ' && line[index] != '\n') {
				numberToAdd[f_index] = line[index];
				index += 1;
				f_index += 1;
			}

			index -= 1;
			float val = atof(numberToAdd);

			if (count == 0) {
				nextVert.x = val;
			}
			else if (count == 1) {
				nextVert.y = val;
			}
			else if (count == 2) {
				nextVert.z = val;
			}

			count += 1;
		}
		
		index += 1;
	}
	array_push(mesh.verts, nextVert);
}

void loadFace(char* line) {
	face_t nextFace = { .a = 0, .b = 0, .c = 0 };

	int index = 0;
	int count = 0;

	while (line[index] != '\n') {
		if (line[index] == ' ') {
			char numberToConvert[16] = { 0 };

			int n_index = index + 1;
			int f_index = 0;

			while (line[n_index] != '/' && line[n_index] != ' '){
				numberToConvert[f_index] = line[n_index];
				n_index++;
				f_index++;
			}

			int valueToAdd = atoi(numberToConvert);

			if (count == 0) {
				nextFace.a = valueToAdd;
			}
			if (count == 1) {
				nextFace.b = valueToAdd;
			}
			if (count == 2) {
				nextFace.c = valueToAdd;
			}
			count += 1;
		}
		index += 1;
	}
	array_push(mesh.faces, nextFace);
}

bool loadObj(const char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	char ch[64];
	const unsigned maxLineLength = 64;
	char buffer[64];

	bool isVertIndex = false;
	bool shouldGoToNextVert = false;
	char* vertIndexBuffer = NULL;
	int currentFacePoint = 0;

	vec3_t nextVert;
	face_t nextFace = { .a = 0, .b = 0, .c = 0 };

	printf("[FILE IO]	Loading in OBJ: %s\n", fileName);

	if (fp == NULL) {
		return false;
	}

	while (fgets(buffer, maxLineLength, fp)) {
		if (buffer[0] == 'v' && buffer[1] == ' ') {
			loadVert(buffer);
		}
		if (buffer[0] == 'f') {
			loadFace(buffer);
		}
	}

	return true;
}