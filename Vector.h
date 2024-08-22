#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	float x;
	float y;
}vec2_t;

typedef struct {
	float x;
	float y;
	float z;
}vec3_t;

///////////////////////////////////////////
/// Vector 2D Functions
///////////////////////////////////////////

float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t v, float factor);
vec2_t vec2_div(vec2_t v, float factor);
float vec2_dot(vec2_t a, vec2_t b);

///////////////////////////////////////////
/// Vector 3D Functions
///////////////////////////////////////////

float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_mul(vec3_t v, float factor);
vec3_t vec3_div(vec3_t v, float factor);
float vec3_dot(vec3_t a, vec3_t b);

vec3_t vec3_cross(vec3_t a, vec3_t b);
vec3_t vec3RotateX(vec3_t v, float angle);
vec3_t vec3RotateY(vec3_t v, float angle);
vec3_t vec3RotateZ(vec3_t v, float angle);
vec2_t vec3_project(vec3_t point, float fovFactor);

#endif