
#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include "Vec2.h"
#include "Rect.h"

#define M_PI 3.14159265358979323846f  /* pi */
#define DEG_TO_RAD(degrees) ((degrees) * M_PI / 180.f)
#define RAD_TO_DEG(radians) ((radians) * 180.f / M_PI)

bool circleIntersectsRectangle(Vec2 circlePos, int circleRadius, Rect rect)
{
	int rectCenterX = rect.left + (rect.right - rect.left) / 2;
	int rectCenterY = rect.top + (rect.bottom - rect.top) / 2;
	
	int rectWidth = rect.right - rect.left;
	int rectHeight = rect.bottom - rect.top;

	Vec2 circleDistance( abs(circlePos.x() - rectCenterX),
						abs(circlePos.y() - rectCenterY));

	if (circleDistance.x() > (rectWidth / 2 + circleRadius)) { return false; }
	if (circleDistance.y() > (rectHeight / 2 + circleRadius)) { return false; }

	if (circleDistance.x() <= (rectWidth / 2)) { return true; }
	if (circleDistance.y() <= (rectHeight / 2)) { return true; }

	float cornerDistance_sq = (circleDistance.x() - rectWidth / 2) * (circleDistance.x() - rectWidth / 2) +
		(circleDistance.y() - rectHeight / 2) * (circleDistance.y() - rectHeight / 2);

	return (cornerDistance_sq <= (circleRadius * circleRadius));
}

void rotatePoint(float& x, float& y, float angle, float originX = 0, float originY = 0)
{
	float s = sin(DEG_TO_RAD(angle));
	float c = cos(DEG_TO_RAD(angle));

	x -= originX;
	y -= originY;

	// do the rotation
	int rotX = x * c - y * s;
	int rotY = x * s + y * c;

	x = rotX + originX;
	y = rotY + originY;
}

#endif // MATH_HELPERS_H