
#include "ezGfxPlusCanvas.h"
#include "ezGfx.h"

using namespace ezGfxPlus;
using namespace ezGfxPlus::Math;

Canvas::Canvas()
{

}

void Canvas::setClearColor(int colorR, int colorG, int colorB)
{
	ezGfxSetClearColor(colorR, colorG, colorB);
}

void Canvas::clear()
{
	ezGfxClear();
}

void Canvas::setStrokeParameters(int thickness, int colorR, int colorG, int colorB)
{
	ezGfxSetStrokeParameters(thickness, colorR, colorG, colorB);
}

void Canvas::setFillParameters(int colorR, int colorG, int colorB)
{
	ezGfxSetFillParameters(colorR, colorG, colorB);
}

void Canvas::moveTo(int x, int y)
{
	ezGfxMoveTo(x, y);
}

void Canvas::lineTo(int x, int y)
{
	ezGfxLineTo(x, y);
}

void Canvas::drawRectangle(int x, int y, int width, int height)
{
	ezGfxDrawRectangle(x, y, width, height);
}

void Canvas::drawRoundedRectangle(int x, int y, int width, int height, int cornerRadius)
{
	ezGfxDrawRoundedRectangle(x, y, width, height, cornerRadius);
}

void Canvas::drawEllipse(int x, int y, int width, int height)
{
	ezGfxDrawEllipse(x, y, width, height);
}

void Canvas::setFontParameters(const char* name, int size, bool bold)
{
	ezGfxSetFontParameters(name, size, bold);
}

void Canvas::setTextColor(int colorR, int colorG, int colorB)
{
	ezGfxSetTextColor(colorR, colorG, colorB);
}

void Canvas::drawText(int x, int y, const char *text)
{
	ezGfxDrawText(x, y, text);
}

void Canvas::getPixel(int x, int y, int& colorR, int& colorG, int& colorB)
{
	ezGfxGetPixel(x, y, colorR, colorG, colorB);
}

void Canvas::setPixel(int x, int y, int colorR, int colorG, int colorB)
{
	ezGfxSetPixel(x, y, colorR, colorG, colorB);
}

unsigned char* Canvas::getPixels(int startLine, int numLines)
{
	return ezGfxGetPixels(startLine, numLines);
}

void Canvas::updatePixels(int startLine, int numLines)
{
	ezGfxUpdatePixels(startLine, numLines);
}

void Canvas::drawImage(const Image& image, int x, int y, bool blend)
{
	ezGfxImageRef imageRef = static_cast<ezGfxImageRef>(image.m_imageRef);
	if (blend)
		ezGfxBlendImage(imageRef, x, y);
	else
		ezGfxDrawImage(imageRef, x, y);
}

void Canvas::drawImage(const Image& image, int x, int y, const Math::Rect& srcRect, bool blend)
{
	drawImage(image, Rect(x, y, srcRect.width, srcRect.height), srcRect, blend);
}

void Canvas::drawImage(const Image& image, const Rect& destRect, const Rect& srcRect, bool blend)
{
	ezGfxImageRef imageRef = static_cast<ezGfxImageRef>(image.m_imageRef);
	if (blend)
		ezGfxBlendImage(imageRef, destRect.x, destRect.y, destRect.width, destRect.height, srcRect.x, srcRect.y, srcRect.width, srcRect.height);
	else
		ezGfxDrawImage(imageRef, destRect.x, destRect.y, destRect.width, destRect.height, srcRect.x, srcRect.y, srcRect.width, srcRect.height);
}