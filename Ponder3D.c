#define _USE_MATH_DEFINES

#include <stdio.h>
#include <math.h>
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
#include "Light.h"
#include "Texture.h"
#include "Tri.h"

///////////////////////////////////////////
/// Array of Tris to render
///////////////////////////////////////////

tri_t* trisToRender = NULL;

///////////////////////////////////////////
/// Variables for execution and game loop
///////////////////////////////////////////

settings_t settings;

vec3_t cameraPosition = { 0, 0, 0 };
mat4_t proj_matrix;

bool isRunning = false;
int previousFrameTime = 0;
color_t currentColor;
light_t sun_light;

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

	float fov = M_PI / 3.0; // the same as 180/3, or 60deg
	float aspect = (float)windowHeight / (float)windowWidth;
	float znear = 0.1;
	float zfar = 100.0;
	proj_matrix = mat4_persp(fov, aspect, znear, zfar);

	// Set texture:
	current_texture = (int32_t*)DEBUG_TEXTURE;
	// TODO: Convert to .png

	// loadObj("Assets/Hornet.obj");
	loadCubeMeshData();

	currentColor = HSVAToColor(24, 83, 87, 255);

	sun_light.direction.x = 0 ;
	sun_light.direction.y = 1.0f;
	sun_light.direction.z = 1.0f;
	sun_light.intensity = 0.6f;

	vec3_normalize(&sun_light.direction);

	settings.showVerts		   = true;
	settings.showWireframe     = true;
	settings.showTris	       = true;
	settings.cull		       = cull_backface;
	settings.face_render_style = FLAT_TEXTURED;
}

void update(void) {
	// while (!SDL_TICKS_PASSED(SDL_GetTicks(), previousFrameTime + FRAME_TARGET_TIME));
	previousFrameTime = SDL_GetTicks();

	// Initialize Triangle Array
	trisToRender = NULL;

	mesh.rotation.y += 0.1;

	mesh.translation.y = -4;
	mesh.translation.z = 12.0;

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

		// Get the vector between AB and AC ?? NOTE: I changed the l (0,0) and r (1,2) values here
		vec3_t vecAB = vec3_sub(vec3FromVec4(transformedVerts[0]), vec3FromVec4(transformedVerts[1]));
		vec3_t vecAC = vec3_sub(vec3FromVec4(transformedVerts[0]), vec3FromVec4(transformedVerts[2]));
		vec3_normalize(&vecAB);
		vec3_normalize(&vecAC);

		// Get the normal of the current triangle, the perpendicular angle from the tri
		vec3_t vertexNormal = vec3_cross(vecAB, vecAC);
		vec3_normalize(&vertexNormal);

		// Find the vector between point A and the camera
		vec3_t cameraRay = vec3_sub(cameraPosition, vec3FromVec4(transformedVerts[0]));

		// Find how aligned the camera is with the point the camera is facing
		float dotNormalCamera = vec3_dot(vertexNormal, cameraRay);

		// Backface culling
		if (settings.cull)
		{
			// Skip over non-camera-facing faces
			if (dotNormalCamera < 0) {
				continue;
			}
		}
		
		vec4_t projectedPoints[3];

		// Perform projection: 
		for (int j = 0; j < 3; j++) {
			// ERROR: this func causes X to always be 0
			projectedPoints[j] = mat4_project(proj_matrix, transformedVerts[j]);

			// Scale into the view, and negate the Y
			projectedPoints[j].x *= (windowWidth / 2.0);
			projectedPoints[j].y *= -(windowHeight / 2.0);

			// Translate to the middle of the view
			projectedPoints[j].x += ( windowWidth  / 2.0 );
			projectedPoints[j].y += ( windowHeight / 2.0 );
		}

		float face_brightness = light_getFaceAlignment(vertexNormal, sun_light);
		face_brightness += 0.2;
		color_t tri_color; 
		tri_color.color = apply_light(meshFace.color, face_brightness);

		tri_t projectedTri = {
			.points = {
				{projectedPoints[0].x, projectedPoints[0].y},
				{projectedPoints[1].x, projectedPoints[1].y},
				{projectedPoints[2].x, projectedPoints[2].y}
			},
			.texcoords = {
				{meshFace.a_uv.u, meshFace.a_uv.v},
				{meshFace.b_uv.u, meshFace.b_uv.v},
				{meshFace.c_uv.u, meshFace.c_uv.v}
			},
			.color = tri_color,
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
			switch (settings.face_render_style) {
			case FLAT:
				drawTri_flat(
					triangle.points[0].x, triangle.points[0].y,
					triangle.points[1].x, triangle.points[1].y,
					triangle.points[2].x, triangle.points[2].y,
					triangle.color.color);
				break;
			case FLAT_TEXTURED:
				printf("[MAIN] No function exists for drawing textured tri\n");
				drawTri_textured(
					triangle.points[0].x, triangle.points[0].y, triangle.texcoords[0].u, triangle.texcoords[0].v,
					triangle.points[1].x, triangle.points[1].y, triangle.texcoords[1].u, triangle.texcoords[1].v,
					triangle.points[2].x, triangle.points[2].y, triangle.texcoords[2].u, triangle.texcoords[2].v,
					triangle.color.color);
				break;
			default:
				break;
			}
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
	case SDLK_5:
		if (settings.face_render_style == FLAT) 
			settings.face_render_style = FLAT_TEXTURED;
		else settings.face_render_style = FLAT;
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
