
#ifndef RECT_H
#define RECT_H

struct Rect
{
	Rect()
	: left(0), right(0), top(0), bottom(0)
	{
	}

	Rect(int l, int t, int r, int b)
	: left(l), right(r), top(t), bottom(b)
	{
	}

	int left, top, right, bottom;
};

#endif // RECT_H