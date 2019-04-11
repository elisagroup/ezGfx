
#include "MyApplication.h"
#include "ezGfxPlusMath.h"

#include <iostream>

using namespace std;
using namespace ezGfxPlus;
using namespace ezGfxPlus::Math;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800


MyApplication::MyApplication()
: Application(WINDOW_WIDTH, WINDOW_HEIGHT, "ezGfxPlus Test")
{
    m_images.push_back(Image("horse.png"));
    m_images.push_back(Image("banana.png"));
}

MyApplication::~MyApplication()
{
}

void MyApplication::draw(Canvas& canvas)
{
	canvas.setClearColor(0, 0, 255);
	canvas.clear();

    for (int i = 0; i < m_images.size(); i++)
    {
        canvas.drawImage(m_images[i], 0, 0, Rect(0, 0, m_images[i].getWidth(), m_images[i].getHeight()), true);
    }
}

void MyApplication::mousePressed(const MouseEvent& event)
{
	cout << "pressed" << endl;
}

void MyApplication::mouseReleased(const MouseEvent& event)
{
	cout << "released" << endl;
}

void MyApplication::mouseMoved(const MouseEvent& event)
{
	cout << "event.button = " << (int)event.button() << endl;

	if (event.button() == MouseButton::LeftButton)
	{
		cout << "mouse moved and left button is held" << endl;
	}

	//cout << "moved" << endl;
}

void MyApplication::keyPressed(const KeyEvent& event)
{
	char key = event.key();
	cout << "you pressed" << key << endl;
}