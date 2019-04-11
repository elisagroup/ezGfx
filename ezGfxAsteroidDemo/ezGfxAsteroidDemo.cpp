// ezGfxDemo.cpp : Defines the entry point for the console application.
//

#include "ezGfx.h"
#include "MathHelpers.h"
#include "SoundEffect.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <future>
#include <memory>

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define THRUST 300.f
#define BULLET_SPEED 800.f
#define BULLET_SIZE 10.f


struct Spaceship
{
	Vec2 pos;
	Vec2 speed;
	Vec2 accel;
	float rotation;
	vector<Rect> body;
	bool collided;
};

struct Asteroid
{
	Vec2 pos;
	Vec2 speed;
	float radius;
	int colorR, colorG, colorB;
};

struct Bullet
{
	Vec2 pos;
	Vec2 speed;
};

Spaceship g_ship;
vector<Asteroid> g_asteroids;
vector<Bullet> g_bullets;

string g_prompts[] = { "OUCH!", "YOU'RE KIDDING!", "HEY, ARE YOU A COMPLETE LOOSER?", "STOP PLAYING, IT'S A GAME OVER ALREADY!" };
int g_promptIndex = 0;

float g_textPositionX = WINDOW_WIDTH;
float g_textPositionY = 20;

int g_points = 0;

// ### musir data starts here

#define MUSIC_LIFE 0
#define MUSIC_GROW 1
#define MUSIC_COIN 2
#define MUSIC_PIPE 3
#define MUSIC_FIRE 4
#define MUSIC_HIT 5
#define MUSIC_DEAD 6
#define MUSIC_NOTE 7
#define MUSIC_STAR 8

vector<int> g_lifeMusic = { 56, 8, 61, 8, 65, 8, 61, 8, 63, 8, 68, 8 };
vector<int> g_growMusic = { 37, 4, 44, 4, 49, 4, 38, 4, 45, 4, 50, 4, 39, 4, 46, 4, 51, 4 };
vector<int> g_coinMusic = { 66, 1 };
vector<int> g_pipeMusic = { 13, 0, 13, 8, 1, 0 };
vector<int> g_fireMusic = { 41, 1, 46, 1, 41, 1 };
vector<int> g_hitMusic = { 25, 2, 13, 3, 1, 4, 25, 1, 13, 2, 1, 3 };
vector<int> g_deadMusic = { 25, 3, 13, 4, 1, 6 };
vector<int> g_noteMusic = { 1, 3, 13, 4, 1, 6 };
vector<int> g_starMusic = { 7, 4, 41, 4, 44, 4, 49, 4, 53, 4, 56, 4, 61, 4, 65, 4, 68, 4, 73, 4 };

vector<int> g_musicArray[] = { g_lifeMusic, g_growMusic, g_coinMusic, g_pipeMusic, g_fireMusic, g_hitMusic, g_deadMusic, g_noteMusic, g_starMusic };

// ### music data ends here


void drawShip()
{
	ezGfxSetStrokeParameters(2, 0, 255, 0);

	int shipX = g_ship.pos.x();
	int shipY = g_ship.pos.y();

	// iterate through and draw all rectangles our spaceship is made of
	for (auto& rect : g_ship.body)
	{
		float x = rect.left;
		float y = rect.top;
		float x1 = rect.right;
		float y1 = rect.top;
		float x2 = rect.right;
		float y2 = rect.bottom;
		float x3 = rect.left;
		float y3 = rect.bottom;

		rotatePoint(x, y, g_ship.rotation);
		rotatePoint(x1, y1, g_ship.rotation);
		rotatePoint(x2, y2, g_ship.rotation);
		rotatePoint(x3, y3, g_ship.rotation);

		// draw square body
		ezGfxMoveTo(shipX + x, shipY + y);
		ezGfxLineTo(shipX + x1, shipY + y1);
		ezGfxLineTo(shipX + x2, shipY + y2);
		ezGfxLineTo(shipX + x3, shipY + y3);
		ezGfxLineTo(shipX + x, shipY + y);
	}
}

void drawBullets()
{
	ezGfxSetStrokeParameters(1, 255, 255, 255);
	ezGfxSetFillParameters(255, 255, 255);
	for (auto& bullet : g_bullets)
	{
		ezGfxDrawEllipse(bullet.pos.x() - BULLET_SIZE / 2, bullet.pos.y() - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE);
	}
}

void drawAsteroids()
{
	for (auto& asteroid : g_asteroids)
	{
		ezGfxSetStrokeParameters(1, asteroid.colorR, asteroid.colorG, asteroid.colorB);
		ezGfxSetFillParameters(asteroid.colorR, asteroid.colorG, asteroid.colorB);
		ezGfxDrawEllipse(asteroid.pos.x() - asteroid.radius, asteroid.pos.y() - asteroid.radius, asteroid.radius * 2, asteroid.radius * 2);
	}
}

void drawGraphics()
{
	ezGfxClear();

	drawAsteroids();
	drawShip();
	drawBullets();

	// draw animated text
	if (g_ship.collided)
	{
		ezGfxDrawText(g_textPositionX, g_textPositionY, g_prompts[g_promptIndex].c_str());
	}

	// draw points
	string pts = "SCORE: " + to_string(g_points);
	ezGfxDrawText(10, WINDOW_HEIGHT - 40, pts.c_str());
}

void audioThreadFunc(int* notes, int size)
{
	SoundEffect effect(notes, size);
	effect.Play();
}

void playSound(int soundIndex)
{
	async(launch::async, [=]
	{
		SoundEffect effect(g_musicArray[soundIndex].data(), g_musicArray[soundIndex].size());
		effect.Play();
	});
}

void checkWallCollisions()
{
	bool collision = false;

	// left wall
	if (g_ship.pos.x() - 40 < 0)
	{
		g_ship.speed.x() = -g_ship.speed.x();
		collision = true;
	}

	// top wall
	if (g_ship.pos.y() - 40 < 0)
	{
		g_ship.speed.y() = -g_ship.speed.y();
		collision = true;
	}

	// right wall
	if (g_ship.pos.x() + 40 > WINDOW_WIDTH)
	{
		g_ship.speed.x() = -g_ship.speed.x();
		collision = true;
	}

	// bottom wall
	if (g_ship.pos.y() + 40 > WINDOW_HEIGHT)
	{
		g_ship.speed.y() = -g_ship.speed.y();
		collision = true;
	}

	// apply a friction i.e. reduce speed after wall collision
	if (collision)
	{
		g_ship.speed = g_ship.speed.normalized() * g_ship.speed.length() * 0.5;
		// update position by a small timestep to make sure ship gets off the walls
		g_ship.pos += g_ship.speed * 0.005;

		playSound(MUSIC_HIT);
	}
}

void checkCollisions()
{
	// check bullets, remove the ones that went off screen
	auto iterator = remove_if(g_bullets.begin(), g_bullets.end(), [](const Bullet& bullet)
	{
		if (bullet.pos.x() < 0 || bullet.pos.x() > WINDOW_WIDTH ||
			bullet.pos.y() < 0 || bullet.pos.y() > WINDOW_HEIGHT)
		{
			return true;
		}
		return false;
	});
	g_bullets.erase(iterator, g_bullets.end());

	// iterate through all asteroids and check if they collide with the spaceship or bullets
	bool collisionWithBullet = false;
	bool collisionWithSpaceship = false;
	for (auto& asteroid: g_asteroids)
	{
		// collide with spaceship body
		for (auto& rect : g_ship.body)
		{
			// transform asteroid to spaceship's local space
			Vec2 pos = asteroid.pos;
			rotatePoint(pos.x(), pos.y(), -g_ship.rotation, g_ship.pos.x(), g_ship.pos.y());
			pos -= g_ship.pos;

			// check for collision
			collisionWithSpaceship = circleIntersectsRectangle(pos, asteroid.radius, rect);
			if (collisionWithSpaceship)
				break;
		}
		
		// collide with bullets
		for (int i = 0; i < g_bullets.size(); i++)
		{
			const Bullet& bullet = g_bullets[i];

			// check if asteroid circle contains the bullet position
			if ((bullet.pos - asteroid.pos).length() <= (asteroid.radius + BULLET_SIZE / 2))
			{
				// delete the bullet
				g_bullets.erase(g_bullets.begin() + i);

				// reduce the asteroid size
				if (asteroid.radius > 0)
					asteroid.radius -= 30;
				if (asteroid.radius < 0)
					asteroid.radius = 0;

				collisionWithBullet = true;
				break;
			}
		}

		if (collisionWithSpaceship || collisionWithBullet)
			break;
	}

	// remove asteroids whose went too small (size is zero)
	if (collisionWithBullet)
	{
		g_points += 50;

		g_asteroids.erase(remove_if(g_asteroids.begin(), g_asteroids.end(), [](const Asteroid& asteroid)
		{
			return asteroid.radius == 0;
		}), g_asteroids.end());

		playSound(MUSIC_NOTE);
	}

	if (collisionWithSpaceship)
	{
		g_ship.collided = true;
		//cout << "collision detected" << endl;
	}

	checkWallCollisions();
}

void updateScene(float elapsed)
{
	g_ship.speed += g_ship.accel * elapsed;

	// update position for ship
	g_ship.pos += g_ship.speed * elapsed;

	// update position for asteroids
	for (auto& asteroid: g_asteroids)
	{
		asteroid.pos += asteroid.speed * elapsed;
	}

	// update bullets
	for (auto& bullet : g_bullets)
	{
		bullet.pos += bullet.speed * elapsed;
	}

	checkCollisions();

	// when spaceship collides with asteroids we should perform the text scroll animation
	if (g_ship.collided)
	{
		g_textPositionX -= elapsed * 400;
		if (g_textPositionX < -800)
		{
			g_textPositionX = WINDOW_WIDTH;
			g_ship.collided = false;
			if (g_promptIndex < 3)
				++g_promptIndex;
		}
	}
}

void idle()
{
	float elapsedSeconds = ezGfxGetElapsed();
	updateScene(elapsedSeconds);

	// schedule a redraw
	ezGfxUpdate();
}

void processKeyboard(int key, int state)
{
	if (state == EZGFX_RELEASED)
	{
		g_ship.accel = Vec2(0, 0);
		return;
	}

	switch (key)
	{
	case 'a':
	case 'A':
		g_ship.rotation -= 10;
		if (g_ship.rotation <= -360)
			g_ship.rotation = 0;
		break;
	case 'd':
	case 'D':
		g_ship.rotation += 10;
		if (g_ship.rotation >= 360)
			g_ship.rotation = 0;
		break;
	case 'w':
	case 'W':
	{
		float angle = DEG_TO_RAD(g_ship.rotation - 90);
		g_ship.accel = Vec2(cos(angle), sin(angle)) * THRUST;
		break;
	}
	case 's':
	case 'S':
	{
		float angle = DEG_TO_RAD(g_ship.rotation - 90);
		g_ship.accel = Vec2(cos(angle), sin(angle)) * -THRUST;
		break;
	}
	case 0x20: // SPACE key
	{
		break;
	}
	default:
		break;
	}
}

void processMouse(int button, int buttons, int state, int x, int y)
{
	if (state == EZGFX_RELEASED)
		return;

	if (state == EZGFX_MOVED)
	{
		Vec2 cursorDir = Vec2(x, y) - g_ship.pos;
		cursorDir.normalize();

		g_ship.rotation = RAD_TO_DEG( atan2(cursorDir.y(), cursorDir.x()) ) + 90;
	}
	else if (state == EZGFX_PRESSED && button == EZGFX_LEFT_BUTTON)
	{
		// spawn a bullet
		Bullet bullet;
		bullet.pos = g_ship.pos;
		Vec2 speedVec = Vec2(x, y) - bullet.pos;
		speedVec.normalize();
		bullet.pos += speedVec * 40;
		speedVec *= BULLET_SPEED;
		bullet.speed = speedVec;
		g_bullets.push_back(bullet);

		playSound(MUSIC_FIRE);
	}
}

void initializeSpaceship()
{
	g_ship.pos = Vec2(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f);
	g_ship.speed = Vec2(0, 0);
	g_ship.accel = Vec2(0, 0);
	g_ship.rotation = 0;
	g_ship.body.push_back(Rect(-15, -40, 15, 40));	// main body
	g_ship.body.push_back(Rect(-25, 0, -15, 50));	// left wing 
	g_ship.body.push_back(Rect(25, 0, 15, 50));		// right wing
	g_ship.collided = false;
}

void initScene()
{
	// initialize ship on screen center
	initializeSpaceship();

	// initialize asteroids with random locations
	for (int i = 0; i < 500; i++)
	{
		float x = ezGfxMathRandom(-WINDOW_WIDTH * 4, WINDOW_WIDTH);
		float y = ezGfxMathRandom(-WINDOW_HEIGHT * 4, WINDOW_HEIGHT);

		// asteroids should not appear int he mid of screen
		if ((x < WINDOW_WIDTH * 0.4 || x > WINDOW_WIDTH * 0.6) &&
			(y < WINDOW_HEIGHT * 0.4 || y > WINDOW_HEIGHT * 0.6))
		{
			Asteroid asteroid;
			asteroid.pos = Vec2(x, y);
			asteroid.speed = Vec2(40, 0);
			asteroid.radius = ezGfxMathRandom(20, 70);
			asteroid.colorR = ezGfxMathRandom(0, 255);
			asteroid.colorG = ezGfxMathRandom(0, 255);
			asteroid.colorB = ezGfxMathRandom(0, 255);
			g_asteroids.push_back(asteroid);
		}
	}
}

int main(int argc, char* argv[])
{
	ezGfxInit(drawGraphics, idle, processKeyboard, processMouse);
	ezGfxInitWindowParameters(WINDOW_WIDTH, WINDOW_HEIGHT, "Asteroid - Use W/S for Throttle and your mouse to turn around");

	ezGfxSetFontParameters("Arial", 35, true);

	initScene();

	ezGfxMainLoop();

	return 0;
}
