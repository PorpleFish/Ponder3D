#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"
#include "Vector.h"

typedef struct {
	vec3_t direction;
	float intensity;
} light_t;

light_t dirlight_from_angle();

float light_getFaceAlignment(vec3_t normal, light_t light);

uint32_t apply_light(color_t color, float factor);

#endif