
#ifndef EZGFXPLUS_EVENTS_H
#define EZGFXPLUS_EVENTS_H

#include "ezGfxPlus.h"
#include "ezGfxPlusMath.h"

namespace ezGfxPlus
{
	enum class MouseButton
	{
		NoButton = 0, LeftButton = 1, MiddleButton = 2, RightButton = 4
	};

	enum class KeyboardModifier
	{
		NoModifier, ShiftModifier, ControlModifier, AltModifier
	};

	typedef std::bitset<8> MouseButtons;
	typedef std::bitset<8> KeyboardModifiers;

	class EZGFXPLUS_API MouseEvent
	{
	public:
		// returns the mouse button that triggered the event
		MouseButton button() const;

		// returns the state of all mouse buttons
		MouseButtons buttons() const;

		// returns the position of mouse cursor when event was generated
		Math::Point pos() const;
	};

	class EZGFXPLUS_API KeyEvent
	{
	public:
		// returns the key that triggered the event
		int key() const;

		// returns the state of keyboard modifiers (Shift, Control, Alt)
		KeyboardModifiers modifiers() const;
	};
}

#endif // EZGFXPLUS_EVENTS_H