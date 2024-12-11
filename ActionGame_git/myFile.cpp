/*
	myFile.cpp

	ファイル関連
*/
#include "stdafx.h"
#include "myFile.h"

////////////////////////// OpenFile //////////////////////////
// カレントと親ディレクトリからファイルを探す.
FILE* OpenFile(const TCHAR* pFname, LPCTSTR pMode)
{
	FILE* fp;
	int err = _tfopen_s(&fp, pFname, pMode);
	if (err == 0) { return fp; }// オープン成功.

	// 親からさがす.
	_tprintf(_T("%s: 親ディレクトリから探索...\n"), pFname);
	int flen = (int)_tcslen(pFname) + 4;
	TCHAR buff[MAX_PATH];
	_stprintf_s(buff, flen, _T("..\\%s"), pFname);
	err = _tfopen_s(&fp, buff, pMode);
	if (err == 0) {	return fp; }// オープン成功.

	return NULL;// オープン失敗.
}
