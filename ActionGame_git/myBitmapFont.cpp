/*
	myBitmapFont.cpp
	画像文字関連
*/
#include "myBitmapFont.h"
#include "myTexture.h"

ID3DXSprite* MyBitmapFont::sm_pSpr = NULL;

// オブジェクトを生成し、フォントを読み込む.
MyBitmapFont* MyBitmapFont::LoadFont(IDirect3DDevice9* pDev, const TCHAR* pFname)
{
	MyBitmapFont* pFont = new MyBitmapFont;
	if (sm_pSpr == NULL)
	{
		D3DXCreateSprite(pDev, &(sm_pSpr));
	}

	// ↓の::は省略可能だが、グローバルな関数を呼び出していることを明確にしたい時に使うことがあります.
	::LoadTexture(pDev, pFname, &(pFont->m_pTex));
	return pFont;
}

int MyBitmapFont::DrawBmpText(char* pStr, int x, int y, int stride)
{
	return this->DrawBmpText(pStr, x, y, stride, 0xFF000000); // 黒で.
}

// 文字列を描画する。英文字しか表示できないので、UNICODE非対応.
// 位置(x, y)に、文字列strを表示する。文字間はstrideで指定.
// ビットマップは1文字あたり16x16ドットを仮定して.
// 文字コード表に合わせて絵が書かれている前提の処理です.
int MyBitmapFont::DrawBmpText(char* pStr, int x, int y, int stride, DWORD color)
{
	sm_pSpr->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXVECTOR3 cnt(0, 0, 0);
	for (unsigned int i = 0; pStr[i]; i++)
	{
		D3DXVECTOR3 pos((float)x, (float)y, 0);
		int code = (int)pStr[i];
		int u = (code % 16) * 16;
		int v = (code / 16) * 16;
		const int size = 16;

		// 表示範囲を設定する.
		// { left, top, right, bottom }
		const RECT rc = { u, v, u + size, v + size };
		sm_pSpr->Draw(m_pTex, &rc, &cnt, &pos, color);
		x += stride;
	}
	sm_pSpr->End();
	return x;
}

int MyBitmapFont::DrawBmpText(char* pStr, int x, int y, int stride, DWORD color, float ex)
{
	SetSizeChangeText(ex);
	sm_pSpr->Begin(D3DXSPRITE_ALPHABLEND);
	D3DXVECTOR3 cnt(0, 0, 0);
	for (unsigned int i = 0; pStr[i]; i++) {
		D3DXVECTOR3 pos(x, y, 0);
		int code = (int)pStr[i];
		int u = (code % 16) * 16;
		int v = (code / 16) * 16;
		const int size = 16;

		// 表示範囲を設定する.
		//{ left, top, right, bottom }
		const RECT rc = { u, v, u + size, v + size };
		sm_pSpr->Draw(m_pTex, &rc, &cnt, &pos, color);
		x += stride;
	}

	sm_pSpr->End();

	SetSizeChangeText(1.0);

	return x;
}

void MyBitmapFont::SetSizeChangeText(float ex)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixScaling(&mat, ex, ex, 1);
	sm_pSpr->SetTransform(&mat);
}
