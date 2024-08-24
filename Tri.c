#include "Tri.h"

void int_swap(int* a, int*b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
	return;
}

void drawTri_flatBottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	
}

void drawTri_flatTop(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {

}

void drawTri_flat(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// DONE: Sort by Y
	// DONE: Find My
	// DONE: Find MX
	// TODO: Draw flat bottom
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

	int My = y1;
	int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) - x0;

	drawTri_flatBottom(x0, y0, x1, y1, Mx, My, color);
	drawTri_flatTop(x1, y1, Mx, My, x2, y2, color);
}
