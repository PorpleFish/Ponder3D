#include "Tri.h"

void int_swap(int* a, int*b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
	return;
}

void drawTri_flatBottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float inverseSlopeA = (float)(x1 - x0) / (y1 - y0);
	float inverseSlopeB = (float)(x2 - x0) / (y2 - y0);
	
	float startX = x0;
	float endX = x0;

	for (int y = y0; y <= y2; y++) {
		
		drawLineBres(startX, y, endX, y, color);

		startX += inverseSlopeA;
		endX += inverseSlopeB;
	}
}

void drawTri_flatTop(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float inverseSlopeA = (float)(x2 - x0) / (y2 - y0);
	float inverseSlopeB = (float)(x2 - x1) / (y2 - y1);

	float startX = x2;
	float endX = x2;

	for (int y = y2; y >= y0; y--) {
		drawLineBres(startX, y, endX, y, color);

		startX -= inverseSlopeA;
		endX -= inverseSlopeB;
	}
}

void drawTri_flat(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// DONE: Sort by Y
	// DONE: Find My
	// DONE: Find MX
	// DONE: Draw flat bottom
	// TODO: Draw flat top
	
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}

	if (y1 == y2)
	{
		drawTri_flatBottom(x0, y0, x1, y1, x2, y2, color);
	} else if (y0 == y1) {
		drawTri_flatTop(x0, y0, x1, y1, x2, y2, color);
	} else {
		int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
		drawTri_flatBottom(x0, y0, x1, y1, Mx, y1, color);
		drawTri_flatTop(x1, y1, Mx, y1, x2, y2, color);
	}

}
