
#ifndef MY_APPLICATION_H
#define MY_APPLICATION_H

#include "ezGfxPlusApplication.h"
#include "ezGfxPlusCanvas.h"
#include "ezGfxPlusEvents.h"

#include <vector>

class MyApplication : public ezGfxPlus::Application
{
public:
	MyApplication();
	~MyApplication();

	void draw(ezGfxPlus::Canvas& canvas);

	void keyPressed(const ezGfxPlus::KeyEvent& event);
	void mousePressed(const ezGfxPlus::MouseEvent& event);
	void mouseReleased(const ezGfxPlus::MouseEvent& event);
	void mouseMoved(const ezGfxPlus::MouseEvent& event);

private:
	std::vector<ezGfxPlus::Image> m_images;
};

#endif // MY_APPLICATION_H