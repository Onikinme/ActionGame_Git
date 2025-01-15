/*
	myBitmapFont.cpp
	�摜�����֘A
*/
#include "myBitmapFont.h"
#include "myTexture.h"

ID3DXSprite* MyBitmapFont::sm_pSpr = NULL;

// �I�u�W�F�N�g�𐶐����A�t�H���g��ǂݍ���.
MyBitmapFont* MyBitmapFont::LoadFont(IDirect3DDevice9* pDev, const TCHAR* pFname)
{
	MyBitmapFont* pFont = new MyBitmapFont;
	if (sm_pSpr == NULL)
	{
		D3DXCreateSprite(pDev, &(sm_pSpr));
	}

	// ����::�͏ȗ��\�����A�O���[�o���Ȋ֐����Ăяo���Ă��邱�Ƃ𖾊m�ɂ��������Ɏg�����Ƃ�����܂�.
	::LoadTexture(pDev, pFname, &(pFont->m_pTex));
	return pFont;
}

int MyBitmapFont::DrawBmpText(char* pStr, int x, int y, int stride)
{
	return this->DrawBmpText(pStr, x, y, stride, 0xFF000000); // ����.
}

// �������`�悷��B�p���������\���ł��Ȃ��̂ŁAUNICODE��Ή�.
// �ʒu(x, y)�ɁA������str��\������B�����Ԃ�stride�Ŏw��.
// �r�b�g�}�b�v��1����������16x16�h�b�g�����肵��.
// �����R�[�h�\�ɍ��킹�ĊG��������Ă���O��̏����ł�.
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

		// �\���͈͂�ݒ肷��.
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

		// �\���͈͂�ݒ肷��.
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
