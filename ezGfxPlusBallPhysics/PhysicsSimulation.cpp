
#include "PhysicsSimulation.h"
#include "ezGfxPlusMath.h"

using namespace std;

#define GRAVITY Vec2(0, 9.8)

// this specifies how many screen pixels correspond to one meter in the simulation
#define PIXELS_PER_METER 400

#define NUM_BALLS 10

// this simulates a friction when the ball collides with walls
#define BOUNCINESS 0.85

#define CIRCLE_SIZE 50
#define CIRCLE_HALF_SIZE (CIRCLE_SIZE / 2)


PhysicsSimulation::PhysicsSimulation()
{
	for (int i = 0; i < NUM_BALLS; i++)
	{
		Ball ball;
		int randomX = ezGfxPlus::Math::randomNumber(CIRCLE_HALF_SIZE, 800 - CIRCLE_HALF_SIZE);
		int randomY = ezGfxPlus::Math::randomNumber(CIRCLE_HALF_SIZE, 800 - CIRCLE_HALF_SIZE);
		ball.m_pos = Vec2(randomX, randomY);
		ball.m_weight = 0.3;
		ball.m_speed = Vec2(0, 0);
		ball.m_force = Vec2(0, 0);
		m_balls.push_back(ball);
	}
}

void PhysicsSimulation::applyForce(Vec2 force)
{
	for (Ball& ball : m_balls)
	{
		ball.m_force = force;
	}
}

void PhysicsSimulation::advance(float seconds)
{
	// F = m * A
	// A = F / m

	for (Ball& ball : m_balls)
	{
		Vec2 accel = ball.m_force / ball.m_weight + GRAVITY;
		ball.m_speed += accel * seconds;

		ball.m_pos += ball.m_speed * seconds * PIXELS_PER_METER;

		ball.m_force = Vec2(0, 0);

		resolveCollisions();
	}
}

void PhysicsSimulation::resolveCollisions()
{
	for (Ball& ball : m_balls)
	{

		// check against left wall
		if (ball.m_pos.x() - CIRCLE_HALF_SIZE < 0)
		{
			ball.m_pos.x() = CIRCLE_HALF_SIZE;
			ball.m_speed.x() = -ball.m_speed.x() * BOUNCINESS;
		}
		// check against right wall
		if (ball.m_pos.x() + CIRCLE_HALF_SIZE > 800)
		{
			ball.m_pos.x() = 800 - CIRCLE_HALF_SIZE;
			ball.m_speed.x() = -ball.m_speed.x() * BOUNCINESS;
		}

		// check against top wall
		if (ball.m_pos.y() - CIRCLE_HALF_SIZE < 0)
		{
			ball.m_pos.y() = CIRCLE_HALF_SIZE;
			ball.m_speed.y() = -ball.m_speed.y() * BOUNCINESS;
		}
		// check against bottom wall
		if (ball.m_pos.y() + CIRCLE_HALF_SIZE > 800)
		{
			ball.m_pos.y() = 800 - CIRCLE_HALF_SIZE;
			ball.m_speed.y() = -ball.m_speed.y() * BOUNCINESS;
		}
	}
}