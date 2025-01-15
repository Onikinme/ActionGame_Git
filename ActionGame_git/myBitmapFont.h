#include <windows.h>
#include <d3d9.h>			// Direct3D9 (DirectX9 Graphics)�ɕK�v.
#include <d3dx9core.h>		// DirectX �X�v���C�g�ɕK�v.
#include <tchar.h>
#include <stdio.h>

// ������摜��\�����邽�߂̃N���X.
// �r�b�g�}�b�v�t�H���g�摜���ƂɃI�u�W�F�N�g�𐶐����邱��.
class MyBitmapFont
{
private:
	static ID3DXSprite* sm_pSpr;

	IDirect3DTexture9* m_pTex;

	// �O������̃R���X�g���N�^�̌Ăяo�����֎~���邽��private�̈�ɔz�u.
	MyBitmapFont() : m_pTex(NULL){}

public:
	// �I�u�W�F�N�g������static�����o����B�R���X�g���N�^�͌Ăяo���Ȃ�.
	static MyBitmapFont* LoadFont(IDirect3DDevice9* pDev, const TCHAR* pFont);
	virtual ~MyBitmapFont()
	{
		if (m_pTex) {
			m_pTex->Release();
		}
		if (sm_pSpr) {
			sm_pSpr->Release();
			sm_pSpr = NULL;
		}
	}
	int DrawBmpText(char* str, int x, int y, int stride);
	int DrawBmpText(char* str, int x, int y, int stride, DWORD color);
	int DrawBmpText(char* pStr, int x, int y, int stride, DWORD color, float ex);
	void SetSizeChangeText(float ex);
};
