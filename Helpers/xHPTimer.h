
// xHPTimer - Timing related staff
// Author: Zviadi Varshanidze (Based on Evan Pipho's CTimer class)
// Date: 20.07.2008

#ifndef XHPTIMER_H
#define XHPTIMER_H

#include <windows.h>

class xHPTimer
{
public:

	// Initialization
	void Init();

	// Get the FPS based on elapsed frames
	float GetFPS(unsigned int uiFrames = 1);

	// Get the number of ms since last call
	unsigned int GetMS();

	// Get the number of seconds since the last call
	float GetSeconds();

private:
	float m_fInvTicksPerSec;
	float m_fInvTicksPerMs;
	__int64 m_i64StartTime;
	__int64 m_i64LastTime;
};

inline void xHPTimer::Init()
{
	__int64 i64Freq = 0;
	QueryPerformanceFrequency((LARGE_INTEGER *)&i64Freq);
	QueryPerformanceCounter((LARGE_INTEGER *)&m_i64StartTime);
	m_fInvTicksPerSec = 1.f / i64Freq;
	m_fInvTicksPerMs = 1000.f / i64Freq;
	m_i64LastTime = 0;
	GetMS();
}

inline float xHPTimer::GetFPS(unsigned int uiFrames)
{
	static __int64 sLastTime = m_i64StartTime;
	__int64 i64NewTime = 0;
	float fElapsedSecs = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&i64NewTime);
	fElapsedSecs = (i64NewTime - sLastTime) * m_fInvTicksPerSec;
	sLastTime = i64NewTime;

	return (uiFrames / fElapsedSecs); 
}

inline unsigned int xHPTimer::GetMS()
{
	__int64 i64NewTime = 0;
	unsigned int uiMs;

	QueryPerformanceCounter((LARGE_INTEGER *)&i64NewTime);
	uiMs = (unsigned int)((i64NewTime - m_i64LastTime) * m_fInvTicksPerMs);
	m_i64LastTime = i64NewTime;
	return uiMs;
}

inline float xHPTimer::GetSeconds()
{
	__int64 i64NewTime = 0;
	float fElapsedSecs;

	QueryPerformanceCounter((LARGE_INTEGER *)&i64NewTime);
	fElapsedSecs = (i64NewTime - m_i64LastTime) * m_fInvTicksPerSec;
	m_i64LastTime = i64NewTime;
	return fElapsedSecs;
}

#endif // XHPTIMER_H
