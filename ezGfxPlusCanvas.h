
#ifndef EZGFXPLUS_CANVAS_H
#define EZGFXPLUS_CANVAS_H

#include "ezGfxPlusImage.h"
#include "ezGfxPlusMath.h"

namespace ezGfxPlus
{
	class EZGFXPLUS_API Canvas
	{
	public:
		// Clear screen
		void setClearColor(int colorR, int colorG, int colorB);
		void clear();

		// Stroke and fill parameters
		void setStrokeParameters(int thickness, int colorR, int colorG, int colorB);
		void setFillParameters(int colorR, int colorG, int colorB);

		// Shape drawing
		void moveTo(int x, int y);
		void lineTo(int x, int y);
		void drawRectangle(int x, int y, int width, int height);
		void drawRoundedRectangle(int x, int y, int width, int height, int cornerRadius);
		void drawEllipse(int x, int y, int width, int height);

		// Text drawing
		void setFontParameters(const char* name, int size, bool bold);
		void setTextColor(int colorR, int colorG, int colorB);
		void drawText(int x, int y, const char *text);

		// Pixel manipulation
		void getPixel(int x, int y, int& colorR, int& colorG, int& colorB);
		void setPixel(int x, int y, int colorR, int colorG, int colorB);
		unsigned char* getPixels(int startLine, int numLines);
		void updatePixels(int startLine, int numLines);

		// Image support
		// First two methods will not stretch or shrink the image.
		// Third method might stretch or based on the rects supplied
		void drawImage(const Image& image, int x, int y, bool blend = false);
		void drawImage(const Image& image, int x, int y, const Math::Rect& srcRect, bool blend = false);
		void drawImage(const Image& image, const Math::Rect& destRect, const Math::Rect& srcRect, bool blend = false);

	private:
		friend class Application;
		Canvas();
	};
}

#endif // EZGFXPLUS_CANVAS_H