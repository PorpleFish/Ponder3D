#include "Vector.h"
#include <math.h>

///////////////////////////////////////////
/// Vector 2D Function Implementations
///////////////////////////////////////////

float vec2_length(vec2_t v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
	vec2_t returnValue = { 
		.x = a.x + b.x, 
		.y = a.y + b.y 
	};
	return returnValue;
}
	
vec2_t vec2_sub(vec2_t a, vec2_t b) {
	vec2_t returnValue = {
	.x = a.x - b.x,
	.y = a.y - b.y
	}; 
	return returnValue;
}

vec2_t vec2_mul(vec2_t v, float factor) {
	vec2_t returnValue = {
		.x = v.x * factor,
		.y = v.y * factor
	};
	return returnValue;
}

vec2_t vec2_div(vec2_t v, float factor) {
	vec2_t returnValue = {
		.x = v.x / factor,
		.y = v.y / factor
	};
	return returnValue;
}

float vec2_dot(vec2_t a, vec2_t b) {
	return (a.x * b.x) + (a.y * b.y);
}

void vec2_normalize(vec2_t* v) {
	float length = sqrt(v->x + v->x * v->y + v->y);
	v->x /= length;
	v->y /= length;
	return;
}

///////////////////////////////////////////
/// Vector 3D Generic Function Implementations
///////////////////////////////////////////

float vec3_length(vec3_t v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t returnValue = {
	.x = a.x + b.x,
	.y = a.y + b.y, 
	.z = a.z + b.z
	}; 
	return returnValue;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t returnValue = {
	.x = a.x - b.x,
	.y = a.y - b.y, 
	.z = a.z - b.z
	}; 
	return returnValue;
}

vec3_t vec3_mul(vec3_t v, float factor) {
	vec3_t returnValue = {
		.x = v.x * factor,
		.y = v.y * factor, 
		.z = v.z * factor
	};
	return returnValue;
}

vec3_t vec3_div(vec3_t v, float factor) {
	vec3_t returnValue = {
		.x = v.x / factor,
		.y = v.y / factor,
		.z = v.z / factor
	};
	return returnValue;
}

float vec3_dot(vec3_t a, vec3_t b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_normalize(vec3_t* v) {
	float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x /= length;
	v->y /= length;
	v->z /= length;
	return;
}

///////////////////////////////////////////
/// Vector 3D Specific Function Implementations
///////////////////////////////////////////

vec3_t vec3_cross(vec3_t a, vec3_t b) {
	vec3_t returnValue = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return returnValue;
}

vec3_t vec3RotateX(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

vec3_t vec3RotateY(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y, 
		.z = v.x * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

vec3_t vec3RotateZ(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};
	return rotated_vector;
} 

vec2_t vec3_project(vec3_t point, float fovFactor) {
	vec2_t projectedPoint = {
		.x = (fovFactor * point.x) / point.z,
		.y = (fovFactor * point.y) / point.z
	};
	return projectedPoint;
}

///////////////////////////////////////////
/// Vector conversion Function Implementations
///////////////////////////////////////////

vec4_t vec4FromVec3(vec3_t v)
{
	vec4_t result = {v.x, v.y, v.z, 1.0};
	return result;
}

vec3_t vec3FromVec4(vec4_t v)
{
	vec3_t result = { v.x, v.y, v.z};
	return result;
}
