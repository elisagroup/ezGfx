// ezGfxMeelyFSM.cpp : Defines the entry point for the console application.
//

#include "ezGfx.h"
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define FIRST_LED 1
#define SECOND_LED 2
#define THIRD_LED 4
#define FOURTH_LED 8

#define DIR_LEFT 0x80
#define DIR_RIGHT 0x00

struct State
{
	// Our input is a direction.
	// Bits for the output are assigned as follows:
	// D 0 0 0 L4 L3 L2 L1 where
	// D = directon bit, L1, L2, L3, L4 = four LEDs
	unsigned char output[2];

	// Next state defined based on input (direction)
	unsigned char next[2];
};

// state transition table
State g_stateTable[4] = {
	//   output when input = 0,  output when input = 1,  new states
	{ { SECOND_LED + DIR_RIGHT, FIRST_LED + DIR_RIGHT }, { 1, 0 } },
	{ { THIRD_LED + DIR_RIGHT, FIRST_LED + DIR_RIGHT }, { 2, 0 } },
	{ { FOURTH_LED + DIR_LEFT, SECOND_LED + DIR_LEFT }, { 3, 1 } },
	{ { FOURTH_LED + DIR_LEFT, THIRD_LED + DIR_LEFT }, { 3, 2 } },
};

int g_currentState;
char g_lastOutput;
char g_dir;

void drawLED(int index, bool on)
{
	if (on)
		ezGfxSetFillParameters(255, 0, 0);
	else
		ezGfxSetFillParameters(0, 0, 0);
	
	ezGfxDrawEllipse(WINDOW_WIDTH / 2 - 100 + index * 80, WINDOW_HEIGHT / 2, 50, 50);
}

void drawGraphics()
{
	ezGfxClear();

	bool firstOn = g_lastOutput & FIRST_LED;
	bool secondOn = g_lastOutput & SECOND_LED;
	bool thirdOn = g_lastOutput & THIRD_LED;
	bool fourthOn = g_lastOutput & FOURTH_LED;

	drawLED(0, firstOn);
	drawLED(1, secondOn);
	drawLED(2, thirdOn);
	drawLED(3, fourthOn);
}

void timerFunc(int timerId)
{
	State currState = g_stateTable[g_currentState];
	// get output from state transition
	g_lastOutput = currState.output[g_dir];
	// do the actual state transition
	g_currentState = currState.next[g_dir];
	// get new direction which will be the new input in next iterations
	g_dir = g_lastOutput & 0x80 ? 1 : 0;

	ezGfxUpdate();
}

void initFSM()
{
	g_currentState = 0;
	g_dir = 0;
	g_lastOutput = 0x01;
}

int main(int argc, char* argv[])
{
	ezGfxInit(drawGraphics, nullptr, nullptr, nullptr);
	ezGfxInitWindowParameters(WINDOW_WIDTH, WINDOW_HEIGHT, "LED animation - Meely FSM");

	ezGfxTimerFunc(0, 200, timerFunc);

	initFSM();

	ezGfxMainLoop();

	return 0;
}
