
#ifndef EZGFXPLUS_MATH_H
#define EZGFXPLUS_MATH_H

#include "ezGfxPlus.h"

namespace ezGfxPlus
{
	namespace Math
	{
		class EZGFXPLUS_API Point
		{
		public:
			Point();
			Point(int x, int y);

			int x, y;
		};


		class EZGFXPLUS_API Rect
		{
		public:
			Rect();
			Rect(int x, int y, int width, int height);

			bool contains(const Point& pt) const;
			bool contains(const Rect& rect);

			Point topLeft() const;
			Point topRight() const;
			Point bottomLeft() const;
			Point bottomRight() const;

			int x, y;
			int width, height;
		};


		EZGFXPLUS_API int randomNumber(int rangeMin, int rangeMax);
		EZGFXPLUS_API float randomNumber(float rangeMin, float rangeMax);
		EZGFXPLUS_API bool randomBool();
	}
}

#endif // EZGFXPLUS_MATH_H