#pragma once

#include <windows.h>

/////////////////////////////////////////////////////////////
// タイマー管理クラス.

class CTimer
{
private:
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER Frequency;

public:
	CTimer()
	{
		StartingTime.QuadPart = 0;
		InitTimer();
	}
	~CTimer(){};

	// タイマー初期化.
	void InitTimer() {
		QueryPerformanceFrequency(&Frequency);
	}
	// タイマー開始.
	void StartTimer() {
		QueryPerformanceCounter(&StartingTime);
	}
	// 開始からの経過時間を得る.
	void GetPassTime(LONGLONG*, UINT*);
};
