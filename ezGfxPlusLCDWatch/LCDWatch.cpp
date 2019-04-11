#include "LCDWatch.h"
#include <iostream>

using namespace std;
using namespace ezGfxPlus;


LCDWatch::LCDWatch()
{
	reset();

	for (int i = 0; i < 11; i++)
	{
		Image* mydigit = new Image;
		mydigit->load("font\\" + to_string(i) + ".png");

		m_digitImages.push_back(mydigit);
	}
}

LCDWatch::LCDWatch(int hrs, int mins, int secs)
{

}

void LCDWatch::drawNumber(Canvas& canvas, int hour, int minute, int second)
{

	int htens = hour / 10;
	int hones = hour % 10;

	int mtens = minute / 10;
	int mones = minute % 10;

	int tens = second / 10;
	int ones = second % 10;

	canvas.drawImage(*m_digitImages[htens], 10, 300);
	canvas.drawImage(*m_digitImages[hones], 130, 300);

	canvas.drawImage(*m_digitImages[10], 250, 300);

	canvas.drawImage(*m_digitImages[mtens], 280, 300);
	canvas.drawImage(*m_digitImages[mones], 400, 300);

	canvas.drawImage(*m_digitImages[10], 520, 300);

	canvas.drawImage(*m_digitImages[tens], 550, 300);
	canvas.drawImage(*m_digitImages[ones], 670, 300);

}

void LCDWatch::draw(Canvas& canvas)
{
	canvas.setClearColor(255, 255, 255);
	canvas.clear();

	drawNumber(canvas, m_hour, m_minute, m_second);
}

void LCDWatch::print()
{
	cout << m_hour << ":" << m_minute << ":" << m_second << endl;
}

void LCDWatch::tick()
{
	m_second++;
	
	if (m_second > 59)
	{
		m_minute++;
		m_second = 0;
	}
	if (m_minute > 59)
	{
		m_hour++;
		m_minute = 0;
	}
	if (m_hour == 24)
		m_hour = 0;

	print();
}

void LCDWatch::reset()
{
	m_second = 0;
	m_minute = 0;
	m_hour = 12;

	print();
}