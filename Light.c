#include "Light.h"
#include <math.h>

float light_getFaceAlignment(vec3_t normal, light_t light)
{
    float result = fabs(vec3_dot(normal, light.direction));
    result *= light.intensity;
    return result;
}

uint32_t apply_light(uint32_t color, float factor)
{
    if (factor < 0) factor = 0;
    if (factor > 1) factor = 1;

    uint32_t a = (color & 0xFF000000);
    uint32_t r = (color & 0x00FF0000) * factor;
    uint32_t g = (color & 0x0000FF00) * factor;
    uint32_t b = (color & 0x000000FF) * factor;

    uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
    
    return new_color;
}

