#ifndef SETTINGS_H
#define SETTINGS_H

typedef enum faceRenderStyle_t {
	triStyle_flat,
	// TODO: triStyle_matcap,
	// TODO: triStyle_phong, 
	// TODO: triStyle_lambert,
	// TODO: triStyle_shaded
} faceRenderStyle_t;
typedef enum cullMethod_t {
	cull_non,
	cull_backface
} cullMethod_t;

typedef struct{
	bool showVerts;
	bool showWireframe;
	bool showTris;
	cullMethod_t cull;
} settings_t;

#endif