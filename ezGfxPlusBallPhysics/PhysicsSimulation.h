
#include "Vec2.h"
#include <vector>

class Ball
{
public:
	float m_weight;
	Vec2 m_pos;
	Vec2 m_speed;
	Vec2 m_force;
};

class PhysicsSimulation
{
public:
	PhysicsSimulation();

	void applyForce(Vec2 force);
	void advance(float seconds);

	void resolveCollisions();

public:
	std::vector<Ball> m_balls;
};
