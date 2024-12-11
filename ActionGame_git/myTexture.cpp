/*
	myTexture.cpp
	画像読込
*/
#include "myTexture.h"

MyTexture* MyTexture::sm_pStack[MAX_TEXTURE];
int MyTexture::sm_maxTexture = 0;

////////////////////////////////////////////////////////////////////////////////////////
// 画像読込　カレントで失敗したら１つ上のフォルダから読み込む.
// pFname ファイル名。const修飾子をつけて関数内で値を変更しないことを宣言する.
// ppTex 作成するテクスチャへのポインタを入れるためのポインタ.
HRESULT LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname, IDirect3DTexture9** ppTex)
{
	HRESULT hr = D3DXCreateTextureFromFile(pDev, pFname, ppTex);
	if (FAILED(hr))
	{
		// 失敗したので一つ上のフォルダからテクスチャを探してみる.
		int flen = (int)_tcslen(pFname) + 4;
		_tprintf(_T("%s: 親ディレクトリから探索...\n"), pFname);
		TCHAR* fn = new TCHAR[flen];
		_stprintf_s(fn, flen, _T("..\\%s"), pFname);
		hr = D3DXCreateTextureFromFile(pDev, fn, ppTex);
		delete[] fn;
		if (FAILED(hr))
		{
			// 失敗したので例外を発生させて終わる.
			MessageBox(NULL, _T("テクスチャ読み込み失敗"), pFname, MB_OK);
			throw hr;		// エラーが発生したので例外を送る.
		}
	}
	return hr;
}

// 画像を読み込むための関数.
// pDev D3Dデバイス.
// pFname テクスチャファイル名.
MyTexture* MyTexture::LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname)
{
	if (sm_maxTexture >= MAX_TEXTURE)
	{
		return NULL;
	}

	// オブジェクトを生成.
	MyTexture* pMyTex = new MyTexture;
	// ::だけが付いた関数はグローバル関数を呼び出すことを意味する.
	::LoadTexture(pDev, pFname, &(pMyTex->m_pTex));
	sm_pStack[sm_maxTexture++] = pMyTex;// スタックに積む.
	return pMyTex;
}

void MyTexture::ReleaseAll()
{
	for (int i = sm_maxTexture - 1; i >= 0; i--)
	{
		if (sm_pStack[i])
		{
			delete sm_pStack[i];
		}
		sm_pStack[i] = NULL;
	}
	sm_maxTexture = 0;
}
