// ezGfxTetrisDemo.cpp : Defines the entry point for the console application.
//

#include "ezGfx.h"

#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>


#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 800

#define TIMER_ID 0
#define TIMER_DELAY_MS 300

#define DEG_TO_RAD(degrees) ((degrees * M_PI) / 180.f)

#define PLAYFIELD_WIDTH_CELLS 10
#define PLAYFIELD_HEIGHT_CELLS 20

char playfield[PLAYFIELD_HEIGHT_CELLS][PLAYFIELD_WIDTH_CELLS];

struct CellIndex
{
	int x, y;
};

CellIndex figureBlocksFinal[4];
CellIndex figureBlocks[4];
CellIndex currentFigurePos;

#define NUM_SHAPES 5

// tetris shape definitions, cell indices are specified relative to the shape's pivot point
CellIndex tetrisShapes[NUM_SHAPES][4] = {
{ { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } },		// line shape
{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },			// square shape
{ { -1, -1 }, { -1, 0 }, { 0, 0 }, { 1, 0 } },		// L shape
{ { -1, 0 }, { 0, 0 }, { 1, 0 }, { 0, -1 } },		// tee shape
{ { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } }			// Z shape
};

int currentShapeIndex;


void updateCurrentShapePosition(int posX, int posY)
{
	for (int i = 0; i < 4; i++)
	{
		figureBlocksFinal[i].x = posX + figureBlocks[i].x;
		figureBlocksFinal[i].y = posY + figureBlocks[i].y;
	}
}

bool willCollideWithWall(int posX, int posY)
{
	for (int i = 0; i < 4; i++)
	{
		int x = posX + figureBlocks[i].x;
		if (x < 0 || x > PLAYFIELD_WIDTH_CELLS - 1)
		{
			return true;
		}
	}

	return false;
}

bool willCollideWithPlayfieldAndFloor(int posX, int posY)
{
	for (int i = 0; i < 4; i++)
	{
		int x = posX + figureBlocks[i].x;
		int y = posY + figureBlocks[i].y;

		// check against the floor
		if (y >= PLAYFIELD_HEIGHT_CELLS - 1)
		{
			return true;
		}

		// check against the playfield
		if (playfield[y + 1][x] != 0)
		{
			return true;
		}
	}

	return false;
}

void rotateCurrentShape()
{
	for (int i = 0; i < 4; i++)
	{
		int relX = figureBlocks[i].x;
		int relY = figureBlocks[i].y;

		// ignore the pivot point
		if (relX == 0 && relY == 0)
		{
			continue;
		}

		// rotate by 90 degrees
		float s = sin(M_PI_2);
		float c = cos(M_PI_2);

		// do the rotation
		figureBlocks[i].x = relX * c - relY * s;
		figureBlocks[i].y = relX * s + relY * c;
	}
}

void processKeyboard(int key, int state)
{
	if (state == EZGFX_RELEASED)
		return;

	CellIndex newPos = currentFigurePos;

	switch (key)
	{
	case 'A':
		newPos.x--;
		break;
	case 'D':
		newPos.x++;
		break;
	case 'W':
		// do not attempt to rotate the square
		if (currentShapeIndex != 1)
			rotateCurrentShape();
		break;
	case 'S':
		break;
	case 0x20: // SPACE key
		// let the shape fall until the collision happens with floor or playfield blocks
		while (!willCollideWithPlayfieldAndFloor(newPos.x, newPos.y))
		{
			newPos.y++;
		}
		break;
	}

	if (willCollideWithWall(newPos.x, newPos.y))
		return;

	currentFigurePos = newPos;
	updateCurrentShapePosition(currentFigurePos.x, currentFigurePos.y);

	// trigger screen update
	ezGfxUpdate();
}

void generateRandomShape()
{
	currentFigurePos = { PLAYFIELD_WIDTH_CELLS / 2, 0 };
	currentShapeIndex = ezGfxMathRandom(0, NUM_SHAPES - 1);

	// copy initial shape data into separate array
	memcpy(figureBlocks, tetrisShapes[currentShapeIndex], sizeof(figureBlocks));
	
	updateCurrentShapePosition(currentFigurePos.x, currentFigurePos.y);
}

void initScene()
{
	// clear playfield
	for (int i = 0; i < PLAYFIELD_HEIGHT_CELLS; i++)
	{
		for (int j = 0; j < PLAYFIELD_WIDTH_CELLS; j++)
		{
			playfield[i][j] = 0;
		}
	}

	generateRandomShape();
}

void copyCurrentFigureToPlayfield()
{
	for (int i = 0; i < 4; i++)
	{
		playfield[figureBlocksFinal[i].y][figureBlocksFinal[i].x] = 1;
	}
}

bool checkGameOver()
{
	for (int i = 0; i < 4; i++)
	{
		if (figureBlocksFinal[i].y <= 0)
		{
			return true;
		}
	}

	return false;
}

void checkCollisions()
{
	bool hasCollision = false;

	for (int i = 0; i < 4; i++)
	{
		// check against the floor
		if (figureBlocksFinal[i].y >= PLAYFIELD_HEIGHT_CELLS - 1)
		{
			hasCollision = true;
			break;
		}

		// check against the playfield
		if (playfield[figureBlocksFinal[i].y + 1][figureBlocksFinal[i].x] != 0)
		{
			hasCollision = true;
			break;
		}
	}

	// if collision is detect copy the current figure into playfield and generate a new one
	if (hasCollision)
	{
		copyCurrentFigureToPlayfield();
		
		if (checkGameOver())
		{
			initScene();
		}
		else
		{
			generateRandomShape();
		}
	}
}

void checkFilledRows()
{
	for (int j = 0; j < PLAYFIELD_HEIGHT_CELLS; j++)
	{
		bool rowFilled = true;
		for (int i = 0; i < PLAYFIELD_WIDTH_CELLS; i++)
		{
			if (playfield[j][i] == 0)
			{
				rowFilled = false;
				break;
			}
		}

		if (rowFilled)
		{
			// remove the filled row by shifting the rest of the rows towards the floor
			for (int rowIdx = j - 1; rowIdx >= 0; rowIdx--)
			{
				for (int i = 0; i < PLAYFIELD_WIDTH_CELLS; i++)
				{
					playfield[rowIdx + 1][i] = playfield[rowIdx][i];
				}
			}
		}
	}
}

void timerFunc(int timerId)
{
	checkCollisions();

	// shift the figure down
	currentFigurePos.y++;
	updateCurrentShapePosition(currentFigurePos.x, currentFigurePos.y);

	checkFilledRows();

	// trigger screen update
	ezGfxUpdate();
}

void drawCell(int x, int y, bool active)
{
	float cellSize = WINDOW_WIDTH / (float)PLAYFIELD_WIDTH_CELLS;
	const float innerOffset = 3;

	ezGfxSetFillParameters(active ? 255 : 0, 0, active ? 0 : 255);
	ezGfxDrawRoundedRectangle(x * cellSize + innerOffset, y * cellSize + innerOffset, cellSize - innerOffset * 2, cellSize - innerOffset * 2, 10);
}

void drawCurrentFigure()
{
	for (int i = 0; i < 4; i++)
	{
		drawCell(figureBlocksFinal[i].x, figureBlocksFinal[i].y, true);
	}
}

void drawPlayfield()
{
	for (int i = 0; i < PLAYFIELD_HEIGHT_CELLS; i++)
	{
		for (int j = 0; j < PLAYFIELD_WIDTH_CELLS; j++)
		{
			if (playfield[i][j] > 0)
			{
				drawCell(j, i, false);
			}
		}
	}
	drawCurrentFigure();
}

void drawGraphics()
{
	ezGfxClear();

	drawPlayfield();
}

int main(int argc, char* argv[])
{
	ezGfxInit(drawGraphics, nullptr, processKeyboard, nullptr);
	ezGfxInitWindowParameters(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris Demo");

	ezGfxSetFontParameters("Arial", 32, true);

	initScene();

	ezGfxTimerFunc(TIMER_ID, TIMER_DELAY_MS, timerFunc);

	ezGfxMainLoop();
}
