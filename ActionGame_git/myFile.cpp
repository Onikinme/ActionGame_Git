/*
	myFile.cpp

	�t�@�C���֘A
*/
#include "stdafx.h"
#include "myFile.h"

////////////////////////// OpenFile //////////////////////////
// �J�����g�Ɛe�f�B���N�g������t�@�C����T��.
FILE* OpenFile(const TCHAR* pFname, LPCTSTR pMode)
{
	FILE* fp;
	int err = _tfopen_s(&fp, pFname, pMode);
	if (err == 0) { return fp; }// �I�[�v������.

	// �e���炳����.
	_tprintf(_T("%s: �e�f�B���N�g������T��...\n"), pFname);
	int flen = (int)_tcslen(pFname) + 4;
	TCHAR buff[MAX_PATH];
	_stprintf_s(buff, flen, _T("..\\%s"), pFname);
	err = _tfopen_s(&fp, buff, pMode);
	if (err == 0) {	return fp; }// �I�[�v������.

	return NULL;// �I�[�v�����s.
}
