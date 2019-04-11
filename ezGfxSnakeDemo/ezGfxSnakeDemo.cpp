// ezGfxDemo.cpp : Defines the entry point for the console application.
//

#include "ezGfx.h"

#include <iostream>
#include <vector>
#include <algorithm>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define TIMER_ID 0
#define TIMER_DELAY_MS 200

#define PLAYFIELD_WIDTH_CELLS 20
#define PLAYFIELD_HEIGHT_CELLS 20

#define MAX_SNAKE_SIZE 100

char playfield[PLAYFIELD_HEIGHT_CELLS][PLAYFIELD_WIDTH_CELLS];

struct Point
{
	int x, y;
};

Point directionMap[PLAYFIELD_HEIGHT_CELLS][PLAYFIELD_WIDTH_CELLS];

Point snakeHeadDir;
Point snakeBody[MAX_SNAKE_SIZE];
int currentSnakeSize = 0;
bool foodExists = false;

void drawCell(int x, int y, bool isSnakeHead, bool isFood)
{
	float cellSize = WINDOW_WIDTH / (float)PLAYFIELD_WIDTH_CELLS;
	const float innerOffset = 3;
	const float foodInnerOffset = 10;

	if (isFood)
	{
		ezGfxSetFillParameters(0, 0, 255);
		ezGfxDrawRoundedRectangle(x * cellSize + foodInnerOffset, y * cellSize + foodInnerOffset, cellSize - foodInnerOffset * 2, cellSize - foodInnerOffset * 2, 10);
		return;
	}

	ezGfxSetFillParameters(isSnakeHead ? 0 : 255, isSnakeHead ? 255 : 0, 0);
	ezGfxDrawRoundedRectangle(x * cellSize + innerOffset, y * cellSize + innerOffset, cellSize - innerOffset * 2, cellSize - innerOffset * 2, 10);
}

void drawPlayfield()
{
	for (int i = 0; i < PLAYFIELD_HEIGHT_CELLS; i++)
	{
		for (int j = 0; j < PLAYFIELD_WIDTH_CELLS; j++)
		{
			if (playfield[i][j] > 0)
			{
				bool isFood = playfield[i][j] == 3;
				bool isSnakeHead = playfield[i][j] == 2;
				drawCell(j, i, isSnakeHead, isFood);
			}
		}
	}
}

void drawGraphics()
{
	ezGfxClear();

	drawPlayfield();
}

void idle()
{
}

void updatePlayfield()
{
	for (int i = 0; i < currentSnakeSize; i++)
	{
		playfield[snakeBody[i].y][snakeBody[i].x] = (i == 0) ? 2 : 1;
	}
}

void handleFood()
{
	if (!foodExists)
	{
		while (true)
		{
			// generate food position
			int foodPosX = ezGfxMathRandom(0, PLAYFIELD_WIDTH_CELLS);
			int foodPosY = ezGfxMathRandom(0, PLAYFIELD_HEIGHT_CELLS);

			// it should not collide with any segments of the snake
			bool collision = false;
			for (int i = 0; i < currentSnakeSize; i++)
			{
				if (foodPosX == snakeBody[i].x && foodPosY == snakeBody[i].y)
				{
					collision = true;
					break;
				}
			}

			if (!collision)
			{
				playfield[foodPosY][foodPosX] = 3; // mark cell containing the food
				break;
			}
		}

		foodExists = true;
	}
}

void initSnake()
{
	// create snake, 3 cells wide
	snakeBody[0] = { 2, 0 };
	snakeBody[1] = { 1, 0 };
	snakeBody[2] = { 0, 0 };
	currentSnakeSize = 3;

	updatePlayfield();

	// snake moving to the right
	snakeHeadDir = { 1, 0 };
	directionMap[0][0] = { 1, 0 };
	directionMap[0][1] = { 1, 0 };
	directionMap[0][2] = { 1, 0 };

	foodExists = false;
	handleFood();
}

void initScene()
{
	// clear playfield and direction map
	for (int i = 0; i < PLAYFIELD_HEIGHT_CELLS; i++)
	{
		for (int j = 0; j < PLAYFIELD_WIDTH_CELLS; j++)
		{
			playfield[i][j] = 0;// ezGfxMathRandomBool() ? 1 : 0;
			directionMap[i][j] = { 0, 0 }; // no direction
		}
	}

	initSnake();
}

void checkFoodCollision()
{
	// check for collisions between the snake head and the food
	if (playfield[snakeBody[0].y][snakeBody[0].x] == 3)
	{
		Point lastBlockPos = snakeBody[currentSnakeSize - 1];
		Point lastBlockDir = directionMap[lastBlockPos.y][lastBlockPos.x];

		// calculate position of new block
		Point newBlockPos = { lastBlockPos.x - lastBlockDir.x, lastBlockPos.y - lastBlockDir.y };
		snakeBody[currentSnakeSize] = newBlockPos;
		// copy direction from last block
		directionMap[newBlockPos.y][newBlockPos.x] = lastBlockDir;

		currentSnakeSize++;

		foodExists = false;
	}
}

void checkObstacleCollision()
{
	// check snake head against walls
	
	bool collision = false;

	if (snakeBody[0].x < 0 || snakeBody[0].x > PLAYFIELD_WIDTH_CELLS - 1 ||
		snakeBody[0].y < 0 || snakeBody[0].y > PLAYFIELD_HEIGHT_CELLS - 1)
	{
		collision = true;
	}
	else
	{
		// check against ourself
		for (int i = 1; i < currentSnakeSize; i++)
		{
			if (snakeBody[0].x == snakeBody[i].x && snakeBody[0].y == snakeBody[i].y)
			{
				collision = true;
				break;
			}
		}
	}

	if (collision)
	{
		// reset everything
		initScene();
	}
}

void updateSnake()
{
	// first clear the trace left in the playfield by emptying the cells
	for (int i = 0; i < currentSnakeSize; i++)
	{
		playfield[snakeBody[i].y][snakeBody[i].x] = 0;
	}

	// copy the snake head direction into the direction map
	directionMap[snakeBody[0].y][snakeBody[0].x] = snakeHeadDir;

	for (int i = 0; i < currentSnakeSize; i++)
	{
		Point dir = directionMap[snakeBody[i].y][snakeBody[i].x];
		snakeBody[i].x += dir.x;
		snakeBody[i].y += dir.y;
	}

	checkFoodCollision();
	checkObstacleCollision();
}

void processKeyboard(int key, int state)
{
	if (state == EZGFX_RELEASED)
		return;

	switch (key)
	{
	case 'A':
		snakeHeadDir = { -1, 0 };
		break;
	case 'D':
		snakeHeadDir = { 1, 0 };
		break;
	case 'W':
		snakeHeadDir = { 0, -1 };
		break;
	case 'S':
		snakeHeadDir = { 0, 1 };
		break;
	}
}

void timerFunc(int timerId)
{
	updateSnake();
	updatePlayfield();

	handleFood();

	// trigger screen update
	ezGfxUpdate();
}

int main(int argc, char* argv[])
{
	ezGfxInit(drawGraphics, idle, processKeyboard, nullptr);
	ezGfxInitWindowParameters(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake Demo");

	ezGfxSetFontParameters("Arial", 32, true);

	initScene();

	ezGfxTimerFunc(TIMER_ID, TIMER_DELAY_MS, timerFunc);

	ezGfxMainLoop();

	return 0;
}
