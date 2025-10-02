#ifndef SETTINGS_H
#define SETTINGS_H

typedef enum faceRenderStyle_t {
	FLAT,
	FLAT_TEXTURED
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
	faceRenderStyle_t face_render_style;
} settings_t;

#endif