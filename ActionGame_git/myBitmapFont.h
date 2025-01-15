#include <windows.h>
#include <d3d9.h>			// Direct3D9 (DirectX9 Graphics)に必要.
#include <d3dx9core.h>		// DirectX スプライトに必要.
#include <tchar.h>
#include <stdio.h>

// 文字列画像を表示するためのクラス.
// ビットマップフォント画像ごとにオブジェクトを生成すること.
class MyBitmapFont
{
private:
	static ID3DXSprite* sm_pSpr;

	IDirect3DTexture9* m_pTex;

	// 外部からのコンストラクタの呼び出しを禁止するためprivate領域に配置.
	MyBitmapFont() : m_pTex(NULL){}

public:
	// オブジェクト生成はstaticメンバから。コンストラクタは呼び出せない.
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
