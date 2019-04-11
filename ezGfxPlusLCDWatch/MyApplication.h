
#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "ezGfxPlusApplication.h"
#include "ezGfxPlusCanvas.h"
#include "LCDWatch.h"


class MyApplication : public ezGfxPlus::Application
{
public:
	MyApplication();
	~MyApplication();

	void draw(ezGfxPlus::Canvas& canvas);

	void timerTicked(int timerId);

	void keyPressed(const ezGfxPlus::KeyEvent& event);

private:
	LCDWatch m_watch;

};

#endif // MYAPPLICATION_H