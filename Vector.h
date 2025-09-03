#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	float x, y;
}vec2_t;

typedef struct {
	float x, y, z;
}vec3_t;

typedef struct {
	float x, y, z, w;
}vec4_t;

///////////////////////////////////////////
/// Vector 2 Functions
///////////////////////////////////////////

float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t a, vec2_t b);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_mul(vec2_t v, float factor);
vec2_t vec2_div(vec2_t v, float factor);
float vec2_dot(vec2_t a, vec2_t b);
void vec2_normalize(vec2_t* v);

///////////////////////////////////////////
/// Vector 3 Functions
///////////////////////////////////////////

float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_mul(vec3_t v, float factor);
vec3_t vec3_div(vec3_t v, float factor);
float vec3_dot(vec3_t a, vec3_t b);
void vec3_normalize(vec3_t* v);

vec3_t vec3_cross(vec3_t a, vec3_t b);
vec3_t vec3RotateX(vec3_t v, float angle);
vec3_t vec3RotateY(vec3_t v, float angle);
vec3_t vec3RotateZ(vec3_t v, float angle);
vec2_t vec3_project(vec3_t point, float fovFactor);

///////////////////////////////////////////
/// Vector conversion Functions
///////////////////////////////////////////

vec4_t vec4FromVec3(vec3_t v);
vec3_t vec3FromVec4(vec4_t v);

#endif