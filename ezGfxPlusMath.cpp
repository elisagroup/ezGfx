
#include "ezGfxPlusMath.h"
#include "ezGfx.h"

using namespace ezGfxPlus;
using namespace ezGfxPlus::Math;


Point::Point()
{
	this->x = 0; this->y = 0;
}

Point::Point(int x, int y)
{
	this->x = x; this->y = y;
}


Rect::Rect()
{
	this->x = this->y = 0;
	this->width = this->height = 0;
}

Rect::Rect(int x, int y, int width, int height)
{
	this->x = x; this->y = y;
	this->width = width; this->height = height;
}

bool Rect::contains(const Point& pt) const
{
	if (pt.x >= this->x && pt.x < this->x + this->width &&
		pt.y >= this->y && pt.y < this->y + this->height)
	{
		return true;
	}
	return false;
}

bool Rect::contains(const Rect& rect)
{
	if (rect.x >= this->x && rect.x < this->x + this->width &&
		rect.y >= this->y && rect.y < this->y + this->height)
	{
		return true;
	}
	return false;
}

Point Rect::topLeft() const
{
	return Point(x, y);
}

Point Rect::topRight() const
{
	return Point(x + width - 1, y);
}

Point Rect::bottomLeft() const
{
	return Point(x, y + height - 1);
}

Point Rect::bottomRight() const
{
	return Point(x + width - 1, y + height - 1);
}


int Math::randomNumber(int rangeMin, int rangeMax)
{
	return ezGfxMathRandom(rangeMin, rangeMax);
}

float Math::randomNumber(float rangeMin, float rangeMax)
{
	return ezGfxMathRandom(rangeMin, rangeMax);
}

bool Math::randomBool()
{
	return ezGfxMathRandomBool();
}
