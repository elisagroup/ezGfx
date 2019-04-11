
#include "ezGfxPlusApplication.h"
#include "ezGfxPlusCanvas.h"
#include "ezGfxPlusEvents.h"
#include "ezGfx.h"

using namespace ezGfxPlus;
using namespace ezGfxPlus::Math;

Application* g_app = nullptr;
Canvas* g_canvas;

extern MouseButton g_lastMouseButton;
extern MouseButtons g_lastMouseButtons;
extern Point g_lastMousePos;
extern int g_lastKey;
extern KeyboardModifiers g_lastKeyModifiers;


void drawCallback()
{
	g_app->draw(*g_canvas);
}

void idleCallback()
{
	g_app->idle();
}

void timerCallback(int timerId)
{
	g_app->timerTicked(timerId);
}

void keyboardCallback(int key, int state)
{
	// We use this sort of hack to hide implementation details in header file
	KeyEvent event;
	g_lastKey = key;

	if (state == EZGFX_PRESSED)
		g_app->keyPressed(event);
	else if (state == EZGFX_RELEASED)
		g_app->keyReleased(event);
}

void mouseCallback(int button, int buttons, int state, int x, int y)
{
	// We use this sort of hack to hide implementation details in header file
	MouseEvent event;
	g_lastMouseButton = static_cast<MouseButton>(button);
	g_lastMouseButtons = buttons;
	g_lastMousePos.x = x; g_lastMousePos.y = y;

	if (state == EZGFX_PRESSED)
		g_app->mousePressed(event);
	else if (state == EZGFX_RELEASED)
		g_app->mouseReleased(event);
	else if (state == EZGFX_MOVED)
		g_app->mouseMoved(event);
}

Application::Application(int windowWidth, int windowHeight, const std::string& windowTitle)
{
	if (!g_app)
	{
		g_app = this;

		g_canvas = new Canvas;

		ezGfxInit(drawCallback, idleCallback, keyboardCallback, mouseCallback);
		ezGfxInitWindowParameters(windowWidth, windowHeight, windowTitle.c_str());
	}
}

Application::~Application()
{

}

void enableMouseTracking(bool enable)
{

}

void Application::run()
{
	if (g_app)
		ezGfxMainLoop();
}

void Application::update()
{
	ezGfxUpdate();
}

void Application::addTimer(int timerId, int msec)
{
	ezGfxTimerFunc(timerId, msec, timerCallback);
}

float Application::getElapsedTime()
{
	return ezGfxGetElapsed();
}

void Application::idle()
{
	// do nothing
}

void Application::draw(Canvas& canvas)
{
	// do nothing
}

void Application::timerTicked(int timerId)
{
	// do nothing
}

void Application::keyPressed(const KeyEvent& event)
{
	// do nothing
}

void Application::keyReleased(const KeyEvent& event)
{
	// do nothing
}

void Application::mousePressed(const MouseEvent& event)
{
	// do nothing
}

void Application::mouseReleased(const MouseEvent& event)
{
	// do nothing
}

void Application::mouseMoved(const MouseEvent& event)
{
	// do nothing
}
