// STG_v1.cpp
// 自機の操作とショット.

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d9.h>	   // Direct3D9 (DirectX9 Graphics)に必要.
#include <d3dx9core.h> // DirectX スプライトに必要.
#include <tchar.h>
#include <locale.h>

#include "myTexture.h"
#include "myBitmapFont.h"
#include "myInput.h"

//#define FULLSCREEN

#define MEMORY_LEAK
#ifdef MEMORY_LEAK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// ウィンドウの大きさ設定.
#define WIDTH (1024) // 幅.
#define HEIGHT (768) // 高さ.

// ウィンドウタイトルバーに表示されるバージョン名.
#define WDATE(x, y) _T(x y)
static const TCHAR* version = WDATE(__DATE__, __TIME__);


/////////////////////////////////////////////////////////////
// ↓タイマークラス.

class CTimer
{
public:
	static CTimer s_Timer;

private:
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER Frequency;

	CTimer()
	{
		StartingTime.QuadPart = 0;
		Frequency.QuadPart = 0;
	}

public:
	~CTimer(){};

	void InitTimer();			// タイマー初期化.
	void StartTimer();			// タイマー開始.
	void GetPassTime(LONGLONG*, UINT*);	// 開始からの経過時間を得る.

	static CTimer* pGetTimer(){return &s_Timer; }
};

CTimer CTimer::s_Timer;

void CTimer::InitTimer()
{
	QueryPerformanceFrequency(&Frequency);
}

// タイマー開始.
void CTimer::StartTimer()
{
	QueryPerformanceCounter(&StartingTime);
}

// 開始からの経過時間を得る.
void CTimer::GetPassTime(LONGLONG* pMicroSeconds, UINT* pFps)
{
	LARGE_INTEGER EndingTime;
	LARGE_INTEGER ElapsedMicroseconds;

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	*pMicroSeconds = ElapsedMicroseconds.QuadPart;
	*pFps = (UINT)( 1000000 / ElapsedMicroseconds.QuadPart);
}

// ↑タイマークラス.
/////////////////////////////////////////////////////////////

// クラス前方宣言:実体の定義より先にクラス名だけを先に宣言.
// MyTexture *pTex; のようなポインタ宣言に対して有効になる.
class MyTexture;
class BulletOne;
class BulletBuffer;
struct DataSet;

// 関数宣言.
HRESULT InitDirect3D(DataSet* pData);
const TCHAR* D3DErrStr(HRESULT res);
void ReleaseDataSet(DataSet* pData);

#define RELEASE(__xx__)    \
	if (__xx__)            \
	{                      \
		__xx__->Release(); \
		__xx__ = 0;        \
	}

// プログラムに必要な変数を構造体として定義.
// （ゲームに必要なデータをメモリ上に、まとめて配置したいため）.
// ※後でシングルトンモデルのクラスに変更する予定.
struct DataSet
{
	HINSTANCE hInstance;		// インスタンスハンドル.
	HWND hWnd;					// 表示ウィンドウ.

	IDirect3D9* pD3D;			// Direct3Dインスタンスオブジェクト.
	D3DPRESENT_PARAMETERS d3dpp;// デバイス作成時のパラメータ.

	IDirect3DDevice9* pDevice;	// Direct3Dデバイスオブジェクト.
	ID3DXSprite* pSprite;		// スプライト.
	MyBitmapFont* pFont;		// ビットマップフォント.

	MyTexture* pPlayerTex;		// 自機.
	MyTexture* pBulletTex;		// 自機弾丸.
								// 注意事項.
								// MyTextureの実体がまだ定義されていないため.
								// MyTexture plyaer_tex; とは書くことが出来ない.
								// ポインタ型であれば利用することができる.
								// MyTexture型の大きさはこの時点ではまだ不明だが.
								// ポインタは4バイトなので大きさが確定している.

	float player_x, player_y;	// 自機の現在位置.
	bool m_isGround; // 地面に着いているか？
	float m_speed_x, m_speed_y; // 初速度
	float m_gravity; // 重力
	float jumpbutton; // ジャンプボタン押下フラグ
	float shortjump; // ジャンプ上昇中にジャンプボタンを離したフラグ
	int player_nextfire;		// 自機が次に弾を撃てるまでの時間.
	BulletBuffer* pPlayerBullet;// 自機の弾丸.

	UINT joyID;					// ゲームコントローラのID.
	UINT buttons;				// ボタンの情報.

	int game_timer;				// 全体のタイマー.
} mydata;


// BulletOne //////////////////
// 弾丸クラス:一発一発の弾丸を初期化や、移動など.
class BulletOne
{
	// 練習２.
	// TODO 必要なメンバ変数を追加してください.
private:
	float m_x, m_y;
	float m_vx, m_vy;
	float m_timer;
public:
	// 練習２.
	// TODO メンバ変数を追加したらコンストラクタで初期化してください.
	BulletOne()
		: m_x(0.0f)
		, m_y(0.0f)
		, m_vx(0.0f)
		, m_vy(0.0f)
		, m_timer(-1.0f)
	{}

	// デストラクタ.
	~BulletOne(){}

	// 射出初期化.
	void Init(float x, float y, float vx, float vy);

	// 弾丸を進める.g
	void Update(float fTime);

	// 現在のXY座標値を得る.
	void GetXY(float& x, float& y);

	// タイマーが0以上なら有効（インライン関数）.
	bool IsActive();
};

// 射出初期化.
void BulletOne::Init(float x, float y, float vx, float vy)
{
	// x (期位置X), y (初期位置Y), vx (初速度X), vy (初速度Y).

	// 練習２.
	// TODO 弾丸が発射されたときの初期情報を設定する処理を記述してください.
	m_x = x;
	m_y = y;
	m_vx = vx;
	m_vy = vy;
	m_timer = 0.0f;
}

// 弾丸を進める.
void BulletOne::Update(float fTime)
{
	// 練習２.
	// TODO 弾丸が画面上方に移動する処理を記述してください.
	if (m_timer >= 0.0f) {
		m_x += (m_vx * fTime);
		m_y += (m_vy * fTime);
		m_timer += fTime;
	}
	if (m_y < 0.0f) {
		m_timer = -1.0f;
	}
}

// 現在のXY座標値を得る.
void BulletOne::GetXY(float& x, float& y)
{
	// 練習２
	// TODO メンバ変数の値を取得する処理を記述してください.
	x = m_x;
	y = m_y;
}

// タイマーが0以上なら有効（インライン関数）.
bool BulletOne::IsActive()
{
	// 練習２
	// TODO
	return (m_timer >= 0.0f);
} 


// BulletBuffer //////////////////
// 弾丸バッファを管理するクラス。BulletOneクラスは単一の弾丸を管理している.
// 複数の弾丸の管理はこちらのクラスで行う。クラスを二つに分けることで.
// お互いの担当範囲（責任範囲）を分けることができ、プログラムの見通しを良くする.
class BulletBuffer
{
private:
	BulletOne* m_pBuffer;	// 弾丸の配列.
	int m_index;			// 配列上の現在位置を示す.
	int m_bulletMax;		// 配列の大きさ.
	char* m_pTextBuffer;	// デバッグ用のテキストバッファ.

public:
	BulletBuffer(int max);
	virtual ~BulletBuffer();
	BulletOne* pNext(); // 次の空き位置を返す.
	BulletOne* pGet(int ix)
	{
		return m_pBuffer + ix; // &(m_pBuffer[ix]) と同じ意味.
	}
	void Update(float fTime); // 配列の内容である弾丸全体を更新する.
	void Draw(DataSet* pData);
	char* pGetText();
};

// コンストラクタ:引数から配列の最大値を得て、配列を作成している.
BulletBuffer::BulletBuffer(int max)
	: m_index(0)
	, m_pBuffer(NULL)
	, m_bulletMax(max)
	, m_pTextBuffer(NULL)
{
	m_pTextBuffer = new char[max + 1];
	m_pBuffer = new BulletOne[max];
}

// デストラクタ:配列を解放している.
BulletBuffer::~BulletBuffer()
{
	delete[] m_pBuffer;		// delete m_pBufferでないことに注意.
	delete[] m_pTextBuffer;	// 配列で確保したメモリ領域は[]を付けてdeleteすること.
}

// リングバッファを探索して、空きを見つける.
// 戻り値 : NULL　　　⇒　空きなし.
//			NULL以外　⇒　空き位置.
BulletOne* BulletBuffer::pNext()
{
	// 練習３.
	// TODO
	for (int i = 0; i < m_bulletMax; i++) {
		int ix = (m_index + i) % m_bulletMax;
		if ( !m_pBuffer[ix].IsActive() ) {

			m_index = (ix + 1) % m_bulletMax;
			return &m_pBuffer[ix];
		}
	}

	return NULL;// 空きなし.
}

// リングバッファ内のデータを順次更新する.
void BulletBuffer::Update(float fTime)
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		m_pBuffer[i].Update(fTime);
	}
}

// 弾丸を表示.
void BulletBuffer::Draw(DataSet* pData)
{
	D3DXVECTOR3 cnt(8, 8, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	for (int i = 0; i < m_bulletMax; i++)
	{
		BulletOne& blt = m_pBuffer[i];
		// 取得した弾丸が有効なら表示.
		if (blt.IsActive())
		{
			blt.GetXY(pos.x, pos.y); // 現在位置を取得.
			if (pData->pBulletTex != NULL) {
				pData->pSprite->Draw(pData->pBulletTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			}
		}
	}
}

// デバッグ用にpBuffer配列の中身をテキスト化する.
// 弾丸が有効なら*、無効なら-と一文字にする.
char* BulletBuffer::pGetText()
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		if (m_pBuffer[i].IsActive())
		{
			m_pTextBuffer[i] = '*';
		}
		else
		{
			m_pTextBuffer[i] = '-';
		}
	}
	// 文字列の最後は必ずNULLでターミネート.
	m_pTextBuffer[m_bulletMax] = NULL;
	return m_pTextBuffer;
}

// データをアップデートする.
void UpdateData(DataSet* pData)
{
	MyInput* pInput = GetInputInst();

	pData->game_timer++;
	pData->buttons = 0;

	// 入力情報を更新.
	pInput->UpdateInput(pData->hWnd);

	// ジョイスティックでの操作.
	if (pInput->GetJoyCnt() > 0) {
		// 左ステックで移動.
		int jx = pInput->GetStickLX(JOY_CON_0);
		int jy = pInput->GetStickLY(JOY_CON_0);

		// あそびを吸収するため、一定値より大きいか(小さいか)を判定する.
		if (abs(jx) > 10) {
			pData->player_x += jx / 50;
		}
		if (abs(jy) > 10) {
			pData->player_y += jy / 50;
		}

		if (pInput->IsPushBtn(JOY_CON_0, JOY_BTN_BIT_A)) {
			pData->buttons |= JOY_BTN_BIT_A;// Ａボタンで弾丸発射.
		}
	}
	// キーボードでの操作.
	{
		// 左に動く
		if (pInput->IsPushKey(DIK_A)) {
			// 空中にいて、かつ右方向への慣性が働いているとき、
			// 左方向に動きやすくなる
			if (!pData->m_isGround && 0 < pData->m_speed_x) {
				pData->m_speed_x -= 0.3f;
			}
			else pData->m_speed_x -= 0.1f;
			if (pData->m_speed_x < -5.0f)pData->m_speed_x = -5.0f;
		}
		// 右に動く
		if (pInput->IsPushKey(DIK_D)) {
			// 空中にいて、かつ左方向への慣性が働いているとき、
			// 右方向に動きやすくなる
			if (!pData->m_isGround && 0 > pData->m_speed_x) {
				pData->m_speed_x += 0.3f;
			}
			else pData->m_speed_x += 0.1f;
			if (pData->m_speed_x > 5.0f)pData->m_speed_x = 5.0f;
		}

		// キーを離した時の減速調整
		if (!pInput->IsPushKey(DIK_A) && 0 > pData->m_speed_x) {
			if (pData->m_isGround) { // 地上ではブレーキが良く効く
				pData->m_speed_x += 0.1f;
				pData->m_speed_x *= 0.93f;
			}
			else { // 空中では横方向に慣性が働くため、ブレーキが効きづらい
				pData->m_speed_x += 0.01f;
				pData->m_speed_x *= 0.99f;
			}
			if (pData->m_speed_x > 0) pData->m_speed_x = 0;
		}
		if (!pInput->IsPushKey(DIK_D) && 0 < pData->m_speed_x) {
			if (pData->m_isGround) { // 地上ではブレーキが良く効く
				pData->m_speed_x -= 0.1f;
				pData->m_speed_x *= 0.93f;
			}
			else { // 空中では横方向に慣性が働くため、ブレーキが効きづらい
				pData->m_speed_x -= 0.01f;
				pData->m_speed_x *= 0.99f;
			}
			if (pData->m_speed_x < 0) pData->m_speed_x = 0;
		}
		pData->player_x += pData->m_speed_x;

		if (pData->player_x < 0)pData->player_x = 0;
		if(pData->player_x > 1024)pData->player_x = 1024;

		// プレイヤーが地面に着いていたらジャンプ可能
		if (pData->m_isGround) {
			if (pInput->IsPushKey(DIK_SPACE) && pData->player_y == 668 && !pData->jumpbutton) {
				pData->m_speed_y = -10.0f;  // 初速度の設定
				//Y座標の更新
				pData->player_y += pData->m_speed_y;
				pData->jumpbutton = true;
				pData->m_isGround = false;
			}
		}
		// ジャンプ中
		else {
			//Y方向の速度に加速度を加える
			pData->m_speed_y += pData->m_gravity;
			//Y座標の更新
			pData->player_y += pData->m_speed_y;

			//上昇中にボタンを離したときの処理
			if (!pInput->IsPushKey(DIK_SPACE))pData->shortjump = true;
			if(pData->m_speed_y < 0 && pData->shortjump)pData->m_speed_y *= 0.5f;
		}

		// ジャンプボタン長押しでジャンプし続けてしまわないようにチェック
		if (!pInput->IsPushKey(DIK_SPACE) && pData->jumpbutton && pData->player_y == 668) {
			pData->jumpbutton = false;
		}

		// 地面に着いたかチェック
		if (pData->player_y >= 668) {
			pData->player_y = 668;
			pData->m_isGround = true;
			pData->shortjump = false;
		}

		/*
		if (pInput->IsPushKey(DIK_UP)) {
			pData->player_y -= 5;
		}
		if (pInput->IsPushKey(DIK_DOWN)) {
			pData->player_y += 5;
		}
		if (pInput->IsPushKey(DIK_SPACE)) {
			pData->buttons |= JOY_BTN_BIT_A;// スペースキーで弾丸発射.
		}
		*/
	}

	// ボタンが押されていれば、弾丸発射.
	if ((pData->buttons & JOY_BTN_BIT_A) && pData->player_nextfire <= 0)
	{
		BulletOne* pBulletOne = pData->pPlayerBullet->pNext();
		if (pBulletOne != NULL)
		{
			float fXSpeed = 0.0f;
			float fYSpeed = 20.0f;
			pBulletOne->Init(pData->player_x, pData->player_y, fXSpeed, -fYSpeed);

			_tprintf(_T("発射 pBulletOne=%p\n"), pBulletOne);
			// 射出後、Nカウント(1カウント＝1/60sec)は発射不可に.
			pData->player_nextfire = 5;
		}
	}
	// 発射可能になるまでのタイムラグが残っているかどうか.
	if (pData->player_nextfire > 0)
	{
		pData->player_nextfire--;
	}

	// 弾丸と敵を更新、ただしすり抜け対策のため時分割する.
	for (int i = 0; i < 10; i++)
	{
		pData->pPlayerBullet->Update(1.0f / 10);
	}
}


// DataSetに基づいて、スプライトを描画.
// param pData データセット.
// return 発生したエラー.
HRESULT DrawData(DataSet* pData)
{
	D3DCOLOR rgb = D3DCOLOR_XRGB(10, 10, 80);
	// 画面全体を消去.
	pData->pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0);

	// 描画を開始（シーン描画の開始）.
	pData->pDevice->BeginScene();

	// スプライトの描画を開始.
	pData->pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// 弾丸の表示。配列を順番に探査して表示する.
	pData->pPlayerBullet->Draw(pData);

	// プレイヤー機（自機）の描画.
	D3DXVECTOR3 cnt(16, 16, 0);
	D3DXVECTOR3 pos(pData->player_x, pData->player_y - 100, 0);
	RECT rc = { 0, 0, 32, 32 };
	if (pData->m_isGround) {
		if (pData->m_speed_x != 0) rc = { 0, 32, 32, 64 };
		else rc = { 0, 0, 32, 32 };
	}
	else rc = { 0, 192, 32, 224 };
	if (pData->pPlayerTex != NULL) {
		pData->pSprite->Draw(pData->pPlayerTex->GetTexture(), &rc, &cnt, &pos, 0xFFFFFFFF);
	}

	// スプライトの描画を終了.
	pData->pSprite->End();

	// デバッグ情報の表示.
#if defined(RELEASE)
	LONGLONG us; UINT fps;
	CTimer::pGetTimer()->GetPassTime(&us, &fps);
	CTimer::pGetTimer()->StartTimer();

	char txt[256] = { 0 };
	sprintf_s(txt, "microSeconds: %llu, FPS:%d\n", us, fps);
	pData->pFont->DrawBmpText(txt, 20, 640, 8, 0xFFFFFFFF);

	sprintf_s(txt, "game: %04d pad %d:(%04.1f, %03.1f) btn:%04X",
		pData->game_timer, pData->joyID, pData->player_x, pData->player_y, pData->buttons);
	pData->pFont->DrawBmpText(txt, 20, 680, 8, 0xFFFFFFFF);

	sprintf_s(txt, "bullet:%s", pData->pPlayerBullet->pGetText());
	pData->pFont->DrawBmpText(txt, 20, 720, 8, 0xFFFFFFFF);
#endif

	// 描画を終了（シーン描画の終了）.
	pData->pDevice->EndScene();

	// 実際に画面に表示。バックバッファからフロントバッファへの転送.
	// デバイス生成時のフラグ指定により、ここでVSYNCを待つ.
	pData->pDevice->Present(NULL, NULL, NULL, NULL);


	return D3D_OK;
}


// イベント処理コールバック（ウィンドウプロシージャ）.
LRESULT CALLBACK
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// イベントの種類に応じて、switch文にて処理を切り分ける.
	switch (uMsg)
	{
	case WM_KEYDOWN:
		// ESCキーが押下されたら終了.
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;

	case WM_CLOSE: // 終了通知(CLOSEボタンが押された場合など)が届いた場合.
				   // プログラムを終了させるため、イベントループに0を通知する.
				   // この結果、GetMessageの戻り値は0になる.
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	// デフォルトのウィンドウイベント処理.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Windowを作成する.
// return ウィンドウハンドル.
HWND InitWindow(DataSet* pData)
{
	// ウィンドウクラス登録.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));					 // 変数wcをゼロクリアする.
	wc.cbSize = sizeof(WNDCLASSEX);							 // 構造体の大きさを与える.
	wc.lpfnWndProc = (WNDPROC)WindowProc;					 // ウィンドウプロシージャ登録.
	wc.hInstance = pData->hInstance;						 // インスタンスハンドル設定.
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);				 // マウスカーソル登録.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH); // 背景をGRAYに.
	wc.lpszClassName = _T("Window01");						 // クラス名、CreateWindowと一致させる.
	RegisterClassEx(&wc);									 // 登録.

	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_tprintf(_T("rc: %d, %d -- %d, %d\n"), rc.left, rc.top, rc.right, rc.bottom);

	// ウィンドウを作成する。クラス名は"Window01"
	pData->hWnd = CreateWindow(_T("Window01"), version, WS_OVERLAPPEDWINDOW,
		0, 0, WIDTH, HEIGHT, NULL, NULL, pData->hInstance, NULL);

	return pData->hWnd;
}

// メインループ.
void MainLoop(DataSet* pData)
{
	HRESULT hr = E_FAIL;

	// ウィンドウを作成する.
	pData->hWnd = InitWindow(pData);

	// Direct3Dを初期化する.
	hr = InitDirect3D(pData);
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("Direct3D初期化失敗"), MB_OK);
		return;
	}

	// スプライトの作成
	hr = D3DXCreateSprite(pData->pDevice, &(pData->pSprite));
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("スプライト作成失敗"), MB_OK);
		return;
	}

	// 入力デバイスの初期化.
	GetInputInst()->InitInput(pData->hInstance, pData->hWnd);
	if (GetInputInst()->GetJoyCnt() == 0) {
		MessageBox(NULL, _T("ジョイスティックが繋がっていません\r\nキーボードで操作します"), _T("JOYSTICK"), MB_OK);
	}

	// とりあえず初期値を入れておく.
	pData->player_x = WIDTH / 2;
	pData->player_y = HEIGHT - 100;
	pData->m_isGround = true; // プレイヤーが地面についているか？
	pData->m_speed_x = 0.0f; // 横方向へのスピード
	pData->m_speed_y = -12.0f; // ジャンプ力
	pData->m_gravity = 0.3f; // 重力
	pData->pPlayerBullet = new BulletBuffer(3);// 画面上に３発まで打てる.

	// 例外処理.
	// テクスチャを読み込み、エラー時にはLoadTextureは例外を発生させる.
	// LoadTextureごとにif判定をするのは面倒なので、例外処理を使用.
	try
	{
		pData->pFont = MyBitmapFont::LoadFont(pData->pDevice, _T("image/fontBitmap.png"));
		// 練習１.
		// TODO
		pData->pPlayerTex = MyTexture::LoadTexture(pData->pDevice, _T("image/playerkari.png"));
		pData->pBulletTex = MyTexture::LoadTexture(pData->pDevice, _T("image/bullet.png"));
	}
	// catch文はtryの直後に記述する.
	catch (HRESULT /*hr*/)//ここでは変数 hr は使われないため、warning対策のためコメントアウトしておく.
	{
		return;
	}

	ShowWindow(pData->hWnd, SW_SHOWNORMAL);// 作成したウィンドウを表示する.

	CTimer::pGetTimer()->InitTimer();
	CTimer::pGetTimer()->StartTimer();

	// イベントループ.
	// ブロック型関数GetMessageではなくノンブロック型関数のPeekMessageを使う.
	MSG msg;
	bool flag = true;
	while (flag)
	{
		// メッセージがあるかどうか確認する.
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// メッセージがあるので処理する.
			if (GetMessage(&msg, NULL, 0, 0) == 1)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				flag = false;
			}
		}
		else
		{
			UpdateData(pData); // 位置の再計算.
			DrawData(pData);	  // 描画.
		}
		// Sleepなど行っていないが、DrawData関数内のpData->pDevice->Presentが.
		// 画面更新待ちを行うので、結果的に1/60ごとにイベントループが回る.
	}
}

// DataSetを解放する.
void ReleaseDataSet(DataSet* pData)
{
	// テクスチャの開放.
	MyTexture::ReleaseAll();

	// 動的オブジェクトの開放.
	if (pData->pFont)			{ delete pData->pFont; }
	if (pData->pPlayerBullet)	{ delete pData->pPlayerBullet; }

	// 入力デバイスの解放.
	GetInputInst()->Release();

	// DirectXデバイスの開放.
	RELEASE(pData->pSprite);
	RELEASE(pData->pDevice);
	RELEASE(pData->pD3D);
}


// param argc コマンドラインから渡された引数の数.
// param argv 引数の実体へのポインタ配列.
// return 0 正常終了.
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Japanese"); // 日本語にロケールを設定.

#ifdef MEMORY_LEAK
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	// このプログラムが実行されるときのインスタンスハンドルを取得.
	mydata.hInstance = GetModuleHandle(NULL);
	MainLoop(&mydata);

	ReleaseDataSet(&mydata);

#ifdef MEMORY_LEAK
	_CrtDumpMemoryLeaks(); // メモリリークを検出する.
#endif

	return 0;
}

// Direct3Dを初期化する.
// param pData データセット.
// return 発生したエラーまたはD3D_OK.
//
HRESULT InitDirect3D(DataSet* pData)
{
	HRESULT hr;

	// Direct3Dインスタンスオブジェクトを生成する.
	// D3D_SDK_VERSIONと、ランタイムバージョン番号が適切でないと、NULLが返る.
	pData->pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	// NULLならランタイムが不適切.
	if (pData->pD3D == NULL)
	{
		return E_FAIL;
	}

	// ディスプレイモードの取得.
	D3DDISPLAYMODE d3dd;
	hr = pData->pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dd);
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("ディスプレイモード取得失敗"), MB_OK);
		return hr;
	}

	// PRESENTパラメータをゼロクリアし、適切に初期化.
	ZeroMemory(&(pData->d3dpp), sizeof(pData->d3dpp));
	// ウィンドウモードに.
#ifdef FULLSCREEN
	pData->d3dpp.Windowed = FALSE;
	// int fx = GetSystemMetrics(SM_CXSCREEN);
	// int fy = GetSystemMetrics(SM_CYSCREEN);
	int fx = WIDTH;
	int fy = HEIGHT;
	_tprintf(_T("フルスクリーン %d x %d\n"), fx, fy);
	pData->d3dpp.BackBufferHeight = fy;
	pData->d3dpp.BackBufferWidth = fx;
#else
	pData->d3dpp.Windowed = TRUE;
#endif
	pData->d3dpp.BackBufferHeight = HEIGHT;
	pData->d3dpp.BackBufferWidth = WIDTH;
	pData->d3dpp.BackBufferCount = 1;
	pData->d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// バックバッファはRGBそれぞれ８ビットで.
	pData->d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	// Present時に垂直同期に合わせる.
	pData->d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// D3Dデバイスオブジェクトの作成。HAL&HARD.
	hr = pData->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pData->hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &(pData->d3dpp), &(pData->pDevice));
	if (hr == D3D_OK)
	{
		// 頂点計算とラスタライズ（ディスプレイへの投影計算）を両方ともGPUでできれば最速.
		_tprintf(_T("HAL & HARD\n"));
		return hr;
	}

	// D3Dデバイスオブジェクトの作成。HAL&SOFT.
	hr = pData->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pData->hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(pData->d3dpp), &(pData->pDevice));
	if (hr == D3D_OK)
	{
		// 頂点計算をCPU、ラスタライズをGPUで出来れば次点で採用.
		_tprintf(_T("HAL & SOFT\n"));
		return hr;
	}

	// D3Dデバイスオブジェクトの作成。REF&HARD.
	hr = pData->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, pData->hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &(pData->d3dpp), &(pData->pDevice));
	if (hr == D3D_OK)
	{
		// 頂点計算をGPU、ラスタライズをCPUで出来れば採用.
		_tprintf(_T("REF & HARD\n"));
		return hr;
	}

	// D3Dデバイスオブジェクトの作成。REF&SOFT.
	hr = pData->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, pData->hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(pData->d3dpp), &(pData->pDevice));
	if (hr == D3D_OK)
	{
		// 頂点計算もラスタライズもGPUで出来ないと最も遅い.
		_tprintf(_T("REF & SOFT\n"));
		return hr;
	}

	// D3Dデバイスが作成できなかった.
	return hr;
}

// エラー発生時のHRESULTを文字列に変換するための補助関数.
const TCHAR* D3DErrStr(HRESULT res)
{
	switch (res)
	{
	case D3D_OK:
		return _T("D3D_OK");
	case D3DERR_DEVICELOST:
		return _T("D3DERR_DEVICELOST");
	case D3DERR_DRIVERINTERNALERROR:
		return _T("D3DERR_DRIVERINTERNALERROR");
	case D3DERR_INVALIDCALL:
		return _T("D3DERR_INVALIDCALL");
	case D3DERR_OUTOFVIDEOMEMORY:
		return _T("D3DERR_OUTOFVIDEOMEMORY");
	case D3DERR_DEVICENOTRESET:
		return _T("D3DERR_DEVICENOTRESET");
	case D3DERR_NOTAVAILABLE:
		return _T("D3DERR_NOTAVAILABLE");
	case D3DXERR_INVALIDDATA:
		return _T("D3DXERR_INVALIDDATA");
	case MMSYSERR_NODRIVER:
		return _T("MMSYSERR_NODRIVER");
	case MMSYSERR_INVALPARAM:
		return _T("MMSYSERR_INVALPARAM");
	case MMSYSERR_BADDEVICEID:
		return _T("MMSYSERR_BADDEVICEID");
	case JOYERR_UNPLUGGED:
		return _T("JOYERR_UNPLUGGED");
	case JOYERR_PARMS:
		return _T("JOYERR_PARMS");
	case E_OUTOFMEMORY:
		return _T("E_OUTOFMEMORY");
	}
	return _T("unknown error");
}
