#include <windows.h>
#include <d3d9.h>			// Direct3D9 (DirectX9 Graphics)に必要.
#include <d3dx9core.h>		// DirectX スプライトに必要.
#include <tchar.h>
#include <stdio.h>

const int MAX_TEXTURE = 256;

////////////////////////// MyTexture //////////////////////////
// テクスチャを管理するためのクラスを作成する.
// テクスチャが設定されていれば、デストラクタにて解放する.
// この結果、解放忘れを防ぐことができる.
// ただし、オブジェクト自体をdeleteしなければならないので.
// 相変わらず解放忘れには注意しなければならない.
class MyTexture
{
private:
	static MyTexture* sm_pStack[MAX_TEXTURE];
	static int sm_maxTexture;

protected:
	IDirect3DTexture9* m_pTex;			// 管理するテクスチャ.

private:
	// コンストラクタをクラス定義内で記述。メンバ変数pTexを NULL で初期化する.
	MyTexture() : m_pTex(NULL)
	{
	}

	// デストラクタでは、pTexを解放する。自動的に行われるため、解放忘れを防げる.
	// デストラクタもprivateエリアに配置し外部からの開放を禁止.
	virtual ~MyTexture()
	{
		if (m_pTex)
		{
			m_pTex->Release();
		}
	}

public:
	// アクセサ。pTexを取得するための関数.
	IDirect3DTexture9* GetTexture()
	{
		return m_pTex;
	}

	// MyTextureオブジェクトを作成しテクスチャを読み込む.
	static MyTexture* LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname);

	// 読み込んだテクスチャをすべて破棄する.
	static void ReleaseAll();
};

HRESULT LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname, IDirect3DTexture9** ppTex);
