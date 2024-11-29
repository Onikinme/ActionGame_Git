#include "myTimer.h"

// �J�n����̌o�ߎ��Ԃ𓾂�.
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
