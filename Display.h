#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

///////////////////////////////////////////
/// Global variables
///////////////////////////////////////////

#define FPS 30
#define FRAME_TARGET_TIME (1000/FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* colorBuffer;
extern SDL_Texture* colorBufferTexture;

extern int windowWidth;
extern int windowHeight;

///////////////////////////////////////////
/// Functions
///////////////////////////////////////////

bool initializeWindow(void);

void drawPixel(int x, int y, uint32_t color);
void drawGrid(int unitSize);
void drawRect(int xPos, int yPos, int xSize, int ySize, uint32_t color);
void drawLineDDA(int x0, int y0, int x1, int y1, uint32_t color); // Slow + Easy - The division is heavy
void drawLineBres(int x0, int y0, int x1, int y1, uint32_t color); // Very performant - Bresenham
void drawLineXiao(int x0, int y0, int x1, int y1, uint32_t color); // Slow + Pretty - Xiaolin Wu
void drawTri(int x0, int y0, int x1, int y1, int x2, int y3, uint32_t color);

void renderColorBuffer(void);
void clearColorBuffer(uint32_t color);

void destroyWindow(void);

#endif