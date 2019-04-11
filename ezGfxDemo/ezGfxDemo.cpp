// ezGfxDemo.cpp : Defines the entry point for the console application.
//

#include "ezGfx.h"

#include <iostream>
#include <vector>
#include <algorithm>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define NUM_BALLS 55
#define BALL_MAX_RADIUS 25
#define BALL_SPEED 150 // pixels per second

#define TIMER_ID 0
#define TIMER_DELAY_MS 25

#define M_PI 3.14159265358979323846f  /* pi */

struct Ball
{
	int radius;
	float posX, posY;
	int dirX, dirY;
};

std::vector<Ball> balls;

float roundRectAngularPos = 0.0f;
float textPositionX = 0;
float textPositionY = WINDOW_HEIGHT - 50;
int textDir = 1;

ezGfxImageRef image;
int imageX = (WINDOW_WIDTH - 188) / 2;
int imageY = (WINDOW_HEIGHT - 188) / 2;


void drawRotatingSquares()
{
	ezGfxSetFillParameters(10, 0, 100);

	for (float angle = roundRectAngularPos; angle < roundRectAngularPos + 2 * M_PI; angle += 2 * M_PI * 0.015f)
	{
		int roundRectX = cosf(angle) * (WINDOW_WIDTH - 100) * 0.5f + WINDOW_WIDTH * 0.5f;
		int roundRectY = sinf(angle) * (WINDOW_HEIGHT - 100) * 0.5f + WINDOW_HEIGHT * 0.5f;

		ezGfxDrawRoundedRectangle(roundRectX - 25, roundRectY - 25, 50, 50, 10);

		roundRectX = cosf(2 * M_PI - angle) * (WINDOW_WIDTH - 300) * 0.5f + WINDOW_WIDTH * 0.5f;
		roundRectY = sinf(2 * M_PI - angle) * (WINDOW_HEIGHT - 300) * 0.5f + WINDOW_HEIGHT * 0.5f;

		ezGfxDrawEllipse(roundRectX, roundRectY, 15, 15);
	}
}

void drawGraphics()
{
	ezGfxClear();

	drawRotatingSquares();

	ezGfxSetFillParameters(100, 0, 0);

	for (int i = 0; i < balls.size(); i++)
	{
		const Ball &ball = balls[i];
		ezGfxDrawEllipse(ball.posX - ball.radius, ball.posY - ball.radius, ball.radius * 2, ball.radius * 2);
	}

	ezGfxDrawImage(image, imageX, imageY);

	ezGfxDrawText(textPositionX, textPositionY, "ezGfx library demo");
}

void checkBallCollision(Ball &ball)
{
	// left wall
	if (ball.posX - ball.radius < 0)
	{
		ball.posX = ball.radius;
		ball.dirX = -ball.dirX;
	}

	// top wall
	if (ball.posY - ball.radius < 0)
	{
		ball.posY = ball.radius;
		ball.dirY = -ball.dirY;
	}

	// right wall
	if (ball.posX + ball.radius > WINDOW_WIDTH)
	{
		ball.posX = WINDOW_WIDTH - ball.radius;
		ball.dirX = -ball.dirX;
	}

	// bottom wall
	if (ball.posY + ball.radius > WINDOW_HEIGHT)
	{
		ball.posY = WINDOW_HEIGHT - ball.radius;
		ball.dirY = -ball.dirY;
	}
}

void updateScene(float elapsed)
{
	for (int i = 0; i < balls.size(); i++)
	{
		Ball &ball = balls[i];
		ball.posX += ball.dirX * elapsed * BALL_SPEED;
		ball.posY += ball.dirY * elapsed * BALL_SPEED;

		checkBallCollision(ball);
	}

	roundRectAngularPos += elapsed * 0.3;

	textPositionX += textDir * elapsed * 50;
	if (textPositionX > WINDOW_WIDTH)
	{
		textPositionY = 0;
		textDir = -textDir;
	}

	if (textPositionX < 0)
	{
		textPositionY = WINDOW_HEIGHT - 50;
		textDir = -textDir;
	}
}

void idle()
{
	float elapsedSeconds = ezGfxGetElapsed();
	updateScene(elapsedSeconds);
	
	// schedule a redraw
	ezGfxUpdate();
}

void processKeyboard()
{

}

void processMouse(int button, int buttons, int state, int x, int y)
{
	// check for intersection
	if (x > imageX && x < imageX + 188 &&
		y > imageY && y < imageY + 188)
	{
		int dirX = ezGfxMathRandomBool() ? 1 : -1;
		int dirY = ezGfxMathRandomBool() ? 1 : -1;

		dirX *= 188 * 0.5;
		dirY *= 188 * 0.5;

		imageX += dirX;
		imageY += dirY;

		if (imageX < 0)
			imageX = WINDOW_WIDTH - 188;
		if (imageX > WINDOW_WIDTH - 188)
			imageX = 0;

		if (imageY < 0)
			imageY = WINDOW_HEIGHT - 188;
		if (imageY > WINDOW_HEIGHT - 188)
			imageY = 0;
	}
}

void timerFunc()
{

}

void initScene()
{
	for (int i = 0; i < NUM_BALLS; i++)
	{
		Ball ball;
		ball.posX = ezGfxMathRandom(0, WINDOW_WIDTH);
		ball.posY = ezGfxMathRandom(0, WINDOW_WIDTH);
		ball.dirX = ezGfxMathRandomBool() ? 1 : -1;
		ball.dirY = ezGfxMathRandomBool() ? 1 : -1;
		ball.radius = ezGfxMathRandom(10, BALL_MAX_RADIUS);
		balls.push_back(ball);
	}

	image = ezGfxLoadImage("cat.jpg");
}

int main(int argc, char* argv[])
{
	ezGfxInit(drawGraphics, idle, NULL, processMouse);
	ezGfxInitWindowParameters(WINDOW_WIDTH, WINDOW_HEIGHT, "Bouncing Balls");

	ezGfxSetFontParameters("Arial", 32, true);

	initScene();

	//ezGfxTimerFunc(TIMER_ID, TIMER_DELAY_MS, timerFunc);

	ezGfxMainLoop();

	ezGfxReleaseImage(image);

	return 0;
}
