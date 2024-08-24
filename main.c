#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "array.h"
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"
#include "Color.h"

///////////////////////////////////////////
/// Array of Tris to render
///////////////////////////////////////////

tri_t* trisToRender = NULL;

///////////////////////////////////////////
/// Variables for execution and game loop
///////////////////////////////////////////

vec3_t cameraPosition = { 0, 0, 0 };
float fovFactor = 640;

bool isRunning = false;
int previousFrameTime = 0;
color_t currentColor;

///////////////////////////////////////////
/// Initialize variables and game objects
///////////////////////////////////////////

void setup(void) {
	// Allocate required memory in bytes to hold the color buffer
	colorBuffer = (uint32_t*) malloc(sizeof(uint32_t) * windowWidth * windowHeight);

	// Create an SDL texture to display the color buffer
	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		windowWidth,
		windowHeight
	);
	loadObj("Assets/Cube.obj");

	printf("[MAIN]	Vert count: %d\n", array_length(mesh.verts));
	currentColor = HSVAToColor(24, 83, 87, 255);
}

void update(void) {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), previousFrameTime + FRAME_TARGET_TIME));
	previousFrameTime = SDL_GetTicks();

	// Initialize Triangle Array
	trisToRender = NULL;

	mesh.rotation.x += 0.008;
	mesh.rotation.y += 0.008;
	mesh.rotation.z += 0.008;

	int faceCount = array_length(mesh.faces);

	for (int i = 0; i < faceCount; i++) {
		face_t meshFace = mesh.faces[i]; // Set temporary face

		vec3_t faceVertices[3];
		faceVertices[0] = mesh.verts[meshFace.a - 1];
		faceVertices[1] = mesh.verts[meshFace.b - 1];
		faceVertices[2] = mesh.verts[meshFace.c - 1];

		tri_t projectedTri;

		vec3_t transformedVerts[3];

		for (int j = 0; j < 3; j++) {
			vec3_t transformedVertex = faceVertices[j];

			// Apply rotations for this frame
			transformedVertex = vec3RotateX(transformedVertex, mesh.rotation.x);
			transformedVertex = vec3RotateY(transformedVertex, mesh.rotation.y);
			transformedVertex = vec3RotateZ(transformedVertex, mesh.rotation.z);

			// Translate vertex away from camera
			transformedVertex.z += 5;

			transformedVerts[j] = transformedVertex;
		}

		//A = 0   A  
		//B = 1  / \ 
		//C = 2 C - B

		// Get the vector between AB and AC
		vec3_t vecAB = vec3_sub(transformedVerts[1], transformedVerts[0]);
		vec3_t vecAC = vec3_sub(transformedVerts[2], transformedVerts[0]);
		vec3_normalize(&vecAB);
		vec3_normalize(&vecAC);

		// Get the normal of the current triangle, the perpendicular angle from the tri
		vec3_t vertexNormal = vec3_cross(vecAB, vecAC);

		// Normalize the face normal
		vec3_normalize(&vertexNormal);

		// Find the vector between point A and the camera
		vec3_t cameraRay = vec3_sub(cameraPosition, transformedVerts[0]);

		// Find how aligned the camera is with the point the camera is facing
		float dotNormalCamera = vec3_dot(vertexNormal, cameraRay);

		// Skip over non-camera-facing faces
		if (dotNormalCamera < 0) {
			continue;
		}

		// Loop all 3 vertices to perform projection: 
		for (int j = 0; j < 3; j++) {
			vec2_t projectedPoint = vec3_project(transformedVerts[j], fovFactor);

			projectedPoint.x += ( windowWidth  / 2 );
			projectedPoint.y += ( windowHeight / 2 );

			projectedTri.points[j] = projectedPoint;
		}
		array_push(trisToRender, projectedTri);
	}
}

void render(void) {
	drawGrid(10);

	// Loop all projected triangles and render them
	int triCount = array_length(trisToRender);

	//for (int i = 0; i < triCount; i++) {
	//	tri_t triangle = trisToRender[i];
	//	drawRect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, currentColor.color);
	//	drawRect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, currentColor.color);
	//	drawRect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, currentColor.color);

	//	drawTri(
	//		triangle.points[0].x, triangle.points[0].y, 
	//		triangle.points[1].x, triangle.points[1].y, 
	//		triangle.points[2].x, triangle.points[2].y, 
	//		0xFFE0CA3C);
	//}

	drawTri(300, 100, 50, 400, 500, 700, 0xFFFF0000);
	drawTri_flat(300, 100, 50, 400, 500, 700, 0xFF000000);

	// Clear array of triangles to render here
	array_free(trisToRender);

	renderColorBuffer();
	clearColorBuffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

void processInput(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			isRunning = false;
		break;
	}
}

void freeResources(void) {
	array_free(mesh.faces);
	array_free(mesh.verts);
	free(colorBuffer);
}

int main(int argc, char* args[]) {

	isRunning = initializeWindow();

	setup();

	vec3_t myVector = { 2.0, 3.0, -4.0 };

	while (isRunning) {
		processInput();
		update();
		render();
	}
	destroyWindow();
	freeResources();
	return 0;
}