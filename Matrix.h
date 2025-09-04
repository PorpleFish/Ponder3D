#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"

typedef struct {
	float m[4][4];
}mat4_t;

mat4_t mat4_identity(void);

//mat4_t mat4_add(mat4_t a, mat4_t b);
//mat4_t mat4_sub(mat4_t a, mat4_t b);
//mat4_t mat4_mul(mat4_t a, mat4_t b);
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);

mat4_t mat4_rotat_x(float angle);
mat4_t mat4_rotat_y(float angle);
mat4_t mat4_rotat_z(float angle);

mat4_t mat4_scale(float sx, float sy, float sz);
mat4_t mat4_trans(float tx, float ty, float tz);
mat4_t mat4_rotat(float rx, float ry, float rz);

mat4_t mat4_scale_v(vec3_t s);
mat4_t mat4_trans_v(vec3_t t);
mat4_t mat4_rotat_v(vec3_t r);

mat4_t mat4_translate(vec3_t s, vec3_t t, vec3_t r);

mat4_t mat4_persp(float fov, float aspect, float znear, float zfar);
vec4_t mat4_project(mat4_t projection_matrix, vec4_t v);

#endif
