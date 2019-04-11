
#include "MyApplication.h"

#include <iostream>

using namespace std;
using namespace ezGfxPlus;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define CIRCLE_SIZE 50


MyApplication::MyApplication()
: Application(WINDOW_WIDTH, WINDOW_HEIGHT, "ezGfxPlus Test")
{
}


MyApplication::~MyApplication()
{
}

void MyApplication::idle()
{
	float elapsed = getElapsedTime();
	m_simulator.advance(elapsed);
	update();
}

void MyApplication::draw(Canvas& canvas)
{
	canvas.clear();

	canvas.setStrokeParameters(1, 0, 255, 0);
	canvas.setFillParameters(0, 255, 0);

	for (Ball& ball : m_simulator.m_balls)
	{
		canvas.drawEllipse(ball.m_pos.x() - CIRCLE_SIZE / 2, ball.m_pos.y() - CIRCLE_SIZE / 2, CIRCLE_SIZE, CIRCLE_SIZE);
	}
}

void MyApplication::mousePressed(const MouseEvent& event)
{
}

void MyApplication::keyPressed(const KeyEvent& event)
{
	if (event.key() == 'D')
	{
		m_simulator.applyForce(Vec2(500, 0));
	}
	else if (event.key() == 'A')
	{
		m_simulator.applyForce(Vec2(-500, 0));
	}
	else if (event.key() == ' ')
	{
		m_simulator.applyForce(Vec2(0, -10000));
	}
}
