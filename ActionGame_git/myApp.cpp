/*
	myApp.cpp

	アプリケーションの核となるソース
*/
#include "stdafx.h"
#include "myApp.h"
#include "myInput.h"
#include "mySound.h"
#include "myEnemyManager.h"
#include "myExplosionBuffer.h"
#include "myFloorSlip.h"
#include "myEnemyAAA.h"
#include "myBossAAA.h"
#include "myBossBBB.h"
#include "Resource/resource.h"

/***** 残りの作業 *****/

/* bgm、効果音
ゲームクリアの効果音
ゲームオーバーの効果音
*/



//#define FULLSCREEN

// ウィンドウタイトルバーに表示されるバージョン名.
#define WDATE(x, y) _T(x y)
static const TCHAR* version = WDATE(__DATE__, __TIME__);

#define APP_NAME _T("GP3_STG")

// 関数のプロトタイプ宣言.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
const TCHAR* D3DErrStr(HRESULT res);

// staticメンバー変数の実体の定義はソース側（myApp.cpp）に記述する.
MyApp MyApp::sm_myApp;

MyApp::MyApp()
	: hInstance(NULL)			// インスタンスハンドル.
	, hWnd(NULL)				// 表示ウィンドウ.
	, pD3D(NULL)				// Direct3Dインスタンスオブジェクト.
	, pDevice(NULL)				// Direct3Dデバイスオブジェクト.
	, pSprite(NULL)				// スプライト.
	, pFont(NULL)				// ビットマップフォント.
	, pPlayerTex(NULL)			// 自機.
	, pPlayer_WeaponTex(NULL)	// プレイヤーの武器
	, pMatoTex(NULL)			// 当たり判定チェック用の的.
	, pFloorMgr(NULL)			// 床の配列.
	, pFloorSlipTex(NULL)				// すりぬけ床.
	, pEffectTex(NULL)			// 攻撃エフェクト
	, pPlayerBullet(NULL)			// プレイヤーの弾丸.
	, pPlayerBltTex(NULL)		// プレイヤー弾丸テクスチャ.
	, pBossBullet(NULL)			// ボスの弾丸.
	, pEnemyBltTex(NULL)		// 敵弾丸テクスチャ.
	, pBossMgr(NULL)
	, pBossTexA(NULL)			// ボスAのテクスチャ
	, pBossTexB(NULL)			// ボスBのテクスチャ
	, PlayerMeter_Tex(NULL)
	, PlayerMeterFrame_Tex(NULL)
	, PlayerMeterHatsu_Tex(NULL)
	, PlayerMeterMetsu_Tex(NULL)
	, PlayerHPBar_Tex(NULL)
	, PlayerHPBarFrame_Tex(NULL)
	, BossHPBar_Tex(NULL)
	, BossHPBarFrame_Tex(NULL)
	, earth_Tex(NULL)
	, kanban64_Tex(NULL)
	, redmaru_Tex(NULL)
	, kanbanflg(0)
	, nextFireFrame(0)			// 自機が次に弾を撃てるまでの時間（フレーム）.
	, buttons(0)				// ボタンの情報.
	, pPlayer(NULL)
	, pEnemyMgr(NULL)			// 敵の配列.
	, pEnemyTexA(NULL)			// 敵Aのテクスチャ.
	, pEnemyTexB(NULL)			// 敵Bのテクスチャ.
	, pEnemyTexC(NULL)			// 敵Cのテクスチャ.
	, pBackGroundTex(NULL)      // 背景.
	, BackGroundY(0)            // 背景の現在位置.
	, score(0)					// スコア.
	, playerPower(0)			// 自機の耐久力.
	, pExplMgr(NULL)		    // 爆発の配列.
	, pExplosionTex(NULL)
	, joyID(-1)					// ゲームコントローラのID.
	, gameTimer(-1)				// 全体のタイマー.
	, gameState(GAME_STATE_LOAD)// ゲーム全体の状態.
	, stateTimer(0)				// 状態遷移用タイマー.
	, destroyTimer(0)			// ボスを倒した後のウェイトタイマー
	, warningTimer(0)
	, pReadyTex(NULL)			// READY.
	, pStage1Tex(NULL)			// STAGE1.
	, pGameOverTex(NULL)		// GAMEOVER.
	, ptitletextTex(NULL)
	, flash(0)
{
}

bool MyApp::InitApp()
{
	// このプログラムが実行されるときのインスタンスハンドルを取得.
	hInstance = GetModuleHandle(NULL);

	// まずウィンドウクラスを登録する.
	// これはウィンドウ生成後の処理の仕方をWindowsに教えるためである.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));		// 変数wcをゼロクリアする.
	wc.cbSize = sizeof(WNDCLASSEX);				// この構造体の大きさを与える.
	wc.lpfnWndProc = (WNDPROC)WindowProc;		// ウィンドウプロシージャ登録.
	wc.hInstance = hInstance;					// インスタンスハンドルを設定.
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);	// マウスカーソルの登録.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);// 背景をGRAYに.
	wc.lpszClassName = APP_NAME;				// クラス名、CreateWindowと一致させる.
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	RegisterClassEx(&wc);						// 登録.

	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_tprintf(_T("rc: %d, %d -- %d, %d\n"), rc.left, rc.top, rc.right, rc.bottom);

	// ウィンドウを作成する.
	hWnd = CreateWindow(APP_NAME, version, WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	assert(hWnd != NULL);

	// Direct3Dを初期化する.
	HRESULT hr = InitDirect3D();
	if (FAILED(hr)) {
		MessageBox(NULL, D3DErrStr(hr), _T("Direct3D初期化失敗"), MB_OK);
		return false;
	}

	// スプライトの作成.
	hr = D3DXCreateSprite(pDevice, &(pSprite));
	if (FAILED(hr)) {
		MessageBox(NULL, D3DErrStr(hr), _T("スプライト作成失敗"), MB_OK);
		return false;
	}

	// DirectSoundの初期化.
	hr = GetSoundInst()->InitSound(hWnd);
	if (FAILED(hr)) {
		MessageBox(NULL, D3DErrStr(hr), _T("ダイレクトサウンド作成失敗"), MB_OK);
		return false;
	}

	// 入力デバイスの初期化.
	GetInputInst()->InitInput(hInstance, hWnd);
	if (GetInputInst()->GetJoyCnt() == 0) {
		//MessageBox(NULL, _T("ジョイスティックが繋がっていません\r\nキーボードで操作します"), _T("JOYSTICK"), MB_OK);
	}

	// 床管理クラスのオブジェクトを生成.
	pFloorMgr = new FloorManager(10);
	if (pFloorMgr == NULL || !pFloorMgr->Init()) {
		return false;
	}

	// 敵管理クラスのオブジェクトを生成.
	pEnemyMgr = new EnemyManager(70);
	if (pEnemyMgr == NULL || !pEnemyMgr->Init()) {
		return false;
	}

	// ボス管理クラスのオブジェクトを生成.
	pBossMgr = new BossManager(10);
	if (pBossMgr == NULL || !pBossMgr->Init()) {
		return false;
	}

	// 弾丸管理クラス（プレイヤー用）のオブジェクトを生成&初期化.
	pPlayerBullet = new BulletBuffer(1);
	if (pPlayerBullet == NULL || !pPlayerBullet->Init()) {
		return false;
	}

	// 弾丸管理クラス（敵用）のオブジェクトを生成&初期化.
	pBossBullet = new BulletBuffer(100);
	if (pBossBullet == NULL || !pBossBullet->Init()) {
		return false;
	}

	// 同時爆発数最大100個分のバッファを準備.
	pExplMgr = new ExplosionBuffer(100);
	if (pExplMgr == NULL || !pExplMgr->Init()) {
		return false;
	}

	// テクスチャを読み込む。エラー時にはLoadTextureは例外を発生させる.
	// LoadTextureごとにif判定をするのは面倒なので、try～throw～catchを使う.
	try {
		pFont = MyBitmapFont::LoadFont(pDevice, _T("data/image/fontBitmap.png"));

		pPlayerTex			= MyTexture::LoadTexture(pDevice, _T("data/image/player_kari.png"));
		pPlayer_WeaponTex   = MyTexture::LoadTexture(pDevice, _T("data/image/player_weapon.png"));
		pPlayerBltTex		= MyTexture::LoadTexture(pDevice, _T("data/image/playerbullet.png"));
		pEnemyBltTex		= MyTexture::LoadTexture(pDevice, _T("data/image/bossbullet.png"));
		pBossTexA			= MyTexture::LoadTexture(pDevice, _T("data/image/bossTexture_2.png"));
		pBossTexB			= MyTexture::LoadTexture(pDevice, _T("data/image/bossTexture_1.png"));
		pMatoTex			= MyTexture::LoadTexture(pDevice, _T("data/image/mato.png"));
		pExplosionTex	    = MyTexture::LoadTexture(pDevice, _T("data/image/effect.png"));

		PlayerMeter_Tex		 = MyTexture::LoadTexture(pDevice, _T("data/image/playerMeter.png"));
		PlayerMeterFrame_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/playerMeterframe.png"));
		PlayerMeterHatsu_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/PlayerMeterHatsudou.png"));
		PlayerMeterMetsu_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/PlayerMeterTenmetsu.png"));
		PlayerHPBar_Tex      = MyTexture::LoadTexture(pDevice, _T("data/image/playerHPbar.png"));
		PlayerHPBarFrame_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/playerHPbarframe.png"));
		BossHPBar_Tex        = MyTexture::LoadTexture(pDevice, _T("data/image/bossHPbar.png"));
		BossHPBarFrame_Tex   = MyTexture::LoadTexture(pDevice, _T("data/image/bossHPbarframe.png"));
		earth_Tex			 = MyTexture::LoadTexture(pDevice, _T("data/image/earth.png"));
		kanban64_Tex	     = MyTexture::LoadTexture(pDevice, _T("data/image/kanban64.png"));
		redmaru_Tex			 = MyTexture::LoadTexture(pDevice, _T("data/image/mark_maru.png"));

		pFloorSlipTex		 = MyTexture::LoadTexture(pDevice, _T("data/image/floorSlip.png"));
		ptitletextTex = MyTexture::LoadTexture(pDevice, _T("data/image/titletext.png"));

		// 数値表示用テクスチャの読み込み.
		MyTexture* pTex = MyTexture::LoadTexture(pDevice, _T("data/image/numbers.png"));
		// スコア表示に使うテクスチャの情報を設定.
		//pScoreDisp->Init(pTex, 64, 64);// 64:数字１つ分のテクスチャサイズ.

		// SE/BGMの読み込み.
		GetSoundInst()->LoadSoundData();
	}
	// catch句はtryの直後に記述する.
	catch (HRESULT /*hr*/) {
		return false;
	}

	return true;
}

// メインループ.
void MyApp::MainLoop()
{
	// BGMを再生.
	//GetSoundInst()->PlayBGMs(BGM_0);

	ShowWindow(hWnd, SW_SHOWNORMAL);	// 作成したウィンドウを表示する.
	timer.StartTimer();
	fps.StartTimer();

	// イベントループ.
	// ブロック型関数GetMessageではなくノンブロック型関数のPeekMessageを使う.
	MSG msg;
	bool flag = true;
	while (flag) {
		// メッセージがあるかどうか確認する.
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージがあるので処理する.
			if (GetMessage(&msg, NULL, 0, 0) != 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				flag = 0;
			}
		}
		else {
			if (UpdateData()) {	// 位置の再計算.
				// ＢＧＭのフェードイン／アウトのためのボリューム更新.
				GetSoundInst()->UpdateAllBGM();
				DrawData();		// 描画.
			}
		}
		// Sleepなど行っていないが、DrawData関数内のpDevice->Presentが.
		// 画面更新待ちを行うので、結果的にVSyncごとにイベントループが回る.
	}
}

// マクロを複数行で書きたいときは\を行末に付けることで可能.
#define RELEASE(__xx__)    \
	if (__xx__)            \
	{                      \
		__xx__->Release(); \
		__xx__ = NULL;     \
	}

// DataSetを解放する.
void MyApp::ReleaseData()
{
	// サウンド解放.
	GetSoundInst()->Release();

	// テクスチャの開放.
	MyTexture::ReleaseAll();

	// 動的オブジェクトの開放.
	if (pExplMgr) { delete pExplMgr; }
	if (pFloorMgr) { delete pFloorMgr; }
	if (pEnemyMgr) { delete pEnemyMgr; }
	if (pBossMgr) { delete pBossMgr; }
	if (pPlayerBullet) { delete pPlayerBullet; }
	if (pBossBullet) { delete pBossBullet; }
	if (pFont) { delete pFont; }

	// 入力デバイスの解放.
	GetInputInst()->Release();

	// DirectXデバイスの開放.
	RELEASE(pSprite);
	RELEASE(pDevice);
	RELEASE(pD3D);
}

void MyApp::ResetGameData()
{
	pFloorMgr->ResetAll();
	pEnemyMgr->ResetAll();
	pBossMgr->ResetAll();
	pExplMgr->ResetAll();
	pPlayerBullet->ResetAll();
	pBossBullet->ResetAll();

	// プレイヤークラスのオブジェクトを生成
	pPlayer = new Player(pPlayerTex, pPlayer_WeaponTex);
	pPlayer->Init(D3DXVECTOR2(WIDTH / 2, HEIGHT - 64 * SIZE), D3DXVECTOR2(0.0f, -10.0f), 32.0f, 32.0f, 48.0f, 48.0f, 25);

	//CreateFloorSlip(D3DXVECTOR2(500, HEIGHT - 100 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 160, 32);

	CreateFloorSlip(D3DXVECTOR2(672, HEIGHT - 144 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 160, 32);

	CreateFloorSlip(D3DXVECTOR2(128, HEIGHT - 144 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 160, 32);

	//CreateEnemyA(D3DXVECTOR2(300, HEIGHT - 80 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 48, 48, 30);
	//CreateEnemyA(200, HEIGHT - 66, 0.0f, 0.0f, 5);
	//CreateEnemyA(600, HEIGHT - 66, 1.0f, 0.0f, 3);

	switch (bossState)
	{
	case BOSS_1:
		//ボスAの初期化
		CreateBossA(800.0f, HEIGHT - 128.0f * SIZE, 0.0f, 0.0f, 64.0f, 60);
		break;
	case BOSS_2:
		//ボスBの初期化
		CreateBossB(800.0f, HEIGHT - 128.0f * SIZE, 0.0f, 0.0f, 64.0f, 90);
		break;
	default:
		//ボスAの初期化
		//CreateBossA(800.0f, HEIGHT - 128.0f * SIZE, 0.0f, 0.0f, 64.0f, 26);
		break;
	}

	playerPower = 10;
	nextFireFrame = 0;
	stateTimer = 0;
	kanbanflg = 0;
}

// Direct3Dを初期化する.
// return 発生したエラーまたはD3D_OK.
HRESULT MyApp::InitDirect3D()
{
	// Direct3Dインスタンスオブジェクトを生成する.
	// D3D_SDK_VERSIONと、ランタイムバージョン番号が適切でないと、NULLが返る.
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	// NULLならランタイムが不適切.
	if (pD3D == NULL) return E_FAIL;

	// PRESENTパラメータをゼロクリアし、適切に初期化.
	D3DPRESENT_PARAMETERS d3dpp;	// デバイス作成時のパラメータ.
	ZeroMemory(&(d3dpp), sizeof(d3dpp));
#ifdef FULLSCREEN
	d3dpp.Windowed = FALSE;
	//int fx = GetSystemMetrics(SM_CXSCREEN);
	//int fy = GetSystemMetrics(SM_CYSCREEN);
	int fx = WIDTH;
	int fy = HEIGHT;
	_tprintf(_T("フルスクリーン %d x %d\n"), fx, fy);
	d3dpp.BackBufferHeight = fy;
	d3dpp.BackBufferWidth = fx;
#else
	d3dpp.Windowed = TRUE;
#endif
	d3dpp.BackBufferHeight = HEIGHT;
	d3dpp.BackBufferWidth = WIDTH;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// バックバッファはRGBそれぞれ８ビットで.
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	// Present時に垂直同期に合わせる.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// D3Dデバイスオブジェクトの作成。HAL&HARD.
	HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("HAL & HARD\n"));
		return hr;
	}
	// D3Dデバイスオブジェクトの作成。HAL&SOFT.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("HAL & SOFT\n"));
		return hr;
	}
	// D3Dデバイスオブジェクトの作成。REF&HARD.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("REF & HARD\n"));
		return hr;
	}
	// D3Dデバイスオブジェクトの作成。REF&SOFT.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("REF & SOFT\n"));
		return hr;
	}
	return hr;
}

// データをアップデートする.
BOOL MyApp::UpdateData()
{
	LONGLONG us; UINT frames;
	timer.GetPassTime(&us, &frames);
	if (us < 1000000 / 60) {
		return FALSE;// 60FPSで更新.
	}
	//timer.StartTimer();

	gameTimer++;
	stateTimer++;
	buttons = 0;

	// 入力情報を更新.
	MyInput* pInput = GetInputInst();
	pInput->UpdateInput(hWnd);

	// ゲーム状態に応じた処理に変更.
	switch (gameState)
	{
	case GAME_STATE_LOAD:
	{

		// 次の状態を準備状態にまずしておく.
		GameState eNextState = GAME_STATE_TITLE;
		BossState NextState = BOSS_0;

		// 状態を遷移.
		gameState = eNextState;
		bossState = NextState;

		ResetGameData();
		break;
	}

	case GAME_STATE_TITLE:
	{
		if (pInput->IsPushBtn(JOY_CON_0, JOY_BTN_BIT_A)) {
			buttons |= JOY_BTN_BIT_A;// 
		}
		if (pInput->IsPushKey(DIK_SPACE)) {
			buttons |= JOY_BTN_BIT_A;// 
		}

		// Aボタン(SPACE-Key)だけが押されたらゲーム開始.
		if (buttons == JOY_BTN_BIT_A)
		{
			GetSoundInst()->StopAllBGM();
			// ＢＧＭを再生.
			GetSoundInst()->PlayBGM(BGM_3, true);
			gameState = GAME_STATE_TUTORIAL;
			stateTimer = 0;
			gameTimer = 0;
		}
		break;
	}

	case GAME_STATE_WARNING:
	{
		warningTimer++;
		if (warningTimer > 180) {
			//GetSoundInst()->PlaySE(SE_START);// スタート音を鳴らす.
			GetSoundInst()->StopAllBGM();
			// ＢＧＭを再生.
			switch (bossState)
			{
			case BOSS_1:
				GetSoundInst()->PlayBGM(BGM_4, true);
				break;
			case BOSS_2:
				GetSoundInst()->PlayBGM(BGM_5, true);
				break;
			default:
				break;
			}
			gameState = GAME_STATE_PLAYING;
			stateTimer = 0;
			gameTimer = 0;
			warningTimer = 0;
		}
		break;
	}

	case GAME_STATE_TUTORIAL:
	{
		UpdatePlaying(true);

		if (kanbanflg == 0 && (pInput->IsPushKey(DIK_A) || pInput->IsPushKey(DIK_D))) kanbanflg = 1;
		if (kanbanflg == 1 && pInput->IsPushKey(DIK_P)) kanbanflg = 2;
		if (kanbanflg == 2 && pInput->IsPushKey(DIK_SPACE)) {
			kanbanflg = 3;
			CreateFloorSlip(D3DXVECTOR2(256, HEIGHT - 144 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 160, 32);
			CreateEnemyA(D3DXVECTOR2(160, HEIGHT - 192 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 48, 48, 30);
			CreateFloorSlip(D3DXVECTOR2(544, HEIGHT - 144 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 160, 32);
			CreateEnemyA(D3DXVECTOR2(704, HEIGHT - 192 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 48, 48, 30);
		}
		bool bulletflg = false; // 弾があるかどうか
		for (int i = 0; i < pPlayerBullet->BulletMax(); i++) {
			if (pPlayerBullet->PPBuffer(i)) {
				bulletflg = true;
			}
		}
		if (kanbanflg == 3 && pInput->IsPushKey(DIK_C) && pPlayer->GetMeter() == 0 && bulletflg) kanbanflg = 4;

		if (pInput->IsPushKey(DIK_E)) {
			buttons |= JOY_BTN_BIT_A;// 
		}

		// E-Keyが押されたらゲーム画面へ.
		if (buttons == JOY_BTN_BIT_A && kanbanflg >= 4 && destroyTimer == 0)
		{
			bossState = BOSS_1;
			ResetGameData();
			gameState = GAME_STATE_WARNING;
			GetSoundInst()->StopAllBGM();
			GetSoundInst()->PlaySE(SE_WARNING);// WARNING音を鳴らす.
			stateTimer = 0;
			gameTimer = 0;
		}

		break;
	}

	case GAME_STATE_PLAYING:
	{
		UpdatePlaying(true);

		// プレイヤーのパワーがゼロになったら、ゲームオーバー.
		if (playerPower <= 0)
		{
			GetSoundInst()->StopAllBGM();
			GetSoundInst()->PlaySE(SE_DEAD);
			gameState = GAME_STATE_DEAD;
			stateTimer = 0;
		}
		
		/* デバッグ用
		if (pInput->IsPushKey(DIK_Q)) {
			buttons |= JOY_BTN_BIT_A;// 
		}

		//	Q-Keyが押されたらクリア画面へ.
		if (buttons == JOY_BTN_BIT_A)
		{
			gameState = GAME_STATE_CLEAR;
		}
		*/

		break;
	}

	case GAME_STATE_DEAD:
	{
		if (pInput->IsPushKey(DIK_BACK)) {
			buttons |= JOY_BTN_BIT_A;// 
		}

		// BACKSPACE-Keyだけが押されたらタイトルに戻る.
		if (buttons == JOY_BTN_BIT_A)
		{
			gameState = GAME_STATE_LOAD;
			stateTimer = 0;
			gameTimer = 0;
			playerPower = 10;
			score = 0;
		}
		break;
	}

	case GAME_STATE_CLEAR:
	{
		if (pInput->IsPushKey(DIK_BACK)) {
			buttons |= JOY_BTN_BIT_A;// 
		}

		// BACKSPACE-Keyだけが押されたらタイトルに戻る.
		if (buttons == JOY_BTN_BIT_A)
		{
			gameState = GAME_STATE_LOAD;
			stateTimer = 0;
			gameTimer = 0;
			playerPower = 10;
			score = 0;
		}
		break;
	}
	}
	return TRUE;
}

// GAME_PLAYING状態の時のUpdate
// playableがtrueであれば自機操作
void MyApp::UpdatePlaying(bool playable)
{
	if (playable) {
		MyInput* pInput = GetInputInst();

		// ジョイスティックでの操作.
		if (pInput->GetJoyCnt() > 0) {
			// 左ステックで移動.
			int jx = pInput->GetStickLX(JOY_CON_0);
			int jy = pInput->GetStickLY(JOY_CON_0);

			// あそびを吸収するため、一定値より大きいか(小さいか)を判定する.
			if (abs(jx) > 10) {
				//player_pos.x += jx / 50;
			}
			if (abs(jy) > 10) {
				//player_pos.y += jy / 50;
			}

			if (pInput->IsPushBtn(JOY_CON_0, JOY_BTN_BIT_A)) {
				buttons |= JOY_BTN_BIT_A;// Ａボタンで弾丸発射.
			}
		}

		// キーボードでの操作.
		// プレイヤーを更新、ただしすり抜け対策のため時分割する.
		pPlayer->Update(1.0f / 10);

		// プレイヤーの攻撃が終了したことを検知し、敵とボスの被ダメージフラグをfalseにする
		if (pPlayer->AttackEnd()) {
			for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
				if (pEnemyMgr->PPBuffer(i))pEnemyMgr->DamageReset(0x2, &i);
			}
			for (int i = 0; i < pBossMgr->BossMax(); i++) {
				if (pBossMgr->PPBuffer(i))pBossMgr->DamageReset(0x2 , &i);
			}
		}

		if (!pPlayerBullet->Next() == NULL) {
			for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
				if (pEnemyMgr->PPBuffer(i))pEnemyMgr->DamageReset(0x1, &i);
			}
			for (int i = 0; i < pBossMgr->BossMax(); i++) {
				if (pBossMgr->PPBuffer(i))pBossMgr->DamageReset(0x1, &i);
			}
		}

		// すりぬけ床を更新、ただしすり抜け対策のため時分割する.
		pFloorMgr->Update(1.0f / 10);

		// 敵を更新、ただしすり抜け対策のため時分割する.
		pEnemyMgr->Update(1.0f / 10);

		// 弾丸とボスを更新、ただしすり抜け対策のため時分割する.
		for (int i = 0; i < 10; i++)
		{
			pPlayerBullet->Update(1.0f / 10);
			pBossBullet->Update(1.0f / 10);
		}
		pBossMgr->Update(1.0f / 10);

		// 爆発には当たり判定はないので、時分割は不要.
		pExplMgr->Update(1);

		/*
		//Debug
		//Fire
		if (pInput->IsPushKey(DIK_X))
		{
			//_tprintf(_T("xボタンが押されました\n"));
			pBossMgr->ChangeAction(1, 0);
		}
		//Jump
		if (pInput->IsPushKey(DIK_Z))
		{
			//_tprintf(_T("xボタンが押されました\n"));
			pBossMgr->ChangeAction(2, 0);
		}
		*/

		// 敵と武器の当たり判定（武器を出している最中ならば）
		for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
			if (pEnemyMgr->PPBuffer(i)) {
				if (pPlayer->IsWeapon() && Collision(pPlayer->GetWeaponXY(), pPlayer->GetWeaponW() / 2, pEnemyMgr->GetXY(&i), 48.0f / 2) && (pEnemyMgr->IsDamage(&i) & 0x1) == 0) { // 武器の座標、武器の半径、敵の座標、敵の半径
					pEnemyMgr->DamageFlg(0x1, &i);
					// 中心点の距離 
					float lengthX = (pPlayer->GetWeaponXY().x + pEnemyMgr->GetXY(&i).x) / 2;

					float lengthY = (pPlayer->GetWeaponXY().y + pEnemyMgr->GetXY(&i).y) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					pEnemyMgr->Damage(1, i);
					GetSoundInst()->PlaySE(SE_PLAYERATTACKHIT);
					GetPlayer()->PlusMeter(1);
				}
			}
		}

		// 敵とプレイヤーの当たり判定
		// 当たり判定が厳しすぎないように、敵の当たり判定を見た目の2/3にする
		for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
			if (pEnemyMgr->PPBuffer(i)) {
				if (Collision(pPlayer->GetXY(), pPlayer->GetSize() / 2, D3DXVECTOR2(pEnemyMgr->GetXY(&i).x + 32.0f / 2, pEnemyMgr->GetXY(&i).y + 32.0f / 2), 32.0f / 2) && !pPlayer->IsDamage()) { // 武器の座標、武器の半径、敵の座標、敵の半径
					pPlayer->DamageFlg(true);
					// 中心点の距離 
					float lengthX = ((pPlayer->GetXY().x + pPlayer->GetSize() / 2) + ((pEnemyMgr->GetXY(&i).x) + 48.0f / 2)) / 2;

					float lengthY = ((pPlayer->GetXY().y + pPlayer->GetSize() / 2) + ((pEnemyMgr->GetXY(&i).y) + 48.0f / 2)) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					bool direct = false;
					if (((pEnemyMgr->GetXY(&i).x) + 48.0f / 2) - (pPlayer->GetXY().x + pPlayer->GetSize() / 2) > 0) direct = true;
					pPlayer->Damage(1, direct);
					GetSoundInst()->PlaySE(SE_ATTACKHIT);
				}
			}
		}

		D3DXVECTOR2 pos = { 0, 0 };
		float size = 0;
		// 敵とプレイヤーの弾丸の当たり判定
		for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
			if (pEnemyMgr->PPBuffer(i)) {
				for (int j = 0; j < pPlayerBullet->BulletMax(); j++) {
					if (pPlayerBullet->PPBuffer(j)) {
						pPlayerBullet->GetXY(&j, pos.x, pos.y);
						pPlayerBullet->GetSize(&j, size);
						if (Collision(pos, size / 2, pEnemyMgr->GetXY(&i), pEnemyMgr->GetSize(&i) / 2) && (pEnemyMgr->IsDamage(&i) & 0x2) == 0) { // 武器の座標、武器の半径、ボスの座標、ボスの半径
							pEnemyMgr->DamageFlg(0x2, &i);
							// 中心点の距離 
							float lengthX = (pos.x + pEnemyMgr->GetXY(&i).x) / 2;

							float lengthY = (pos.y + pEnemyMgr->GetXY(&i).y) / 2;

							pExplMgr->Explose(lengthX, lengthY);
							pEnemyMgr->Damage(10, i);
							GetSoundInst()->PlaySE(SE_PLAYERBULLETHIT);
							GetPlayer()->PlusMeter(0);
						}
					}
				}
			}
		}


		// ボスとプレイヤーの武器の当たり判定（武器を出している最中ならば）
		for (int i = 0; i < pBossMgr->BossMax(); i++) {
			if (pBossMgr->PPBuffer(i)) {
				if (pPlayer->IsWeapon() && Collision(pPlayer->GetWeaponXY(), pPlayer->GetWeaponW() / 2, pBossMgr->GetXY(&i), pBossMgr->GetSize(&i) / 2) && (pBossMgr->IsDamage(&i) & 0x1) == 0) { // 武器の座標、武器の半径、ボスの座標、ボスの半径
					pBossMgr->DamageFlg(0x1, &i);
					// 中心点の距離 
					float lengthX = (pPlayer->GetWeaponXY().x + pBossMgr->GetXY(&i).x) / 2;

					float lengthY = (pPlayer->GetWeaponXY().y + pBossMgr->GetXY(&i).y) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					pBossMgr->Damage(1, i);
					GetSoundInst()->PlaySE(SE_PLAYERATTACKHIT);
					GetPlayer()->PlusMeter(1);
				}
			}
		}

		// ボスとプレイヤーの当たり判定
		// 当たり判定が厳しすぎないように、ボスの当たり判定を見た目の2/3にする
		for (int i = 0; i < pBossMgr->BossMax(); i++) {
			if (pBossMgr->PPBuffer(i)) {
				if (Collision(pPlayer->GetXY(), pPlayer->GetSize() / 2, D3DXVECTOR2(pBossMgr->GetXY(&i).x + pBossMgr->GetSize(&i) / 3 * 2 / 2, pBossMgr->GetXY(&i).y + pBossMgr->GetSize(&i) / 3 * 2 / 2), pBossMgr->GetSize(&i) / 3 * 2 / 2) && !pPlayer->IsDamage()) { // 武器の座標、武器の半径、敵の座標、敵の半径
					pPlayer->DamageFlg(true);
					// 中心点の距離 
					float lengthX = (pPlayer->GetXY().x + (pBossMgr->GetXY(&i).x) + pBossMgr->GetSize(&i) / 3 * 2 / 2) / 2;

					float lengthY = (pPlayer->GetXY().y + (pBossMgr->GetXY(&i).y) + pBossMgr->GetSize(&i) / 3 * 2 / 2) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					bool direct = false;
					if (lengthX > pPlayer->GetXY().x - pPlayer->GetSize() / 4) direct = true;
					pPlayer->Damage(1, direct);
					GetSoundInst()->PlaySE(SE_ATTACKHIT);
				}
			}
		}

		pos = { 0, 0 };
		size = 0;
		// ボスの弾丸とプレイヤーの当たり判定
				for (int i = 0; i < pBossBullet->BulletMax(); i++) {
					if (pBossBullet->PPBuffer(i)) {
						pBossBullet->GetXY(&i, pos.x, pos.y);
						pBossBullet->GetSize(&i, size);
						if (Collision(pPlayer->GetXY(), pPlayer->GetSize() / 2, pos, size / 2) && !pPlayer->IsDamage()) { // 武器の座標、武器の半径、ボスの座標、ボスの半径
							pPlayer->DamageFlg(true);
							// 中心点の距離 
							float lengthX = (pPlayer->GetXY().x + pos.x) / 2;

							float lengthY = (pPlayer->GetXY().y + pos.y) / 2;

							pExplMgr->Explose(lengthX, lengthY);
							bool direct = false;
							if (lengthX > pPlayer->GetXY().x - pPlayer->GetSize() / 4) direct = true;
							pPlayer->Damage(1, direct);
							GetSoundInst()->PlaySE(SE_ATTACKHIT);
							// 衝突したので、弾丸を無効に.
							pBossBullet->Deactive(&i);
						}
					}
				}

		pos = { 0, 0 };
		size = 0;
		// ボスとプレイヤーの弾丸の当たり判定
		for (int i = 0; i < pBossMgr->BossMax(); i++) {
			if (pBossMgr->PPBuffer(i)) {
				for (int j = 0; j < pPlayerBullet->BulletMax(); j++) {
					if (pPlayerBullet->PPBuffer(j)) {
						pPlayerBullet->GetXY(&j, pos.x, pos.y);
						pPlayerBullet->GetSize(&j, size);
						if (Collision(pos, size / 2, pBossMgr->GetXY(&i), pBossMgr->GetSize(&i) / 2) && (pBossMgr->IsDamage(&i) & 0x2) == 0) { // 武器の座標、武器の半径、ボスの座標、ボスの半径
							pBossMgr->DamageFlg(0x2, &i);
							// 中心点の距離 
							float lengthX = (pos.x + pBossMgr->GetXY(&i).x) / 2;

							float lengthY = (pos.y + pBossMgr->GetXY(&i).y) / 2;

							pExplMgr->Explose(lengthX, lengthY);
							pBossMgr->Damage(10, i);
							GetSoundInst()->PlaySE(SE_PLAYERBULLETHIT);
							GetPlayer()->PlusMeter(0);
						}
					}
				}
			}
		}
	}
}

// DataSetに基づいて、スプライトを描画.
// return 発生したエラー.
HRESULT MyApp::DrawData()
{
	D3DXVECTOR3 cnt(0, 0, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	D3DCOLOR rgb = D3DCOLOR_XRGB(128, 128, 128);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // 画面全体を消去.

		bool bossflg = true; // ボスを倒したかどうか
		if (gameState == GAME_STATE_PLAYING) {
			bossflg = false;
			for (int i = 0; i < pBossMgr->BossMax(); i++) {
				if (pBossMgr->PPBuffer(i)) {
					bossflg = true;
				}
			}
		}

		if (gameState == GAME_STATE_PLAYING || gameState == GAME_STATE_TUTORIAL) {
			if ((!bossflg || pPlayer->GetHP() == 0) && destroyTimer == 0) { // ボスを倒したか、プレイヤーがやられたら
				if (pPlayer->GetHP() == 0) {
					bossState = BOSS_0;
				}
				destroyTimer = stateTimer;
				flash = 255;
				D3DCOLOR rgb = D3DCOLOR_XRGB(255, 255, 255);
				pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // 画面全体を消去.
				GetSoundInst()->StopAllBGM();
			}

			if (destroyTimer > 0) {
				if (flash < 129) {
					flash = 128;
					if (stateTimer - destroyTimer > 240) {
						switch (bossState)
						{
						case BOSS_0:
							destroyTimer = 0;
							if (gameState == GAME_STATE_PLAYING) {
								gameState = GAME_STATE_DEAD;
								GetSoundInst()->PlaySE(SE_GAMEOVER);
							}
							else {
								gameState = GAME_STATE_TITLE;
							}
							ResetGameData();
							break;
						case BOSS_1:
							destroyTimer = 0;
							gameState = GAME_STATE_WARNING;
							GetSoundInst()->PlaySE(SE_WARNING);// WARNING音を鳴らす.
							bossState = BOSS_2;
							ResetGameData();
							break;
						case BOSS_2:
							destroyTimer = 0;
							gameState = GAME_STATE_CLEAR;
							GetSoundInst()->PlaySE(SE_CLEAR);
							break;
						default:
							break;
						}
					}
				}
				else {
					flash--;
					D3DCOLOR rgb = D3DCOLOR_XRGB(flash, flash, flash);
					pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // 画面全体を消去.
				}
			}
		}

	// ゲームの状態による描画分け.
	switch (gameState)
	{
	case GAME_STATE_LOAD:
	{
	}
	break;

	case GAME_STATE_TITLE:
	{
		rgb = D3DCOLOR_XRGB(128, 160, 128);
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // 画面全体を消去.
		cnt.x = 128; cnt.y = 32;
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2 - 64;
		//pSprite->Draw(pTitleTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2 + 64;
		if (stateTimer % 30 < 20)
		{
			pSprite->Draw(redmaru_Tex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		}

		//fps.StartTimer();

		Draw(pSprite, ptitletextTex->GetTexture(), D3DXVECTOR3(WIDTH / 2 - 310, 120, 0), RECT{ 0, 0, 311, 52 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, pPlayerTex->GetTexture(), D3DXVECTOR3(WIDTH / 2 - 192, HEIGHT / 2 + 32, 0), RECT{ 0, 128, 32, 160 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, pBossTexA->GetTexture(), D3DXVECTOR3(WIDTH / 2 + 160 - 32, HEIGHT / 2- 32, 0), RECT{ 0, 0, 64, 64 }, 0.0f, 0.0f, false, false, false, 1);

		//sprintf_s(text, "Boss Rush");
		//pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2, 8, 0xFFFFFFFF, 3.3);
		//pFont->DrawBmpText(text, WIDTH / 6 - 36, HEIGHT / 6 - 36, 8, 0xFFFFFFFF, 3);

		sprintf_s(text, "press space");
		pFont->DrawBmpText(text, WIDTH / 4 - 42, HEIGHT / 4 + 64, 8, 0xFFFFFFFF, 2);
	}
	break;

	case GAME_STATE_WARNING:
	{
		cnt.x = 128; cnt.y = 32;
		pos.x = WIDTH / 2 - 32; pos.y = HEIGHT / 2;
		//if (stateTimer % 30 < 20)
		{
			sprintf_s(text, "WARNING");
			//pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2, 8, 0xFFFFFFFF, 3.3);
			pFont->DrawBmpText(text, WIDTH / 6 - 24, HEIGHT / 6 - 36, 8, 0xFFFFFFFF, 3);
			switch (bossState)
			{
			case BOSS_1:
				Draw(pSprite, pBossTexA->GetTexture(), pos, RECT{ 0, 0, 64, 64 }, 0.0f, 0.0f, false, false, false, 1);
				break;
			case BOSS_2:
				Draw(pSprite, pBossTexB->GetTexture(), pos, RECT{ 0, 0, 64, 64 }, 0.0f, 0.0f, false, false, false, 1);
				break;
			default:
				break;
			}
		}
	}
	break;

	case GAME_STATE_TUTORIAL:
	{
		DrawDataPlaying();

		if (kanbanflg >= 4 && destroyTimer == 0) {
			sprintf_s(text, "press E-key");
			pFont->DrawBmpText(text, WIDTH / 6 - 42, HEIGHT / 4 - 128, 8, 0xFFFFFFFF, 3);
		}
		/*
		rgb = D3DCOLOR_XRGB(128, 128, 160);
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // 画面全体を消去.
		sprintf_s(text, "tutorial...");
		pFont->DrawBmpText(text, WIDTH / 6 - 42, HEIGHT / 6 - 36, 8, 0xFFFFFFFF, 3);

		sprintf_s(text, "press W-key");
		pFont->DrawBmpText(text, WIDTH / 4 - 42, HEIGHT / 4 + 21, 8, 0xFFFFFFFF, 2);
		*/
	}
	break;

	case GAME_STATE_PLAYING:
	{
		//rgb = D3DCOLOR_XRGB(163, 213, 226);
		//pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // 画面全体を消去.
		// 画像の表示.
		DrawDataPlaying();

		/* デバッグ用
		sprintf_s(text, "game playing...");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2, 8, 0xFFFFFFFF);

		sprintf_s(text, "press Q-key");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2 + 128, 8, 0xFFFFFFFF);
		*/
	}
	break;

	case GAME_STATE_DEAD:
	{
		// 敵と弾丸の表示.
		//DrawDataPlaying();

		rgb = D3DCOLOR_XRGB(64, 64, 128);
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // 画面全体を消去.
		sprintf_s(text, "game over...");
		pFont->DrawBmpText(text, WIDTH / 6 - 52, HEIGHT / 6 - 36, 8, 0xFFFFFFFF, 3);

		sprintf_s(text, "press backspace");
		pFont->DrawBmpText(text, WIDTH / 4 - 60, HEIGHT / 4 + 21, 8, 0xFFFFFFFF, 2);
	}
	break;

	case GAME_STATE_CLEAR:
	{
		//fps.StartTimer();

		rgb = D3DCOLOR_XRGB(160, 128, 128);
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // 画面全体を消去.
		sprintf_s(text, "game clear!!!");
		pFont->DrawBmpText(text, WIDTH / 6 - 52, HEIGHT / 6 - 36, 8, 0xFFFFFFFF, 3);

		sprintf_s(text, "press backspace");
		pFont->DrawBmpText(text, WIDTH / 4 - 60, HEIGHT / 4 + 21, 8, 0xFFFFFFFF, 2);
	}
	break;
	}

	// 描画を開始（シーン描画の開始）.
	pDevice->BeginScene();

	// スプライトの描画を開始.
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	/******************
	* 
	* 常に表示する情報
	* 
	******************/

	// スプライトの描画を終了、シーンの描画を終了.
	pSprite->End();

	// デバッグ用の情報表示.
#if defined(_DEBUG)

	LONGLONG us; UINT frames;
	fps.GetPassTime(&us, &frames);

	char txt[256] = { 0 };
	sprintf_s(txt, "microSeconds: %llu, FPS:%d\n", us, frames);
	pFont->DrawBmpText(txt, 20, 660, 8, 0xFFFFFFFF);

	/*
	sprintf_s(txt, "game: %04d state: %04d pad %d:(%04.1f, %03.1f) btn:%04X  line=%03d  power=%d",
		gameTimer, stateTimer, joyID, playerX, playerY,
		buttons, pStage->GetCurrent(), playerPower);
		*/

	pFont->DrawBmpText(txt, 20, 680, 8, 0xFFFFFFFF);

	
	//sprintf_s(txt, "bullet:%s", pBossBullet->GetText());
	//pFont->DrawBmpText(txt, 20, 500, 8, 0xFFFFFFFF);

	//int u = 0;
	//sprintf_s(txt, "enemies:%x", pBossMgr->IsDamage(&u));
	//pFont->DrawBmpText(txt, 20, 500, 8, 0xFFFFFFFF);
	

	//sprintf_s(txt, "enemies:%s", pEnemyMgr->GetText());
	//pFont->DrawBmpText(txt, 20, 500, 8, 0xFFFFFFFF);

	//fps.StartTimer();

	//sprintf_s(txt, "explosion:%s", pBossMgr->GetText());
	//pFont->DrawBmpText(txt, 20, 400, 8, 0xFFFFFFFF);

	//sprintf_s(txt, "player:%s", pPlayer->GetXY().x);
	//pFont->DrawBmpText(txt, 20, 500, 8, 0xFFFFFFFF);
#endif

	pDevice->EndScene();

	// 実際に画面に表示。バックバッファからフロントバッファへの転送.
	// デバイス生成時のフラグ指定により、ここでVSYNCを待つ.
	pDevice->Present(NULL, NULL, NULL, NULL);

	return D3D_OK;
}

// 画像の表示.
void MyApp::DrawDataPlaying()
{
	// プレイヤーのHPバーを更新
	float PlayerMaxHP = 0;
	float PlayerHP = 0;
	PlayerMaxHP = pPlayer->GetMaxHP();
	PlayerHP = pPlayer->GetHP();
	PlayerHP = PlayerHP / PlayerMaxHP * 100;
	int Meter = pPlayer->GetMeter();

	switch (gameState)
	{
		// チュートリアル画面
	case GAME_STATE_TUTORIAL:

		Draw(pSprite, kanban64_Tex->GetTexture(), D3DXVECTOR3( 89, HEIGHT - 96 * SIZE, 0), RECT{   0, 0,  64, 64 }, 0.0f, 0.0f, false, false, false, 1);
		if (kanbanflg >= 1) {
			Draw(pSprite, kanban64_Tex->GetTexture(), D3DXVECTOR3(306, HEIGHT - 96 * SIZE, 0), RECT{ 64, 0, 128, 64 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, redmaru_Tex->GetTexture(), D3DXVECTOR3(89 + 48 * SIZE, HEIGHT - 112 * SIZE, 0), RECT{ 0, 0, 32, 32 }, 0.0f, 0.0f, false, false, false, 1);
		}
		if (kanbanflg >= 2) {
			Draw(pSprite, kanban64_Tex->GetTexture(), D3DXVECTOR3(523, HEIGHT - 96 * SIZE, 0), RECT{ 128, 0, 192, 64 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, redmaru_Tex->GetTexture(), D3DXVECTOR3(306 + 48 * SIZE, HEIGHT - (96 + 16) * SIZE, 0), RECT{ 0, 0, 32, 32 }, 0.0f, 0.0f, false, false, false, 1);
		}
		if (kanbanflg >= 3) {
			Draw(pSprite, kanban64_Tex->GetTexture(), D3DXVECTOR3(740, HEIGHT - 96 * SIZE, 0), RECT{ 192, 0, 256, 64 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, redmaru_Tex->GetTexture(), D3DXVECTOR3(523 + 48 * SIZE, HEIGHT - (96 + 16) * SIZE, 0), RECT{ 0, 0, 32, 32 }, 0.0f, 0.0f, false, false, false, 1);
		}
		if(kanbanflg >= 4)			Draw(pSprite, redmaru_Tex->GetTexture(), D3DXVECTOR3(740 + 48 * SIZE, HEIGHT - (96 + 16) * SIZE, 0), RECT{ 0, 0, 32, 32 }, 0.0f, 0.0f, false, false, false, 1);

		// すりぬけ床の描画.
		pFloorMgr->Show();

		// 敵の描画.
		pEnemyMgr->Show();

		// 弾丸の表示。配列を順番に探査して表示する.
		pPlayerBullet->Draw(pPlayerBltTex);

		// プレイヤーの描画.
		pPlayer->Show();

		// 爆発の描画
		pExplMgr->Draw(pExplosionTex);

		Draw(pSprite, PlayerHPBarFrame_Tex->GetTexture(), D3DXVECTOR3(0, HEIGHT - 24 * SIZE, 0), RECT{ 0, 0, 138, 24 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 35 * SIZE, HEIGHT - 21 * SIZE, 0), RECT{ 0, 0, 100, 18 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 35 * SIZE, HEIGHT - 21 * SIZE, 0), RECT{ 0, 18, (int)PlayerHP, 36 }, 0.0f, 0.0f, false, false, false, 1);

		// プレイヤーの必殺技ゲージの描画
		Draw(pSprite, PlayerMeterFrame_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, PlayerMeter_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, PlayerMeter_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 12, 16 * Meter, 24 }, 0.0f, 0.0f, false, false, false, 1);
		if (Meter == 5) {
			if (gameTimer % 50 >= 25)Draw(pSprite, PlayerMeterMetsu_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, PlayerMeterHatsu_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
		}

		break;
		// ゲーム画面
	case GAME_STATE_PLAYING:

		Draw(pSprite, earth_Tex->GetTexture(), D3DXVECTOR3(0, HEIGHT - 64, 0), RECT{ 0, 0,  960, 32 }, 0.0f, 0.0f, false, false, false, 1);
		// すりぬけ床の描画.
		pFloorMgr->Show();

		// 敵の描画.
		//pEnemyMgr->Show();

		// 弾丸の表示。配列を順番に探査して表示する.
		pPlayerBullet->Draw(pPlayerBltTex);
		pBossBullet->Draw(pEnemyBltTex);

		//ボス描画
		pBossMgr->Show();

		//DrawChara(pSprite, tmpBossTex->GetTexture(), D3DXVECTOR3(WIDTH - 200 - 32, 0 + 16, 0), RECT{ 0, 0, 64, 64 }, 0.0f, 0.0f, false, false);

		// プレイヤーの描画.
		pPlayer->Show();

		// 爆発の描画
		pExplMgr->Draw(pExplosionTex);

		{
			/*
			Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 36, HEIGHT - 36, 0), RECT{ 0, 0, 100, 18 }, 0.0f, 0.0f, false, false);
			Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 36, HEIGHT - 36, 0), RECT{ 0, 18, (int)PlayerHP, 36 }, 0.0f, 0.0f, false, false);
			*/
			Draw(pSprite, PlayerHPBarFrame_Tex->GetTexture(), D3DXVECTOR3(0, HEIGHT - 24 * SIZE, 0), RECT{ 0, 0, 138, 24 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 35 * SIZE, HEIGHT - 21 * SIZE, 0), RECT{ 0, 0, 100, 18 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 35 * SIZE, HEIGHT - 21 * SIZE, 0), RECT{ 0, 18, (int)PlayerHP, 36 }, 0.0f, 0.0f, false, false, false, 1);

			// プレイヤーの必殺技ゲージの描画
			Draw(pSprite, PlayerMeterFrame_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, PlayerMeter_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, PlayerMeter_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 12, 16 * Meter, 24 }, 0.0f, 0.0f, false, false, false, 1);
			if (Meter == 5) {
				if (gameTimer % 50 >= 25)Draw(pSprite, PlayerMeterMetsu_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
				Draw(pSprite, PlayerMeterHatsu_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
			}

			// ボスのHPバーを更新
			//int b = 0; // ポインタを使用したいため、変数を定義
			float BossMaxHP = 0;
			float BossHP = 0;
			for (int i = 0; i < pBossMgr->BossMax(); i++) {
				if (pBossMgr->PPBuffer(i)) {
					BossMaxHP = pBossMgr->GetMaxHP(&i);
					BossHP = pBossMgr->GetHP(&i);
					BossHP = BossHP / BossMaxHP * 200;
				}
			}
			/*
			Draw(pSprite, BossHPBarFrame_Tex->GetTexture(), D3DXVECTOR3(WIDTH - 238 - 33, 0 + 15, 0), RECT{ 0, 0, 238, 24 }, 0.0f, 0.0f, false, false);
			Draw(pSprite, BossHPBar_Tex->GetTexture(), D3DXVECTOR3(WIDTH - 200 - 36, 0 + 18, 0), RECT{ 0, 0, 200, 18 }, 0.0f, 0.0f, false, false);
			Draw(pSprite, BossHPBar_Tex->GetTexture(), D3DXVECTOR3(WIDTH - 200 - 36, 0 + 18, 0), RECT{ 200 - (int)BossHP, 18, 200, 36 }, 200.0f, 0.0f, true, false);
			*/
			Draw(pSprite, BossHPBarFrame_Tex->GetTexture(), D3DXVECTOR3(WIDTH - 238 * SIZE, 0, 0), RECT{ 0, 0, 238, 24 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, BossHPBar_Tex->GetTexture(), D3DXVECTOR3(WIDTH - 200 * SIZE - 3 * SIZE, 0 + 3 * SIZE, 0), RECT{ 0, 0, 200, 18 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, BossHPBar_Tex->GetTexture(), D3DXVECTOR3(WIDTH - 200 * SIZE - 3 * SIZE, 0 + 3 * SIZE, 0), RECT{ 200 - (int)BossHP, 18, 200, 36 }, 200.0f, 0.0f, true, false, false, 1);
		}
		break;
	default:
		break;
	}

}

// 当たり判定（円）
bool MyApp::Collision(D3DXVECTOR2 a, float a_r, D3DXVECTOR2 b, float b_r) {
	// 中心点の距離
	float lengthX = (a.x + a_r * SIZE) - (b.x + b_r * SIZE);

	float lengthY = (a.y + a_r * SIZE) - (b.y + b_r * SIZE);
	float centerDistance = sqrtf(lengthX * lengthX + lengthY * lengthY);

	//半径の和
	float radiusSum = a_r * SIZE + b_r * SIZE;

	//中心点の距離より半径の和のほうが大きい
	if (centerDistance <= radiusSum)
	{
		//二つの円が当たっている
		return true;
	}
	return false; // 当たっていない
}

// 描画関数
void MyApp::Draw(ID3DXSprite* pSprite, IDirect3DTexture9* pTexture,
	D3DXVECTOR3 pos, RECT rc, float textureWidth, float textureHeight, bool flipHorizontal, bool flipVertical, bool damageflg, float size) {
	D3DXVECTOR2 scaling(SIZE * size, SIZE * size); // スケール
	D3DXVECTOR2 position(pos);     // 描画位置
	D3DXVECTOR2 spriteCenter(0.0f, 0.0f); // 基準点（画像の左上）

	// 横反転
	if (flipHorizontal) {
		scaling.x = -SIZE * size;
		spriteCenter.x = textureWidth; // 横方向の基準を右端に
		position.x += textureWidth * SIZE * size;
	}

	// 縦反転
	if (flipVertical) {
		scaling.y = -SIZE * size;
		spriteCenter.y = textureHeight; // 縦方向の基準を下端に
		position.y += textureWidth * SIZE * size;
	}

	// 変換行列を設定
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scaling, &spriteCenter, 0.0f, &position);
	pSprite->SetTransform(&mat);

	// 描画
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if (damageflg)pSprite->Draw(pTexture, &rc, NULL, NULL, D3DCOLOR_ARGB(255, 255, 0, 0));
	else pSprite->Draw(pTexture, &rc, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	pSprite->End();

	// 変換行列をリセット
	D3DXMatrixIdentity(&mat);
	pSprite->SetTransform(&mat);
}

// すりぬけ床を生成.
void MyApp::CreateFloorSlip(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h)
{
	FloorSlip* pFloorSlip = new FloorSlip(pFloorSlipTex);
	pFloorSlip->Init(pos, v_pos, w, h);
	pFloorMgr->Add(pFloorSlip);
}

// 敵Ａを生成.
void MyApp::CreateEnemyA(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, int maxhp)
{
	EnemyAAA* pEnemyA = new EnemyAAA(pMatoTex);
	pEnemyA->Init(pos, v_pos,w, h,  maxhp);
	pEnemyMgr->Add(pEnemyA);
}

// ボスＡを生成.
void MyApp::CreateBossA(float x, float y, float vx, float vy, float w, int maxhp)
{
	BossAAA* pBossA = new BossAAA(pBossTexA);
	pBossA->Init(x, y, vx, vy, w, maxhp);
	pBossMgr->Add(pBossA);
}

// ボスＡを生成.
void MyApp::CreateBossB(float x, float y, float vx, float vy, float w, int maxhp)
{
	BossBBB* pBossB = new BossBBB(pBossTexB);
	pBossB->Init(x, y, vx, vy, w, maxhp);
    pBossMgr->Add(pBossB);
}