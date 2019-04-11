#ifndef LCDWATCH_H
#define LCDWATCH_H

#include "ezGfxPlusCanvas.h"
#include "ezGfxPlusImage.h"
#include <vector>


class LCDWatch
{
public:
	// default constructor. will initialize time to 12 : 00 : 00
	LCDWatch();


	// another constructor to initialize with the given time
	LCDWatch(int hrs, int mins, int secs);

	// draws LCD watch on the screen
	void draw(ezGfxPlus::Canvas& canvas);

	void print();
	void tick();
	
	// resets time back to 12 : 00 :00
	void reset();
	
private:

	void drawNumber(ezGfxPlus::Canvas& canvas, int hour, int minute, int second);

	int m_hour;
	int m_minute;
	int m_second;
	std::vector<ezGfxPlus::Image*> m_digitImages;
	
};

#endif // LCDWATCH_H