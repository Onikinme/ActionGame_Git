#include "myTimer.h"

// ŠJŽn‚©‚ç‚ÌŒo‰ßŽžŠÔ‚ð“¾‚é.
void CTimer::GetPassTime(LONGLONG* pMicroSec, UINT* pFps)
{
	LARGE_INTEGER EndingTime;
	LARGE_INTEGER ElapsedMS;

	QueryPerformanceCounter(&EndingTime);
	ElapsedMS.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedMS.QuadPart *= 1000000;
	ElapsedMS.QuadPart /= Frequency.QuadPart;

	*pMicroSec = ElapsedMS.QuadPart;
	*pFps = (UINT)(1000000 / ElapsedMS.QuadPart);
}
