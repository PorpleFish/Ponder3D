#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

#include "Settings.h"
#include "array.h"
#include "Display.h"
#include "Vector.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Color.h"

///////////////////////////////////////////
/// Array of Tris to render
///////////////////////////////////////////

tri_t* trisToRender = NULL;

///////////////////////////////////////////
/// Variables for execution and game loop
///////////////////////////////////////////

settings_t settings;

vec3_t cameraPosition = { 0, 0, 0 };
float fovFactor = 640;

bool isRunning = false;
int previousFrameTime = 0;
color_t currentColor;

//DisplayState currentState = FlatWires;

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
	//loadObj("Assets/cube.obj");
	loadCubeMeshData();

	currentColor = HSVAToColor(24, 83, 87, 255);

	settings.showVerts		 = true;
	settings.showWireframe   = true;
	settings.showTris	     = true;
	settings.cull		     = cull_backface;
}

void update(void) {
	// Static frame rate:
	// while (!SDL_TICKS_PASSED(SDL_GetTicks(), previousFrameTime + FRAME_TARGET_TIME));
	previousFrameTime = SDL_GetTicks();

	// Initialize Triangle Array
	trisToRender = NULL;

	mesh.rotation.x += 0.05;
	mesh.rotation.y += 0.05;
	mesh.rotation.z += 0.05;

	mesh.scale.x -= 0.0;
	mesh.scale.y -= 0.0;
	mesh.scale.z -= 0.0;

	mesh.translation.x += 0.0;
	mesh.translation.y += 0.0;
	mesh.translation.z += 0.0;

	mat4_t mesh_transform = mat4_translate(mesh.scale, mesh.translation, mesh.rotation);

	int faceCount = array_length(mesh.faces);

	for (int i = 0; i < faceCount; i++) {
		face_t meshFace = mesh.faces[i]; // Set temporary face

		vec3_t faceVertices[3];
		faceVertices[0] = mesh.verts[meshFace.a - 1];
		faceVertices[1] = mesh.verts[meshFace.b - 1];
		faceVertices[2] = mesh.verts[meshFace.c - 1];

		vec4_t transformedVerts[3];

		for (int j = 0; j < 3; j++)
			transformedVerts[j] = mat4_mul_vec4(mesh_transform, vec4FromVec3(faceVertices[j]));

		// Get the vector between AB and AC
		vec3_t vecAB = vec3_sub(vec3FromVec4(transformedVerts[1]), vec3FromVec4(transformedVerts[0]));
		vec3_t vecAC = vec3_sub(vec3FromVec4(transformedVerts[2]), vec3FromVec4(transformedVerts[0]));
		vec3_normalize(&vecAB);
		vec3_normalize(&vecAC);

		// Get the normal of the current triangle, the perpendicular angle from the tri
		vec3_t vertexNormal = vec3_cross(vecAB, vecAC);

		// Normalize the face normal
		vec3_normalize(&vertexNormal);

		// Find the vector between point A and the camera
		vec3_t cameraRay = vec3_sub(cameraPosition, vec3FromVec4(transformedVerts[0]));

		// Find how aligned the camera is with the point the camera is facing
		float dotNormalCamera = vec3_dot(vertexNormal, cameraRay);

		// Skip over non-camera-facing faces
		if (dotNormalCamera < 0 && settings.cull == cull_backface) {
			continue;
		}

		vec2_t projectedPoints[3];

		// Loop all 3 vertices to perform projection: 
		for (int j = 0; j < 3; j++) {
			projectedPoints[j] = vec3_project(vec3FromVec4(transformedVerts[j]), fovFactor);
			projectedPoints[j].x += ( windowWidth  / 2 );
			projectedPoints[j].y += ( windowHeight / 2 );
		}

		tri_t projectedTri = {
			.points = {
				{projectedPoints[0].x, projectedPoints[0].y},
				{projectedPoints[1].x, projectedPoints[1].y},
				{projectedPoints[2].x, projectedPoints[2].y}
			},
			.color = meshFace.color,
			.depth = (transformedVerts[0].z + transformedVerts[1].z + transformedVerts[2].z) / 3
		};
		array_push(trisToRender, projectedTri);
	}
	int triCount = array_length(trisToRender);
	for (int i = 0; i < triCount; i++) {
		for (int j = 0; j < triCount; j++) {
			if (trisToRender) {
				if (trisToRender[i].depth > trisToRender[j].depth) {
					tri_t temp = trisToRender[i];
					trisToRender[i] = trisToRender[j];
					trisToRender[j] = temp;
				}
			}
		}
	}
}

void render(void) {
	drawGrid(10);

	// Loop all projected triangles and render them
	int triCount = array_length(trisToRender);

	for (int i = 0; i < triCount; i++) {
		tri_t triangle = trisToRender[i];

		if (settings.showTris) { 
			drawTri_flat(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				triangle.color.color);
		}

		if (settings.showWireframe) {
			drawTri(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				0xFFE00000);
		}

		if (settings.showVerts) {
			drawRect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, currentColor.color);
			drawRect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, currentColor.color);
			drawRect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, currentColor.color);
		}
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

	if (event.type == SDL_EVENT_QUIT)isRunning = false;
	if (event.type != SDL_EVENT_KEY_DOWN) return;

	switch (event.key.key) {
	case SDLK_ESCAPE:
		isRunning = false;
		break;
	case SDLK_1:
		settings.showVerts = !settings.showVerts;
		break;
	case SDLK_2:
		settings.showWireframe = !settings.showWireframe;
		break;
	case SDLK_3:
		settings.showTris = !settings.showTris;
		break;
	case SDLK_4:
		settings.cull = (settings.cull + 1) % 2;
		break;
	default:
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

	while (isRunning) {
		processInput();
		update();
		render();
	}
	destroyWindow();
	freeResources();
	return 0;

}
