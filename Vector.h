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



// TODO add functions to manip 2D vectors
// TODO add functions to manip 3D vectors

vec3_t vec3RotateX(vec3_t v, float angle);
vec3_t vec3RotateY(vec3_t v, float angle);
vec3_t vec3RotateZ(vec3_t v, float angle);

#endif
