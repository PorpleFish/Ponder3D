#include "display.h"

///////////////////////////////////////////
/// Global variables
///////////////////////////////////////////

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* colorBuffer = NULL;
SDL_Texture* colorBufferTexture = NULL;

int windowWidth = 800;
int windowHeight = 600;

///////////////////////////////////////////
/// Functions
///////////////////////////////////////////

bool initializeWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Use SQL to query what the fullscreen size should be
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(
		0,
		&displayMode
	);

	windowWidth = displayMode.w;
	windowHeight = displayMode.h;

	// Create an SDL window
	window = SDL_CreateWindow(
		"PorpleRender",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	// Create an SDL renderer
	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}
	return true;
}

void drawPixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < windowWidth && y >= 0 && y < windowHeight) {
		colorBuffer[(windowWidth * y) + x] = color;
	}
}

void drawGrid(int unitSize) {
	for (int y = 0; y < windowHeight; y++) {
		for (int x = 0; x < windowWidth; x++) {
			if (y % unitSize == 0 || x % unitSize == 0) {
				colorBuffer[(windowWidth * y) + x] = 0xFF222222;
			}
			if (y % unitSize == 0 && x % unitSize == 0) {
				colorBuffer[(windowWidth * y) + x] = 0xFFAAAAAA;
			}
		}
	}
}

void drawRect(int xPos, int yPos, int xSize, int ySize, uint32_t color) {
	for (int y = 0; y < ySize; y++) {
		for (int x = 0; x < xSize; x++) {
			drawPixel(xPos + x, yPos + y, color);
		}
	}
}

void drawLineDDA(int x0, int y0, int x1, int y1, uint32_t color) {
	// Get the adjacent and opposite triangle sides
	int deltaX = (x1 - x0);
	int deltaY = (y1 - y0);

	// Get the length of the triangle 
	// Only for <45 degree angle approach:
	//int sideLength = abs(deltaX); 

	// Approach for all possible angles, inline conditional statement saves on cpu! 
	int longestSideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY); 
	// Ternary operator - for reference. In this case, side length = the longest side

	// Cast to float because the increment of the line isn't going to work in pixels
	float xIncrement = deltaX / (float)longestSideLength;
	float yIncrement = deltaY / (float)longestSideLength;

	// Start the line at the point the line begins
	float currentX = x0;
	float currentY = y0;

	// Loop for each Y line this will cover
	for (int i = 0; i <= longestSideLength; i++) {
		if (i % 1 == 0) {
			drawPixel(round(currentX), round(currentY), color);
		}
		currentX += xIncrement;
		currentY += yIncrement;	
	}
}

void bresenhamLow(int x0, int y0, int x1, int y1, uint32_t color) {
	int deltaX = x1 - x0;
	int deltaY = y1 - y0;

	int yi = 1;

	// If deltaY is less than 0, set Y Index as -1 and invert deltaY
	if (deltaY < 0) {
		yi = -1;
		deltaY = -deltaY;
	}

	int delta = (2 * deltaY) - deltaX;
	int y = y0;

	for (int x = x0; x < x1; x++) {

		drawPixel(x, y, color);

		if (delta > 0) {
			y += yi;
			delta += 2 * (deltaX - deltaY);
		}
		else {
			delta += 2 * deltaY;
		}
	}
}

void bresenhamHigh(int x0, int y0, int x1, int y1, uint32_t color) {
	int deltaX = x1 - x0;
	int deltaY = y1 - y0;

	int xi = 1;

	// If deltaY is less than 0, set Y Index as -1 and invert deltaY
	if (deltaX < 0) {
		xi = -1;
		deltaX = -deltaX;
	}

	int delta = (2 * deltaX) - deltaY;
	int x = x0;

	for (int y = y0; y < y1; y++) {

		drawPixel(x, y, color);

		if (delta > 0) {
			x += xi;
			delta += 2 * (deltaX - deltaY);
		}
		else {
			delta += 2 * deltaX;
		}
	}
}

void drawLineBres(int x0, int y0, int x1, int y1, uint32_t color) {

	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			bresenhamLow(x1, y1, x0, y0, color);
		}
		else {
			bresenhamLow(x0, y0, x1, y1, color);
		}
	}
	else {
		if (y0 > y1) {
			bresenhamHigh(x1, y1, x0, y0, color);
		}
		else {
			bresenhamHigh(x0, y0, x1, y1, color);
		}
	}
}

void drawTri(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	drawLineBres(x0, y0, x1, y1, color);
	drawLineBres(x1, y1, x2, y2, color);
	drawLineBres(x2, y2, x0, y0, color);
}

void renderColorBuffer(void) {
	// Copies the buffer into the texture
	SDL_UpdateTexture(
		colorBufferTexture,
		NULL,
		colorBuffer,
		(windowWidth * sizeof(uint32_t))
	);
	// Renders the buffer texture, with no subdivision
	SDL_RenderCopy(
		renderer,
		colorBufferTexture,
		NULL,
		NULL
	);
}

void clearColorBuffer(uint32_t color) {
	for (int y = 0; y < windowHeight; y++) {
		for (int x = 0; x < windowWidth; x++) {
			colorBuffer[(windowWidth * y) + x] = color;
		}
	}
}

void destroyWindow(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return;
}