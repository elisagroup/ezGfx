// ezGfxBlobsDemo.cpp : Defines the entry point for the console application.
//

#include "ezGfx.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

#define NUM_BALLS 20
#define BALL_RADIUS 15
#define BALL_SPEED 100 // pixels per second

#define TIMER_ID 0
#define TIMER_DELAY_MS 20

#define ISO_VALUE 2.3

struct Ball
{
	int radius;
	float posX, posY;
	int dirX, dirY;
};

std::vector<Ball> balls;
std::vector<float> field;
float minEnergy, maxEnergy;

class vec2
{
public:
	vec2(float x, float y) : m_x(x), m_y(y) {}
	vec2 operator-(const vec2& other) const { return vec2(this->m_x - other.m_x, this->m_y - other.m_y); }
	float length() { return sqrt(m_x * m_x + m_y * m_y); }

	float m_x, m_y;
};

float energyField(vec2 p)
{
	float nrg = 0.f;
	bool result = false;
	for (int i = 0; i < NUM_BALLS; ++i)
	{
		float radius = balls[i].radius;
		vec2 ballPos(balls[i].posX, balls[i].posY);
		float denom = std::max(0.0001f, (ballPos - p).length());
		nrg += (radius / denom);
	}
	return nrg;
}

void drawGraphics()
{
	ezGfxClear();

	unsigned char *pixels = ezGfxGetPixels(0, WINDOW_HEIGHT);
	int stride = WINDOW_WIDTH * 4;

	for (int i = 0; i < WINDOW_HEIGHT; i++)
	{
		for (int j = 0; j < WINDOW_WIDTH; j++)
		{
			unsigned char *pixel = pixels + i * stride + j * 4;
			
			float nrg = field[i * WINDOW_WIDTH + j];
			// map to 0..1
			float intensity = (nrg - minEnergy) / (maxEnergy - minEnergy);
			
			if (nrg < ISO_VALUE)
				intensity = 0;
			else
				intensity = 1;

			// use green color
			pixel[1] = 255 * intensity;
		}
	}

	ezGfxUpdatePixels(0, WINDOW_HEIGHT);

	/*for (int i = 0; i < balls.size(); i++)
	{
		ezGfxDrawEllipse(balls[i].posX - balls[i].radius,
			balls[i].posY - balls[i].radius,
			balls[i].radius * 2, balls[i].radius * 2);
	}*/
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

	// recalculate energy field
	minEnergy = std::numeric_limits<float>::max();
	maxEnergy = std::numeric_limits<float>::min();
	for (int i = 0; i < WINDOW_HEIGHT; i++)
	{
		for (int j = 0; j < WINDOW_WIDTH; j++)
		{
			float nrg = energyField(vec2(j, i));
			field[i * WINDOW_WIDTH + j] = nrg;
		
			minEnergy = std::min(nrg, minEnergy);
			maxEnergy = std::max(nrg, maxEnergy);
		}
	}
}

void timerFunc(int timerId)
{
	updateScene(TIMER_DELAY_MS / 1000.f);

	// schedule a redraw
	ezGfxUpdate();
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
		ball.radius = BALL_RADIUS;
		balls.push_back(ball);
	}

	field.resize(WINDOW_WIDTH * WINDOW_HEIGHT);
}

int main(int argc, char* argv[])
{
	ezGfxInit(drawGraphics, nullptr, nullptr, nullptr);
	ezGfxInitWindowParameters(WINDOW_WIDTH, WINDOW_HEIGHT, "Blobs");

	initScene();

	ezGfxTimerFunc(TIMER_ID, TIMER_DELAY_MS, timerFunc);

	ezGfxMainLoop();

	return 0;
}
