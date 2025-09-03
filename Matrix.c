#include "Matrix.h"
#include <math.h>

mat4_t mat4_identity(void)
{
	mat4_t identity =
	{{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}};
	return identity;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v)
{
	vec4_t result;

	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
	result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

	return result;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b)
{
	mat4_t result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = a.m[i][0] * b.m[0][j];
			result.m[i][j] += a.m[i][1] * b.m[1][j];
			result.m[i][j] += a.m[i][2] * b.m[2][j];
			result.m[i][j] += a.m[i][3] * b.m[3][j];
		}
	}

	return result;
}

mat4_t mat4_rotat_x(float angle)
{
	mat4_t rot_x = mat4_identity();

	float c = cos(angle);
	float s = sin(angle);

	// |  1   0   0   0  | \\
	// |  0   c  -s   0  | \\
	// |  0   s   c   0  | \\
	// |  0   0   0   1  | \\

	rot_x.m[1][1] = c;
	rot_x.m[2][1] = -s;
	rot_x.m[1][2] = s;
	rot_x.m[2][2] = c;

	return rot_x;
}

mat4_t mat4_rotat_y(float angle)
{
	mat4_t rot_y = mat4_identity();

	float c = cos(angle);
	float s = sin(angle);

	// |  c   0   s   0  | \\
	// |  0   1   0   0  | \\
	// | -s   0   c   0  | \\
	// |  0   0   0   1  | \\

	rot_y.m[0][0] = c;
	rot_y.m[2][0] = s;
	rot_y.m[0][2] = -s;
	rot_y.m[2][2] = c;

	return rot_y;
}

mat4_t mat4_rotat_z(float angle)
{
	mat4_t rot_z = mat4_identity();

	float c = cos(angle);
	float s = sin(angle);

	// |  c  -s   0   0  | \\
	// |  s   c   0   0  | \\
	// |  0   0   1   0  | \\
	// |  0   0   0   1  | \\

	rot_z.m[0][0] = c;
	rot_z.m[0][1] = -s;
	rot_z.m[1][0] = s;
	rot_z.m[1][1] = c;

	return rot_z;
}

mat4_t mat4_scale(float sx, float sy, float sz)
{
	mat4_t scale = mat4_identity();
	scale.m[0][0] = sx;
	scale.m[1][1] = sy;
	scale.m[2][2] = sz;
	return scale;
}

mat4_t mat4_trans(float tx, float ty, float tz)
{
	mat4_t trans = mat4_identity();
	trans.m[0][3] = tx;
	trans.m[1][3] = ty;
	trans.m[2][3] = tz;
	return trans;
}

mat4_t mat4_rotat(float rx, float ry, float rz)
{
	mat4_t rotat = mat4_identity();

	rotat = mat4_mul_mat4(rotat, mat4_rotat_z(rz));
	rotat = mat4_mul_mat4(rotat, mat4_rotat_y(ry));
	rotat = mat4_mul_mat4(rotat, mat4_rotat_x(rx));

	return rotat;
}

mat4_t mat4_scale_v(vec3_t s)
{
	return mat4_scale(s.x, s.y, s.z);
}

mat4_t mat4_trans_v(vec3_t t)
{
	return mat4_trans(t.x, t.y, t.z);
}

mat4_t mat4_rotat_v(vec3_t r)
{
	return mat4_rotat(r.x, r.y, r.z);
}

mat4_t mat4_translate(vec3_t s, vec3_t t, vec3_t r)
{
	mat4_t result = mat4_identity();

	// The order here matters a lot

	result = mat4_mul_mat4(mat4_scale_v(s), result);
	result = mat4_mul_mat4(mat4_rotat_v(r), result);
	result = mat4_mul_mat4(mat4_trans_v(t), result);

	return result;
}
