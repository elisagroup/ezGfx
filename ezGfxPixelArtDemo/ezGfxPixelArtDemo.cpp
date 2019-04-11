// ezGfxDemo.cpp : Defines the entry point for the console application.
//

#include "ezGfx.h"
#include <iostream>

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define NUM_HEIGHT_CELLS 40
#define NUM_WIDTH_CELLS 40

#define CELL_DISTANCE 2

struct Color
{
	int colorR;
	int colorG;
	int colorB;
};

int cellArray[NUM_HEIGHT_CELLS][NUM_WIDTH_CELLS];

#define NUM_PALETTE_COLORS 5

Color colorPalette[NUM_PALETTE_COLORS] = {
	{ 0, 0, 0 },
	{ 255, 0, 0 },
	{ 0, 255, 0 },
	{ 0, 0, 255 },
	{ 255, 255, 0 }
};

int selectedColorIndex = 1;

void drawCell(int x, int y, Color color)
{
	ezGfxSetFillParameters(color.colorR, color.colorG, color.colorB);

	int cellWidth = WINDOW_WIDTH / NUM_WIDTH_CELLS;
	int cellHeight = WINDOW_HEIGHT / NUM_HEIGHT_CELLS;

	int pixelX = x * cellWidth;
	int pixelY = y * cellHeight;

	pixelX += CELL_DISTANCE;
	pixelY += CELL_DISTANCE;

	cellWidth -= 2 * CELL_DISTANCE;
	cellHeight -= 2 * CELL_DISTANCE;

	ezGfxDrawRectangle(pixelX, pixelY, cellWidth, cellHeight);
}

void drawGraphics()
{
	// add your drawing code here
	ezGfxClear();

	ezGfxSetStrokeParameters(1, 0, 0, 0);

	for (int i = 0; i < NUM_HEIGHT_CELLS; i++)
	{
		for (int j = 0; j < NUM_WIDTH_CELLS; j++)
		{
			Color selectedColor = colorPalette[cellArray[i][j]];
			drawCell(j, i, selectedColor);
		}
	}
}

void processMouse(int button, int buttons, int state, int x, int y)
{
	if (state == EZGFX_RELEASED)
		return;

	int cellWidth = WINDOW_WIDTH / NUM_WIDTH_CELLS;
	int cellHeight = WINDOW_HEIGHT / NUM_HEIGHT_CELLS;

	// convert pixel X and Y to block coordinates
	int cellX = x / cellWidth;
	int cellY = y / cellHeight;

	//cout << cellX << " " << cellY << endl;

	if (buttons & EZGFX_LEFT_BUTTON && buttons & EZGFX_RIGHT_BUTTON && state == EZGFX_PRESSED)
	{
		selectedColorIndex++;
		if (selectedColorIndex >= NUM_PALETTE_COLORS)
			selectedColorIndex = 1;
	}

	if (buttons == EZGFX_LEFT_BUTTON && state == EZGFX_MOVED)
	{
		cellArray[cellY][cellX] = selectedColorIndex;
	}
	else if (buttons == EZGFX_RIGHT_BUTTON && state == EZGFX_MOVED)
	{
		cellArray[cellY][cellX] = 0;
	}

	if (buttons != EZGFX_NO_BUTTON)
		ezGfxUpdate();
}

void animateCells(int timerId)
{
	for (int i = 0; i < NUM_HEIGHT_CELLS; i++)
	{
		for (int j = 0; j < NUM_WIDTH_CELLS; j++)
		{
			if (cellArray[i][j] == 0)
				continue;

			cellArray[i][j]++;
			if (cellArray[i][j] >= NUM_PALETTE_COLORS)
				cellArray[i][j] = 1;
		}
	}

	ezGfxUpdate();
}

int main(int argc, char* argv[])
{
	ezGfxInit(drawGraphics, nullptr, nullptr, processMouse);
	ezGfxInitWindowParameters(WINDOW_WIDTH, WINDOW_HEIGHT, "ezGfx Pixel Art");

	ezGfxTimerFunc(0, 200, animateCells);

	// this loop only returns when you exit the app
	ezGfxMainLoop();

	return 0;
}
