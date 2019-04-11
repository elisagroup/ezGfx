
#include "ezGfxPlusEvents.h"

using namespace ezGfxPlus;
using namespace ezGfxPlus::Math;


// global shared variables
MouseButton g_lastMouseButton;
MouseButtons g_lastMouseButtons;
Point g_lastMousePos;
int g_lastKey;
KeyboardModifiers g_lastKeyModifiers;


MouseButton MouseEvent::button() const
{
	return g_lastMouseButton;
}

MouseButtons MouseEvent::buttons() const
{
	return g_lastMouseButtons;
}

Point MouseEvent::pos() const
{
	return g_lastMousePos;
}

int KeyEvent::key() const
{
	return g_lastKey;
}

KeyboardModifiers KeyEvent::modifiers() const
{
	return g_lastKeyModifiers;
}
