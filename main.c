#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "array.h"
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"

///////////////////////////////////////////
/// Array of Tris to render
///////////////////////////////////////////

tri_t* trisToRender = NULL;

///////////////////////////////////////////
/// Variables for execution and game loop
///////////////////////////////////////////

vec3_t cameraPosition = { .x = 0, .y = 0, .z = -5 };
vec3_t cubeRotation = { .x = 0, .y = 0, .z = 0 };
float fovFactor = 1280;

bool isRunning = false;
int previousFrameTime = 0;

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
}

///////////////////////////////////////////
/// Function that turns a Vec3D into a Vec2D
///////////////////////////////////////////

vec2_t project(vec3_t point) {
	vec2_t projectedPoint = {
		.x = (fovFactor * point.x) / point.z,
		.y = (fovFactor * point.y) / point.z
	};
	return projectedPoint;
}

void update(void) {
	// Make a timeToWait variable
	int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - previousFrameTime);

	// Only delay if it's required
	if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
		// Delay here, OS dependant so it's a bit black-box! 
		SDL_Delay(timeToWait);
	}

	// Initialize Triangle Array
	trisToRender = NULL;

	// Rotate by 0.1 each frame
	cubeRotation.x += 0.01;
	cubeRotation.y += 0.01;
	cubeRotation.z += 0.01;

	// Loop through cube mesh faces
	for (int i = 0; i < N_MESH_FACES; i++) {
		face_t meshFace = meshFaces[i]; // Set temporary face
		
		vec3_t faceVertices[3];
		faceVertices[0] = meshVertices[meshFace.a - 1];
		faceVertices[1] = meshVertices[meshFace.b - 1];
		faceVertices[2] = meshVertices[meshFace.c - 1];

		tri_t projectedTri;

		// Loop all three vertices and apply transformations 
		for (int j = 0; j < 3; j++) {
			vec3_t transformedVertex = faceVertices[j];

			// Apply rotations for this frame
			transformedVertex = vec3RotateX(transformedVertex, cubeRotation.x);
			transformedVertex = vec3RotateY(transformedVertex, cubeRotation.y);
			transformedVertex = vec3RotateZ(transformedVertex, cubeRotation.z);

			// Translate vertex away from camera
			transformedVertex.z -= cameraPosition.z;

			// Project current vertex
			vec2_t projectedPoint = project(transformedVertex);

			// Scale and translate the projected points to the middle of the screen 
			projectedPoint.x += ( windowWidth  / 2 );
			projectedPoint.y += ( windowHeight / 2 );

			projectedTri.points[j] = projectedPoint;
		}
		// Save the projected triangle in the array of triangles to render.
		array_push(trisToRender, projectedTri);
	}
}

void render(void) {
	drawGrid(10);

	// Loop all projected triangles and render them
	int triCount = array_length(trisToRender);

	for (int i = 0; i < triCount; i++) {
		tri_t triangle = trisToRender[i];
		drawRect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFF9864FF);
		drawRect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFF9864FF);
		drawRect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFF9864FF);
		drawTri(
			triangle.points[0].x, triangle.points[0].y, 
			triangle.points[1].x, triangle.points[1].y, 
			triangle.points[2].x, triangle.points[2].y, 
			0xFFE0CA3C);
	}

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

	return 0;
}