/*
	myInput.h

	入力処理管理用
*/
#pragma once

// dinput.hのインクルードの前にどのバージョンを使うかを定義する必要がある.
#define DIRECTINPUT_VERSION 0x0800// これがないとビルド時に警告がでる.
#include <dinput.h>

constexpr int KEYBUF_SIZE = 256;// キーボード入力情報を得るためのバッファサイズ.

// IDの順番はDXライブラリのビット配列を参考にしている.
enum E_MOUSE_BTN_ID
{
	MOUSE_BTN_L,
	MOUSE_BTN_R,
	MOUSE_BTN_M,

	MOUSE_BTN_COUNT,
};
// IsPushMouse関数に使うbtnの値をビット演算で指定.
#define MOUSE_BTN_BIT_L	(1 << MOUSE_BTN_L)
#define MOUSE_BTN_BIT_R	(1 << MOUSE_BTN_R)
#define MOUSE_BTN_BIT_M	(1 << MOUSE_BTN_M)
typedef int MOUSE_BTN_BIT;

struct MYMOUSEINFO
{
	int click;		// クリック情報.	MOUSE_BTN_BIT_L | MOUSE_BTN_BIT_R | MOUSE_BTN_BIT_M
	int x;			// ｘ座標.		
	int y;			// ｙ座標.
};


enum E_JOY_CON_ID
{
	JOY_CON_0,
	JOY_CON_1,

	JOY_CON_COUNT,			// ゲームで使うコントローラの最大数.
	JOY_CON_INVALID = -1,
	JOY_CON_BEGIN,
};

/*
	DIJOYSTATE構造体に入るデータの詳細
		lX		左スティックを左に倒すと-255まで、右に倒すと+255まで
		lY		左スティックを上に倒すと-255まで、下に倒すと+255まで
		lZ		(R2|ZR)で-255まで (L2|ZL)で+255まで
		lRx		右スティックを左に倒すと-255まで、右に倒すと+255まで
		lRy		右スティックを上に倒すと-255まで、下に倒すと+255まで

		rgdwPOV		押されていないとき0xffffffff
			[0]	十字ボタン上で0、右で9000、下で18000、左で27000、

		rgbButtons	[0]～[9] 押されていないとき0、押されているとき128
			[0]	×ボタン｜Ａボタン
			[1]	〇ボタン｜Ｂボタン
			[2]	□ボタン｜Ｘボタン
			[3]	△ボタン｜Ｙボタン
			[4]	L1ボタン｜Ｌボタン
			[5]	R1ボタン｜Ｒボタン
			[6]	シェアボタン
			[7]	オプションボタン
			[8]	左スティック押し込み
			[9]	右スティック押し込み
*/

// 十字以外のボタン(ネーミングはXBOXコントローラに準拠).
enum E_JOY_BTN_LAW_ID
{
	JOY_BTN_LAW_A,		// 0 : Ａボタン.
	JOY_BTN_LAW_B,		// 1 : Ｂボタン.
	JOY_BTN_LAW_X,		// 2 : Ｘボタン.
	JOY_BTN_LAW_Y,		// 3 : Ｙボタン.
	JOY_BTN_LAW_L,		// 4 : Ｌボタン.
	JOY_BTN_LAW_R,		// 5 : Ｒボタン.
	JOY_BTN_LAW_VW,		// 6 : viewボタン.
	JOY_BTN_LAW_MU,		// 7 : menuボタン.
	JOY_BTN_LAW_L3,		// 8 : 左スティック押し込み.
	JOY_BTN_LAW_R3,		// 9 : 右スティック押し込み.

	JOY_BTN_LAW_COUNT,
	JOY_BTN_LAW_INVALID = -1,
	JOY_BTN_LAW_BEGIN,
};
#define	IS_VALID_BTN_LAW_ID(id)	((JOY_BTN_LAW_BEGIN <= id) && (id < JOY_BTN_LAW_COUNT))

// IDの順番はDXライブラリのビット配列を参考にしている.
enum E_JOY_BTN_ID
{
	JOY_BTN_DN,		//  0 : 十字キー下.
	JOY_BTN_LT,		//  1 : 十字キー左.
	JOY_BTN_RT,		//  2 : 十字キー右.
	JOY_BTN_UP,		//  3 : 十字キー上.
	JOY_BTN_A,		//  4 :	Ａボタン.
	JOY_BTN_B,		//  5 : Ｂボタン.
	JOY_BTN_X,		//  6 : Ｘボタン.
	JOY_BTN_Y,		//  7 : Ｙボタン.
	JOY_BTN_L,		//  8 : Ｌボタン.
	JOY_BTN_R,		//  9 : Ｒボタン.
	JOY_BTN_VW,		// 10 : viewボタン.
	JOY_BTN_MU,		// 11 : menuボタン.
	JOY_BTN_L3,		// 12 : 左スティック押し込み.
	JOY_BTN_R3,		// 13 : 右スティック押し込み.

	JOY_BTN_COUNT,
	JOY_BTN_BEGIN = 0,
};
#define	JOY_BTN_BIT_DN	(1 << JOY_BTN_DN)		// 0x00000001
#define	JOY_BTN_BIT_LT	(1 << JOY_BTN_LT)		// 0x00000002
#define	JOY_BTN_BIT_RT	(1 << JOY_BTN_RT)		// 0x00000004
#define	JOY_BTN_BIT_UP	(1 << JOY_BTN_UP)		// 0x00000008
#define	JOY_BTN_BIT_A	(1 << JOY_BTN_A)		// 0x00000010
#define	JOY_BTN_BIT_B	(1 << JOY_BTN_B)		// 0x00000020
#define	JOY_BTN_BIT_X	(1 << JOY_BTN_X)		// 0x00000040
#define	JOY_BTN_BIT_Y	(1 << JOY_BTN_Y)		// 0x00000080
#define	JOY_BTN_BIT_L	(1 << JOY_BTN_L)		// 0x00000100
#define	JOY_BTN_BIT_R	(1 << JOY_BTN_R)		// 0x00000200
#define	JOY_BTN_BIT_CR	(1 << JOY_BTN_VW)		// 0x00000400
#define	JOY_BTN_BIT_OP	(1 << JOY_BTN_MU)		// 0x00000800
#define	JOY_BTN_BIT_L3	(1 << JOY_BTN_L3)		// 0x00001000
#define	JOY_BTN_BIT_R3	(1 << JOY_BTN_R3)		// 0x00002000
typedef int JOY_BTN_BIT;

struct MYJOYINFO
{
	int button;		// ボタン情報.		MY_PAD_INP_DN | MY_PAD_INP_LT | ...
	int lx, ly;		// 左スティック情報.
	int rx, ry;		// 右スティック情報.
};

// 入力情報管理クラス.
class MyInput
{
private:
	static MyInput sm_inputObj;// このクラス自身のオブジェクト（実体）の宣言.
	MyInput();// 勝手にオブジェクトを追加できないようにコンストラクタをprivate領域に配置.

public:
	~MyInput() { Release(); }
	static MyInput* GetInst() { return &sm_inputObj; }
	
	HRESULT InitInput(HINSTANCE, HWND);		// 入力まわりの初期化.
	void Release();							// リソース解放.

	void UpdateInput(HWND);					// 入力情報を更新.

	// ↓キーボード関連.
	BOOL IsPushKey(BYTE code);				// 指定のキーが押されているか?
	BOOL IsPushKeyOne(BYTE code);			// 指定のキーが押された瞬間か?

	// ↓マウス関連.
	BOOL IsPushMouse(MOUSE_BTN_BIT);		// マウスの（左／中／右）ボタンが押されているか?
	BOOL IsPushMouseOne(MOUSE_BTN_BIT);		// マウスの（左／中／右）ボタンが押された瞬間か?
	void GetMousePos(LONG* px, LONG* py);	// マウス位置の取得.

	// ↓ジョイスティック関連.
	BOOL IsPushBtn(E_JOY_CON_ID, JOY_BTN_BIT);		// ボタンが押されているか?
	BOOL IsPushBtnOne(E_JOY_CON_ID, JOY_BTN_BIT);	// ボタンが押された瞬間か?
	int GetStickLX(E_JOY_CON_ID);					// 左スティックの左右方向の傾きを得る.
	int GetStickLY(E_JOY_CON_ID);					// 左スティックの前後方向の傾きを得る.
	int GetStickRX(E_JOY_CON_ID);					// 右スティックの左右方向の傾きを得る.
	int GetStickRY(E_JOY_CON_ID);					// 右スティックの前後方向の傾きを得る.
	int GetJoyCnt(){return m_nJoyCnt;}				// 接続されているコントローラの数を得る.

private:
	LPDIRECTINPUT8 m_pDInput;						// ダイレクトインプット.
	LPDIRECTINPUTDEVICE8 m_pKeyboardDev;			// キーボードデバイス.
	LPDIRECTINPUTDEVICE8 m_pMouseDev;				// マウスデバイス.
	LPDIRECTINPUTDEVICE8 m_pJoystickDev[JOY_CON_COUNT];// ジョイスティックデバイス（２つまで使える）.

	char		m_keyboardInfo[2][KEYBUF_SIZE];		// キーボード情報（２フレーム分）.
	MYMOUSEINFO	m_mouseInfo[2];						// マウス情報（２フレーム分）.
	MYJOYINFO	m_joystickInfo[JOY_CON_COUNT][2];	// ジョイスティック情報（２フレーム分）.
	int			m_nJoyCnt;
	int			m_nDioiCnt;

	static BOOL CALLBACK EnumDevObjCallback(LPCDIDEVICEOBJECTINSTANCE, LPVOID);
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE*, LPVOID);
};

inline 	MyInput* GetInputInst() { return MyInput::GetInst(); }
