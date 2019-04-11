
#include "MyApplication.h"
#include "ezGfxPlusEvents.h"

#include <iostream>

using namespace ezGfxPlus;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


MyApplication::MyApplication()
: Application(WINDOW_WIDTH, WINDOW_HEIGHT, "ezGfxPlus Watch")
{
	addTimer(0, 1000);
}

MyApplication::~MyApplication()
{
}

void MyApplication::draw(Canvas& canvas)
{
	m_watch.draw(canvas);
}

void MyApplication::timerTicked(int timerID)
{
	m_watch.tick();
	update();
}

void MyApplication::keyPressed(const KeyEvent& event)
{
	if (event.key() == ' ')
	{
		m_watch.reset();
		update();
	}
}

