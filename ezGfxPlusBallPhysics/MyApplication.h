
#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "ezGfxPlusApplication.h"
#include "ezGfxPlusCanvas.h"
#include "ezGfxPlusEvents.h"

#include "PhysicsSimulation.h"

class MyApplication : public ezGfxPlus::Application
{
public:
	MyApplication();
	~MyApplication();

	void idle();

	void draw(ezGfxPlus::Canvas& canvas);

	void keyPressed(const ezGfxPlus::KeyEvent& event);
	void mousePressed(const ezGfxPlus::MouseEvent& event);

private:
	PhysicsSimulation m_simulator;
};

#endif // MYAPPLICATION_H