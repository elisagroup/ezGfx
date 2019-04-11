// ezGfx.cpp : Defines the exported functions for the DLL application.
//

#include "ezGfx.h"
#include "Helpers/xHPTimer.h"
#include "Helpers/xImage.h"

#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <map>

HINSTANCE	g_app_hModule;
HWND		g_hWnd;
int			g_windowWidth;
int			g_windowHeight;
char		g_windowTitle[255];

// GDI object handles
HDC			g_memHdc = NULL;
HBITMAP		g_memBitmap = NULL;
HPEN		g_currentPenHandle = NULL;
HBRUSH		g_currentBrushHandle = NULL;
HBRUSH		g_screenClearBrush = NULL;
HFONT		g_currentFont = NULL;
BITMAPINFO	g_memBitmapInfo;
unsigned char* g_memPixels = NULL;

bool g_mainLoopRunning = false;

// Callbacks
CallbackFunc g_displayFunc = NULL, g_idleFunc = NULL;
KeyboardCallbackFunc g_keyboardFunc = NULL;
MouseCallbackFunc g_mouseFunc = NULL;

struct TimerEntry
{
	int timerId;
	int timerDelayMs;
	TimerCallbackFunc timerCallback;
};

std::map<unsigned int, TimerEntry> g_timers;

xHPTimer g_HpTimer;

struct ezGfxImage
{
	xImage xImage;
};

LRESULT CALLBACK DLLWindowProc(HWND, UINT, WPARAM, LPARAM);


BOOL RegisterDLLWindowClass(wchar_t szClassName[])
{
	WNDCLASSEX wc;
	wc.hInstance = g_app_hModule;
	wc.lpszClassName = (LPCWSTR)L"ezGfxWindowClass";
	wc.lpszClassName = (LPCWSTR)szClassName;
	wc.lpfnWndProc = DLLWindowProc;
	wc.style = CS_DBLCLKS;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	if (!RegisterClassEx(&wc))
		return 0;

	return TRUE;
}

int parseAllButtonsState(WPARAM wParam)
{
	int buttons = 0;
	if (wParam & MK_LBUTTON)
		buttons |= EZGFX_LEFT_BUTTON;
	if (wParam & MK_MBUTTON)
		buttons |= EZGFX_MIDDLE_BUTTON;
	if (wParam & MK_RBUTTON)
		buttons |= EZGFX_RIGHT_BUTTON;

	return buttons;
}

LRESULT CALLBACK DLLWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ERASEBKGND:
		return (LRESULT)1; // Flicker-free drawing
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT clientRect;
		GetClientRect(hWnd, &clientRect);
		int clientWidth = clientRect.right - clientRect.left;
		int clientHeight = clientRect.bottom + clientRect.left;

		// call the host application to draw frame
		if (g_displayFunc)
			g_displayFunc();

		BitBlt(hdc, 0, 0, clientWidth, clientHeight, g_memHdc, 0, 0, SRCCOPY);
		
		DeleteDC(hdc);
		EndPaint(hWnd, &ps);
		
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	{
		if (g_mouseFunc)
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);

			int buttons = parseAllButtonsState(wParam);

			// this will hold the button that triggered the event
			int button = 0;

			if (message == WM_LBUTTONDOWN || message == WM_LBUTTONUP)
				button = EZGFX_LEFT_BUTTON;
			else if (message == WM_MBUTTONDOWN || message == WM_MBUTTONUP)
				button = EZGFX_MIDDLE_BUTTON;
			else if (message == WM_RBUTTONDOWN || message == WM_RBUTTONUP)
				button = EZGFX_RIGHT_BUTTON;
			
			int mouseState = (message == WM_LBUTTONDOWN ||
							message == WM_MBUTTONDOWN ||
							message == WM_RBUTTONDOWN) ? EZGFX_PRESSED : EZGFX_RELEASED;
			if (message == WM_MOUSEMOVE)
				mouseState |= EZGFX_MOVED;

			g_mouseFunc(button, buttons, mouseState, xPos, yPos);
		}

		break;
	}
	case WM_KEYDOWN:
	{
		if (g_keyboardFunc)
			g_keyboardFunc(wParam, EZGFX_PRESSED);
		
		break;
	}
	case WM_KEYUP:
	{
		if (g_keyboardFunc)
			g_keyboardFunc(wParam, EZGFX_RELEASED);

		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void initializeDll(HMODULE hModule)
{
	g_app_hModule = hModule;

	// initialize random number generator
	srand(static_cast<unsigned int>(time(NULL)));
}

void destroyDll()
{
	if (g_currentPenHandle)
		DeleteObject(g_currentPenHandle);
	if (g_currentBrushHandle)
		DeleteObject(g_currentBrushHandle);
	if (g_screenClearBrush)
		DeleteObject(g_screenClearBrush);
	if (g_memBitmap)
		DeleteObject(g_memBitmap);
	if (g_memHdc)
		DeleteDC(g_memHdc);
	
	if (g_memPixels)
		delete g_memPixels;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		initializeDll(hModule);
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		destroyDll();
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

void createOffscreenBitmap(HWND hWnd)
{
	RECT clientRect;
	GetClientRect(hWnd, &clientRect);
	int clientWidth = clientRect.right - clientRect.left;
	int clientHeight = clientRect.bottom + clientRect.left;

	HDC hDC = GetDC(hWnd);
	g_memHdc = CreateCompatibleDC(hDC);
	g_memBitmap = CreateCompatibleBitmap(hDC, clientWidth, clientHeight);
	SelectObject(g_memHdc, g_memBitmap);

	// Set defaults
	SelectObject(g_memHdc, (HBRUSH)GetStockObject(NULL_BRUSH));
	SelectObject(g_memHdc, (HPEN)GetStockObject(WHITE_PEN));
	SetTextColor(g_memHdc, RGB(255, 255, 255));

	ReleaseDC(hWnd, hDC);

	// allocate memory to hold pixels for GetPixels/SetPixels calls
	ZeroMemory(&g_memBitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER));
	g_memBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GetDIBits(g_memHdc, g_memBitmap, 0, clientHeight, NULL, &g_memBitmapInfo, DIB_RGB_COLORS);
	
	// we need to fix the compression parameter and make sure bitmap will be flipped vertically
	g_memBitmapInfo.bmiHeader.biCompression = BI_RGB;
	g_memBitmapInfo.bmiHeader.biHeight = -g_memBitmapInfo.bmiHeader.biHeight;
	
	g_memPixels = new unsigned char[g_memBitmapInfo.bmiHeader.biSizeImage];

	g_hWnd = hWnd;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	auto iter = g_timers.find(idTimer);
	if (iter != g_timers.end() && iter->second.timerCallback)
		iter->second.timerCallback(iter->second.timerId);
}

void initializeTimers(HWND hWnd)
{
	auto iter = g_timers.cbegin();
	auto iterEnd = g_timers.cend();

	for (; iter != iterEnd; ++iter)
	{
		SetTimer(hWnd, iter->first, iter->second.timerDelayMs, TimerProc);
	}
}

bool ezGfxInit(CallbackFunc displayFunc, CallbackFunc idleFunc, KeyboardCallbackFunc keyBoardFunc, MouseCallbackFunc mouseFunc)
{
	g_displayFunc = displayFunc;
	g_idleFunc = idleFunc;
	g_keyboardFunc = keyBoardFunc;
	g_mouseFunc = mouseFunc;

	return true;
}

bool ezGfxInitWindowParameters(int width, int height, const char *title)
{
	g_windowWidth = width;
	g_windowHeight = height;
	strcpy_s(g_windowTitle, 100, title);

	return true;
}

void ezGfxUpdate()
{
	if (g_hWnd)
		InvalidateRect(g_hWnd, NULL, FALSE);
}

void ezGfxMainLoop()
{
	g_mainLoopRunning = true;

	RegisterDLLWindowClass(L"ezGfxWindowClass");

	// calculate window size based on the requested client area
	RECT clientRect = { 0, 0, g_windowWidth, g_windowHeight };
	AdjustWindowRectEx(&clientRect, WS_CAPTION, FALSE, WS_EX_PALETTEWINDOW);

	// get adjusted width and height
	g_windowWidth = clientRect.right - clientRect.left;
	g_windowHeight = clientRect.bottom - clientRect.top;

	HWND hWnd = CreateWindowExA(WS_EX_PALETTEWINDOW, "ezGfxWindowClass", g_windowTitle, WS_CAPTION, CW_USEDEFAULT, CW_USEDEFAULT,
		g_windowWidth, g_windowHeight, NULL, NULL, g_app_hModule, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
	createOffscreenBitmap(hWnd);

	initializeTimers(hWnd);

	g_HpTimer.Init();

	MSG msg;
	BOOL msgRes;
	while (true)
	{
		msgRes = g_idleFunc ? PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) : GetMessage(&msg, hWnd, 0, 0);
		if (msgRes)
		{
			if (WM_QUIT == msg.message)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		
		if (g_idleFunc)
		{
			g_idleFunc();
		}
	}

	g_mainLoopRunning = false;
}

void ezGfxTimerFunc(int timerId, int msec, TimerCallbackFunc timerFunc)
{
	// Timer objects should be created before starting the main loop
	if (g_mainLoopRunning)
		return;

	TimerEntry entry;
	entry.timerId = timerId;
	entry.timerDelayMs = msec;
	entry.timerCallback = timerFunc;
	g_timers[timerId] = entry;
}

float ezGfxGetElapsed()
{
	return g_HpTimer.GetSeconds();
}

void ezGfxSetClearColor(int colorR, int colorG, int colorB)
{
	// delete old brush if exists
	if (g_screenClearBrush)
		DeleteObject(g_screenClearBrush);

	g_screenClearBrush = CreateSolidBrush(RGB(colorR, colorG, colorB));
}

void ezGfxClear()
{
	RECT rect = { 0, 0, g_memBitmapInfo.bmiHeader.biWidth, -g_memBitmapInfo.bmiHeader.biHeight };
	// black is the default color if clear color is not set explicitely
	FillRect(g_memHdc, &rect, g_screenClearBrush ? g_screenClearBrush : (HBRUSH)(GetStockObject(BLACK_BRUSH)));
}

void ezGfxSetStrokeParameters(int thickness, int colorR, int colorG, int colorB)
{
	// delete old pen if exists
	if (g_currentPenHandle)
		DeleteObject(g_currentPenHandle);

	g_currentPenHandle = CreatePen(PS_SOLID, thickness, RGB(colorR, colorG, colorB));
	SelectObject(g_memHdc, g_currentPenHandle);
}

void ezGfxSetFillParameters(int colorR, int colorG, int colorB)
{
	// delete old brush if exists
	if (g_currentBrushHandle)
		DeleteObject(g_currentBrushHandle);

	g_currentBrushHandle = CreateSolidBrush(RGB(colorR, colorG, colorB));
	SelectObject(g_memHdc, g_currentBrushHandle);
}

void ezGfxMoveTo(int x, int y)
{
	MoveToEx(g_memHdc, x, y, NULL);
}

void ezGfxLineTo(int x, int y)
{
	LineTo(g_memHdc, x, y);
}

void ezGfxDrawRectangle(int x, int y, int width, int height)
{
	Rectangle(g_memHdc, x, y, x + width, y + height);
}

void ezGfxDrawRoundedRectangle(int x, int y, int width, int height, int cornerRadius)
{
	RoundRect(g_memHdc, x, y, x + width, y + height, cornerRadius, cornerRadius);
}

void ezGfxDrawEllipse(int x, int y, int width, int height)
{
	Ellipse(g_memHdc, x, y, x + width, y + height);
}

void ezGfxSetTextColor(int colorR, int colorG, int colorB)
{
	SetTextColor(g_memHdc, RGB(colorR, colorG, colorB));
}

void ezGfxSetFontParameters(const char* name, int size, bool bold)
{
	// delete old font if exists
	if (g_currentFont)
		DeleteObject(g_currentFont);

	LOGFONTA logFont;
	memset(&logFont, 0, sizeof(logFont));
	logFont.lfHeight = -size;
	logFont.lfWeight = bold ? FW_BOLD : FW_NORMAL;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = ANTIALIASED_QUALITY;

	strcpy_s(logFont.lfFaceName, sizeof(logFont.lfFaceName), name);
	g_currentFont = CreateFontIndirectA(&logFont);
}

void ezGfxDrawText(int x, int y, const char *text)
{
	SetBkMode(g_memHdc, TRANSPARENT);

	if (g_currentFont)
		SelectObject(g_memHdc, g_currentFont);

	TextOutA(g_memHdc, x, y, text, strlen(text));
}

void ezGfxGetPixel(int x, int y, int& colorR, int& colorG, int& colorB)
{
	COLORREF pixel = GetPixel(g_memHdc, x, y);
	colorR = GetRValue(pixel);
	colorG = GetGValue(pixel);
	colorB = GetBValue(pixel); 
}

void ezGfxSetPixel(int x, int y, int colorR, int colorG, int colorB)
{
	SetPixel(g_memHdc, x, y, RGB(colorR, colorG, colorB));
}

unsigned char* ezGfxGetPixels(int startLine, int numLines)
{
	if (!GetDIBits(g_memHdc, g_memBitmap, startLine, numLines, g_memPixels, &g_memBitmapInfo, DIB_RGB_COLORS))
		return NULL;

	return g_memPixels;
}

void ezGfxUpdatePixels(int startLine, int numLines)
{
	SetDIBits(g_memHdc, g_memBitmap, startLine, numLines, (LPVOID)g_memPixels, &g_memBitmapInfo, DIB_RGB_COLORS);
}

ezGfxImageRef ezGfxLoadImage(const char* filePath)
{
	ezGfxImageRef imageRef = new ezGfxImage;
	if (!imageRef->xImage.Load(filePath, true, true))
		return 0;

	return imageRef;
}

void ezGfxReleaseImage(ezGfxImageRef imageRef)
{
	delete imageRef;
}

int ezGfxGetImageWidth(ezGfxImageRef imageRef)
{
	if (imageRef && imageRef->xImage.isLoaded())
		return imageRef->xImage.getWidth();

	return 0;
}

int ezGfxGetImageHeight(ezGfxImageRef imageRef)
{
	if (imageRef && imageRef->xImage.isLoaded())
		return imageRef->xImage.getHeight();

	return 0;
}

void ezGfxDrawImage(ezGfxImageRef imageRef, int x, int y)
{
	if (!imageRef)
		return;

	unsigned char *pixels = imageRef->xImage.getData();

	int w = imageRef->xImage.getWidth();
	int h = imageRef->xImage.getHeight();

	BITMAPINFO info;
	ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = w;
	info.bmiHeader.biHeight = -h;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = w * h * 4;
	info.bmiHeader.biCompression = BI_RGB;

	StretchDIBits(g_memHdc, x, y, w, h, 0, 0, w, h, pixels, &info, DIB_RGB_COLORS, SRCCOPY);
}

void ezGfxDrawImage(ezGfxImageRef imageRef, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight)
{
	if (!imageRef)
		return;

	unsigned char *pixels = imageRef->xImage.getData();

	int w = imageRef->xImage.getWidth();
	int h = imageRef->xImage.getHeight();

	BITMAPINFO info;
	ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = w;
	info.bmiHeader.biHeight = -h;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = w * h * 4;
	info.bmiHeader.biCompression = BI_RGB;
	
	// since the bitmap's Y coordinate is flipped (origin is at bottom left), we need to calculate srcY
	StretchDIBits(g_memHdc, destX, destY, destWidth, destHeight, srcX, h - (srcY + srcHeight), srcWidth, srcHeight, pixels, &info, DIB_RGB_COLORS, SRCCOPY);
}

void ezGfxBlendImage(ezGfxImageRef imageRef, int x, int y)
{
	unsigned char *pixels = imageRef->xImage.getData();

	int w = imageRef->xImage.getWidth();
	int h = imageRef->xImage.getHeight();

	BITMAPINFO info;
	ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = w;
	info.bmiHeader.biHeight = -h;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = w * h * 4;
	info.bmiHeader.biCompression = BI_RGB;

	// create a DC for our bitmap -- the source DC for AlphaBlend  
	HDC hdc = CreateCompatibleDC(g_memHdc);

	// create our DIB section and select the bitmap into the dc
	/*VOID *pvBits;
	HBITMAP hbitmap = CreateDIBSection(hdc, &info, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
	SelectObject(hdc, hbitmap);*/
	HBITMAP hbitmap = CreateCompatibleBitmap(g_memHdc, w, h);
	SetDIBits(hdc, hbitmap, 0, h, pixels, &info, DIB_RGB_COLORS);
	SelectObject(hdc, hbitmap);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha  
	bf.SourceConstantAlpha = 0xff;  // opaque (disable constant alpha) 

	if (!AlphaBlend(g_memHdc, x, y, w, h, hdc, 0, 0, w, h, bf))
		return;

	// do cleanup 
	DeleteObject(hbitmap);
	DeleteDC(hdc);
}

void ezGfxBlendImage(ezGfxImageRef imageRef, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight)
{
	unsigned char *pixels = imageRef->xImage.getData();

	int w = imageRef->xImage.getWidth();
	int h = imageRef->xImage.getHeight();

	BITMAPINFO info;
	ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biWidth = w;
	info.bmiHeader.biHeight = -h;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = w * h * 4;
	info.bmiHeader.biCompression = BI_RGB;

	// create a DC for our bitmap -- the source DC for AlphaBlend  
	HDC hdc = CreateCompatibleDC(g_memHdc);

	// create our DIB section and select the bitmap into the dc
	/*VOID *pvBits;
	HBITMAP hbitmap = CreateDIBSection(hdc, &info, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
	SelectObject(hdc, hbitmap);*/
	HBITMAP hbitmap = CreateCompatibleBitmap(g_memHdc, w, h);
	SetDIBits(hdc, hbitmap, 0, h, pixels, &info, DIB_RGB_COLORS);
	SelectObject(hdc, hbitmap);

	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;  // use source alpha  
	bf.SourceConstantAlpha = 0xff;  // opaque (disable constant alpha) 

	if (!AlphaBlend(g_memHdc, destX, destY, destWidth, destHeight, hdc, srcX, srcY, srcWidth, srcHeight, bf))
		return;

	// do cleanup 
	DeleteObject(hbitmap);
	DeleteDC(hdc);
}

int ezGfxMathRandom(int rangeMin, int rangeMax)
{
	return (rangeMin + rand() % (rangeMax - rangeMin + 1));
}

float ezGfxMathRandom(float rangeMin, float rangeMax)
{
	float randFloat = rand() / (float)RAND_MAX;
	return (rangeMin + randFloat * (rangeMax - rangeMin));
}

bool ezGfxMathRandomBool()
{
	return (rand() % 2 == 0);
}
