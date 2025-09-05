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
	.translation = { 0, 0, 0 },
	.rotation = { 0, 0, 0 },
	.scale = { 1.0, 1.0, 1.0 }
};

vec3_t cubeVertices[N_CUBE_VERTICES] = {
   {.x = -10, .y = -10, .z = -10},
   {.x = -10, .y = 10, .z = -10},
   {.x = 10, .y = 10, .z = -10},
   {.x = 10, .y = -10, .z = -10},
   {.x = 10, .y = 10, .z = 10},
   {.x = 10, .y = -10, .z = 10},
   {.x = -10, .y = 10, .z = 10},
   {.x = -10, .y = -10, .z = 10}
};

face_t cubeFaces[N_CUBE_FACES] = {
	// Front
	{.a = 1, .b = 2, .c = 3, .color = 0xFFFF0000},
	{.a = 1, .b = 3, .c = 4, .color = 0xFFFF0000 },
	// Right
	{.a = 4, .b = 3, .c = 5, .color = 0xFF00FF00 },
	{.a = 4, .b = 5, .c = 6, .color = 0xFF00FF00 },
	// Back
	{.a = 6, .b = 5, .c = 7, .color = 0xFF0000FF },
	{.a = 6, .b = 7, .c = 8, .color = 0xFF0000FF },
	// Left
	{.a = 8, .b = 7, .c = 2, .color = 0xFFFFFF00 },
	{.a = 8, .b = 2, .c = 1, .color = 0xFFFFFF00 },
	// Top
	{.a = 2, .b = 7, .c = 5, .color = 0xFFFF00FF },
	{.a = 2, .b = 5, .c = 3, .color = 0xFFFF00FF },
	// Bottom
	{.a = 6, .b = 8, .c = 1, .color = 0xFF00FFFF },
	{.a = 6, .b = 1, .c = 4, .color = 0xFF00FFFF }
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
	char buffer[1024];

	printf("[FILE IO]	Loading in OBJ: %s\n", fileName);

	if (fp == NULL) {
		return false;
	}

	while (fgets(buffer, 1024, fp)) {
		if (strncmp(buffer, "v ", 2) == 0) {
			vec3_t nextVert;
			sscanf(buffer, "v %f %f %f", &nextVert.x, &nextVert.y, &nextVert.z);
			array_push(mesh.verts, nextVert);
		}
		if (strncmp(buffer, "f ", 2) == 0) {
			int vertIndices[3];
			int textureCoordinates[3];
			int normalIndices[3];

			sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
				&vertIndices[0], &textureCoordinates[0], &normalIndices[0],
				&vertIndices[1], &textureCoordinates[1], &normalIndices[1], 
				&vertIndices[2], &textureCoordinates[2], &normalIndices[2]
			);

			face_t nextFace = {
			.a = vertIndices[0],
			.b = vertIndices[1],
			.c = vertIndices[2]
			};
			nextFace.color = RGBAToColor(255, 255, 255, 255);
			array_push(mesh.faces, nextFace);
		}

	}
	return true;
}