
#ifndef EZGFXPLUS_APPLICATION_H
#define EZGFXPLUS_APPLICATION_H

#include "ezGfxPlus.h"

namespace ezGfxPlus
{
	class Canvas;
	class KeyEvent;
	class MouseEvent;

	class EZGFXPLUS_API Application
	{
	public:
		Application(int windowWidth, int windowHeight, const std::string& windowTitle);
		~Application();

		// main run loop of the application, this call will block execution until the app is terminated
		void run();
		
		// request screen update
		void update();

		// timing stuff
		void addTimer(int timerId, int msec);
		float getElapsedTime();

		// called when application is idling (nothing to draw, no user input etc.)
		virtual void idle();

		virtual void draw(Canvas& canvas) = 0;

		// timer event
		virtual void timerTicked(int timerId);

		// keyboard events 
		virtual void keyPressed(const KeyEvent& event);
		virtual void keyReleased(const KeyEvent& event);

		// mouse events
		virtual void mousePressed(const MouseEvent& event);
		virtual void mouseReleased(const MouseEvent& event);
		virtual void mouseMoved(const MouseEvent& event);
	};
}

#endif // EZGFXPLUS_APPLICATION_H
