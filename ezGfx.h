
#ifndef EZGFX_H
#define EZGFX_H

#ifndef EZGFX_STATIC
#	ifdef EZGFX_EXPORTS
#		define EZGFX_API __declspec(dllexport)
#	else
#		define EZGFX_API __declspec(dllimport)
#	endif
#else
#	define EZGFX_API
#endif

// Mouse buttons (can be OR-ed together)
#define EZGFX_NO_BUTTON		0
#define EZGFX_LEFT_BUTTON	1
#define EZGFX_MIDDLE_BUTTON 2
#define EZGFX_RIGHT_BUTTON	4
// Mouse state
#define EZGFX_RELEASED		0
#define EZGFX_PRESSED		1
#define EZGFX_MOVED			2

typedef void(*CallbackFunc)(void);
typedef void(*TimerCallbackFunc)(int);
typedef void(*MouseCallbackFunc)(int, int, int, int, int);
typedef void(*KeyboardCallbackFunc)(int, int);
typedef struct ezGfxImage* ezGfxImageRef;


// Initialization
// Sets up number of callback functions that the library will use to call the application code
EZGFX_API bool ezGfxInit(CallbackFunc displayFunc, CallbackFunc idleFunc, KeyboardCallbackFunc keyBoardFunc, MouseCallbackFunc mouseFunc);
EZGFX_API bool ezGfxInitWindowParameters(int width, int height, const char *title);
EZGFX_API void ezGfxMainLoop();

// Window update
EZGFX_API void ezGfxUpdate();

// Timing stuff
EZGFX_API void ezGfxTimerFunc(int timerId, int msec, TimerCallbackFunc timerFunc);
EZGFX_API float ezGfxGetElapsed();

// Clear screen
EZGFX_API void ezGfxSetClearColor(int colorR, int colorG, int colorB);
EZGFX_API void ezGfxClear();

// Stroke and fill parameters
EZGFX_API void ezGfxSetStrokeParameters(int thickness, int colorR, int colorG, int colorB);
EZGFX_API void ezGfxSetFillParameters(int colorR, int colorG, int colorB);

// Shape drawing
EZGFX_API void ezGfxMoveTo(int x, int y);
EZGFX_API void ezGfxLineTo(int x, int y);
EZGFX_API void ezGfxDrawRectangle(int x, int y, int width, int height);
EZGFX_API void ezGfxDrawRoundedRectangle(int x, int y, int width, int height, int cornerRadius);
EZGFX_API void ezGfxDrawEllipse(int x, int y, int width, int height);

// Text drawing
EZGFX_API void ezGfxSetFontParameters(const char* name, int size, bool bold);
EZGFX_API void ezGfxSetTextColor(int colorR, int colorG, int colorB);
EZGFX_API void ezGfxDrawText(int x, int y, const char *text);

// Pixel manipulation
EZGFX_API void ezGfxGetPixel(int x, int y, int& colorR, int& colorG, int& colorB);
EZGFX_API void ezGfxSetPixel(int x, int y, int colorR, int colorG, int colorB);
EZGFX_API unsigned char* ezGfxGetPixels(int startLine, int numLines);
EZGFX_API void ezGfxUpdatePixels(int startLine, int numLines);

// Image support
EZGFX_API ezGfxImageRef ezGfxLoadImage(const char* filePath);
EZGFX_API void ezGfxReleaseImage(ezGfxImageRef imageRef);
EZGFX_API int ezGfxGetImageWidth(ezGfxImageRef imageRef);
EZGFX_API int ezGfxGetImageHeight(ezGfxImageRef imageRef);
EZGFX_API void ezGfxDrawImage(ezGfxImageRef imageRef, int x, int y);
EZGFX_API void ezGfxDrawImage(ezGfxImageRef imageRef, int destX, int destY, int destWidth, int destHeight,
														int srcX, int srcY, int srcWidth, int srcHeight);
EZGFX_API void ezGfxBlendImage(ezGfxImageRef imageRef, int x, int y);
EZGFX_API void ezGfxBlendImage(ezGfxImageRef imageRef, int destX, int destY, int destWidth, int destHeight,
														int srcX, int srcY, int srcWidth, int srcHeight);

// Math functions
EZGFX_API int ezGfxMathRandom(int rangeMin, int rangeMax);
EZGFX_API float ezGfxMathRandom(float rangeMin, float rangeMax);
EZGFX_API bool ezGfxMathRandomBool();

#endif // EZGFX_H