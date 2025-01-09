/*
	myApp.cpp

	�A�v���P�[�V�����̊j�ƂȂ�\�[�X
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

// ��ʑJ�ڂ����

//#define FULLSCREEN

// �E�B���h�E�^�C�g���o�[�ɕ\�������o�[�W������.
#define WDATE(x, y) _T(x y)
static const TCHAR* version = WDATE(__DATE__, __TIME__);

#define APP_NAME _T("GP3_STG")

// �֐��̃v���g�^�C�v�錾.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
const TCHAR* D3DErrStr(HRESULT res);

// static�����o�[�ϐ��̎��̂̒�`�̓\�[�X���imyApp.cpp�j�ɋL�q����.
MyApp MyApp::sm_myApp;

MyApp::MyApp()
	: hInstance(NULL)			// �C���X�^���X�n���h��.
	, hWnd(NULL)				// �\���E�B���h�E.
	, pD3D(NULL)				// Direct3D�C���X�^���X�I�u�W�F�N�g.
	, pDevice(NULL)				// Direct3D�f�o�C�X�I�u�W�F�N�g.
	, pSprite(NULL)				// �X�v���C�g.
	, pFont(NULL)				// �r�b�g�}�b�v�t�H���g.
	, pPlayerTex(NULL)			// ���@.
	, pPlayer_WeaponTex(NULL)	// �v���C���[�̕���
	, pMatoTex(NULL)			// �����蔻��`�F�b�N�p�̓I.
	, pFloorMgr(NULL)			// ���̔z��.
	, pFloorSlipTex(NULL)				// ����ʂ���.
	, pEffectTex(NULL)			// �U���G�t�F�N�g
	, pPlayerBullet(NULL)			// �v���C���[�̒e��.
	, pPlayerBltTex(NULL)		// �v���C���[�e�ۃe�N�X�`��.
	, pBossBullet(NULL)			// �{�X�̒e��.
	, pEnemyBltTex(NULL)		// �G�e�ۃe�N�X�`��.
	, pBossMgr(NULL)
	, pBossTexA(NULL)			// �{�XA�̃e�N�X�`��
	, pBossTexB(NULL)			// �{�XB�̃e�N�X�`��
	, PlayerMeter_Tex(NULL)
	, PlayerMeterFrame_Tex(NULL)
	, PlayerMeterHatsu_Tex(NULL)
	, PlayerMeterMetsu_Tex(NULL)
	, PlayerHPBar_Tex(NULL)
	, PlayerHPBarFrame_Tex(NULL)
	, BossHPBar_Tex(NULL)
	, BossHPBarFrame_Tex(NULL)
	, nextFireFrame(0)			// ���@�����ɒe�����Ă�܂ł̎��ԁi�t���[���j.
	, buttons(0)				// �{�^���̏��.
	, pPlayer(NULL)
	, pEnemyMgr(NULL)			// �G�̔z��.
	, pEnemyTexA(NULL)			// �GA�̃e�N�X�`��.
	, pEnemyTexB(NULL)			// �GB�̃e�N�X�`��.
	, pEnemyTexC(NULL)			// �GC�̃e�N�X�`��.
	, pBackGroundTex(NULL)      // �w�i.
	, BackGroundY(0)            // �w�i�̌��݈ʒu.
	, score(0)					// �X�R�A.
	, playerPower(0)			// ���@�̑ϋv��.
	, pExplMgr(NULL)		    // �����̔z��.
	, pExplosionTex(NULL)
	, joyID(-1)					// �Q�[���R���g���[����ID.
	, gameTimer(-1)				// �S�̂̃^�C�}�[.
	, gameState(GAME_STATE_LOAD)// �Q�[���S�̂̏��.
	, stateTimer(0)				// ��ԑJ�ڗp�^�C�}�[.
	, pReadyTex(NULL)			// READY.
	, pStage1Tex(NULL)			// STAGE1.
	, pGameOverTex(NULL)		// GAMEOVER.
{
}

bool MyApp::InitApp()
{
	// ���̃v���O���������s�����Ƃ��̃C���X�^���X�n���h�����擾.
	hInstance = GetModuleHandle(NULL);

	// �܂��E�B���h�E�N���X��o�^����.
	// ����̓E�B���h�E������̏����̎d����Windows�ɋ����邽�߂ł���.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));		// �ϐ�wc���[���N���A����.
	wc.cbSize = sizeof(WNDCLASSEX);				// ���̍\���̂̑傫����^����.
	wc.lpfnWndProc = (WNDPROC)WindowProc;		// �E�B���h�E�v���V�[�W���o�^.
	wc.hInstance = hInstance;					// �C���X�^���X�n���h����ݒ�.
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);	// �}�E�X�J�[�\���̓o�^.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);// �w�i��GRAY��.
	wc.lpszClassName = APP_NAME;				// �N���X���ACreateWindow�ƈ�v������.
	RegisterClassEx(&wc);						// �o�^.

	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_tprintf(_T("rc: %d, %d -- %d, %d\n"), rc.left, rc.top, rc.right, rc.bottom);

	// �E�B���h�E���쐬����.
	hWnd = CreateWindow(APP_NAME, version, WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	assert(hWnd != NULL);

	// Direct3D������������.
	HRESULT hr = InitDirect3D();
	if (FAILED(hr)) {
		MessageBox(NULL, D3DErrStr(hr), _T("Direct3D���������s"), MB_OK);
		return false;
	}

	// �X�v���C�g�̍쐬.
	hr = D3DXCreateSprite(pDevice, &(pSprite));
	if (FAILED(hr)) {
		MessageBox(NULL, D3DErrStr(hr), _T("�X�v���C�g�쐬���s"), MB_OK);
		return false;
	}

	// DirectSound�̏�����.
	hr = GetSoundInst()->InitSound(hWnd);
	if (FAILED(hr)) {
		MessageBox(NULL, D3DErrStr(hr), _T("�_�C���N�g�T�E���h�쐬���s"), MB_OK);
		return false;
	}

	// ���̓f�o�C�X�̏�����.
	GetInputInst()->InitInput(hInstance, hWnd);
	if (GetInputInst()->GetJoyCnt() == 0) {
		MessageBox(NULL, _T("�W���C�X�e�B�b�N���q�����Ă��܂���\r\n�L�[�{�[�h�ő��삵�܂�"), _T("JOYSTICK"), MB_OK);
	}

	// ���Ǘ��N���X�̃I�u�W�F�N�g�𐶐�.
	pFloorMgr = new FloorManager(10);
	if (pFloorMgr == NULL || !pFloorMgr->Init()) {
		return false;
	}

	// �G�Ǘ��N���X�̃I�u�W�F�N�g�𐶐�.
	pEnemyMgr = new EnemyManager(70);
	if (pEnemyMgr == NULL || !pEnemyMgr->Init()) {
		return false;
	}

	// �{�X�Ǘ��N���X�̃I�u�W�F�N�g�𐶐�.
	pBossMgr = new BossManager(10);
	if (pBossMgr == NULL || !pBossMgr->Init()) {
		return false;
	}

	// �e�ۊǗ��N���X�i�v���C���[�p�j�̃I�u�W�F�N�g�𐶐�&������.
	pPlayerBullet = new BulletBuffer(1);
	if (pPlayerBullet == NULL || !pPlayerBullet->Init()) {
		return false;
	}

	// �e�ۊǗ��N���X�i�G�p�j�̃I�u�W�F�N�g�𐶐�&������.
	pBossBullet = new BulletBuffer(100);
	if (pBossBullet == NULL || !pBossBullet->Init()) {
		return false;
	}

	// �����������ő�100���̃o�b�t�@������.
	pExplMgr = new ExplosionBuffer(100);
	if (pExplMgr == NULL || !pExplMgr->Init()) {
		return false;
	}

	// �e�N�X�`����ǂݍ��ށB�G���[���ɂ�LoadTexture�͗�O�𔭐�������.
	// LoadTexture���Ƃ�if���������͖̂ʓ|�Ȃ̂ŁAtry�`throw�`catch���g��.
	try {
		pFont = MyBitmapFont::LoadFont(pDevice, _T("data/image/fontBitmap.png"));

		pPlayerTex = MyTexture::LoadTexture(pDevice, _T("data/image/player_kari.png"));
		pPlayer_WeaponTex = MyTexture::LoadTexture(pDevice, _T("data/image/player_weapon.png"));
		pPlayerBltTex = MyTexture::LoadTexture(pDevice, _T("data/image/playerbullet.png"));
		pEnemyBltTex = MyTexture::LoadTexture(pDevice, _T("data/image/enemybullet.png"));
		pBossTexA = MyTexture::LoadTexture(pDevice, _T("data/image/bossTexture_0.png"));
		pBossTexB = MyTexture::LoadTexture(pDevice, _T("data/image/bossTexture_1.png"));
		pMatoTex = MyTexture::LoadTexture(pDevice, _T("data/image/mato.png"));
		pExplosionTex = MyTexture::LoadTexture(pDevice, _T("data/image/effect.png"));

		PlayerMeter_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/playerMeter.png"));
		PlayerMeterFrame_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/playerMeterframe.png"));
		PlayerMeterHatsu_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/PlayerMeterHatsudou.png"));
		PlayerMeterMetsu_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/PlayerMeterTenmetsu.png"));
		PlayerHPBar_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/playerHPbar.png"));
		PlayerHPBarFrame_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/playerHPbarframe.png"));
		BossHPBar_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/bossHPbar.png"));
		BossHPBarFrame_Tex = MyTexture::LoadTexture(pDevice, _T("data/image/bossHPbarframe.png"));

		pFloorSlipTex = MyTexture::LoadTexture(pDevice, _T("data/image/floorSlip.png"));

		// ���l�\���p�e�N�X�`���̓ǂݍ���.
		MyTexture* pTex = MyTexture::LoadTexture(pDevice, _T("data/image/numbers.png"));
		// �X�R�A�\���Ɏg���e�N�X�`���̏���ݒ�.
		//pScoreDisp->Init(pTex, 64, 64);// 64:�����P���̃e�N�X�`���T�C�Y.

		// SE/BGM�̓ǂݍ���.
		GetSoundInst()->LoadSoundData();
	}
	// catch���try�̒���ɋL�q����.
	catch (HRESULT /*hr*/) {
		return false;
	}

	return true;
}

// ���C�����[�v.
void MyApp::MainLoop()
{
	// BGM���Đ�.
	//GetSoundInst()->PlayBGMs(BGM_0);

	ShowWindow(hWnd, SW_SHOWNORMAL);	// �쐬�����E�B���h�E��\������.
	timer.StartTimer();
	fps.StartTimer();

	// �C�x���g���[�v.
	// �u���b�N�^�֐�GetMessage�ł͂Ȃ��m���u���b�N�^�֐���PeekMessage���g��.
	MSG msg;
	bool flag = true;
	while (flag) {
		// ���b�Z�[�W�����邩�ǂ����m�F����.
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// ���b�Z�[�W������̂ŏ�������.
			if (GetMessage(&msg, NULL, 0, 0) != 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				flag = 0;
			}
		}
		else {
			if (UpdateData()) {	// �ʒu�̍Čv�Z.
				// �a�f�l�̃t�F�[�h�C���^�A�E�g�̂��߂̃{�����[���X�V.
				GetSoundInst()->UpdateAllBGM();
				DrawData();		// �`��.
			}
		}
		// Sleep�ȂǍs���Ă��Ȃ����ADrawData�֐�����pDevice->Present��.
		// ��ʍX�V�҂����s���̂ŁA���ʓI��VSync���ƂɃC�x���g���[�v�����.
	}
}

// �}�N���𕡐��s�ŏ��������Ƃ���\���s���ɕt���邱�Ƃŉ\.
#define RELEASE(__xx__)    \
	if (__xx__)            \
	{                      \
		__xx__->Release(); \
		__xx__ = NULL;     \
	}

// DataSet���������.
void MyApp::ReleaseData()
{
	// �T�E���h���.
	GetSoundInst()->Release();

	// �e�N�X�`���̊J��.
	MyTexture::ReleaseAll();

	// ���I�I�u�W�F�N�g�̊J��.
	if (pExplMgr) { delete pExplMgr; }
	if (pFloorMgr) { delete pFloorMgr; }
	if (pEnemyMgr) { delete pEnemyMgr; }
	if (pBossMgr) { delete pBossMgr; }
	if (pPlayerBullet) { delete pPlayerBullet; }
	if (pBossBullet) { delete pBossBullet; }
	if (pFont) { delete pFont; }

	// ���̓f�o�C�X�̉��.
	GetInputInst()->Release();

	// DirectX�f�o�C�X�̊J��.
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

	// �v���C���[�N���X�̃I�u�W�F�N�g�𐶐�
	pPlayer = new Player(pPlayerTex, pPlayer_WeaponTex);
	pPlayer->Init(D3DXVECTOR2(WIDTH / 2, HEIGHT - 64 * SIZE), D3DXVECTOR2(0.0f, -10.0f), 32.0f, 32.0f, 48.0f, 48.0f, 50);

	//CreateFloorSlip(D3DXVECTOR2(500, HEIGHT - 100 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 160, 32);

	CreateFloorSlip(D3DXVECTOR2(672, HEIGHT - 144 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 160, 32);

	CreateFloorSlip(D3DXVECTOR2(128, HEIGHT - 144 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 160, 32);

	//CreateEnemyA(D3DXVECTOR2(300, HEIGHT - 80 * SIZE), D3DXVECTOR2(0.0f, 0.0f), 48, 48, 30);
	//CreateEnemyA(200, HEIGHT - 66, 0.0f, 0.0f, 5);
	//CreateEnemyA(600, HEIGHT - 66, 1.0f, 0.0f, 3);

	//�{�XA�̏�����
	//CreateBossA(800.0f, HEIGHT - 128.0f * SIZE, 0.0f, 0.0f, 64.0f, 5);

	//�{�XB�̏�����
	CreateBossB(800.0f, HEIGHT - 128.0f * SIZE, 0.0f, 0.0f, 64.0f, 26);

	playerPower = 10;
	nextFireFrame = 0;
	stateTimer = 0;
}

// Direct3D������������.
// return ���������G���[�܂���D3D_OK.
HRESULT MyApp::InitDirect3D()
{
	// Direct3D�C���X�^���X�I�u�W�F�N�g�𐶐�����.
	// D3D_SDK_VERSION�ƁA�����^�C���o�[�W�����ԍ����K�؂łȂ��ƁANULL���Ԃ�.
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	// NULL�Ȃ烉���^�C�����s�K��.
	if (pD3D == NULL) return E_FAIL;

	// PRESENT�p�����[�^���[���N���A���A�K�؂ɏ�����.
	D3DPRESENT_PARAMETERS d3dpp;	// �f�o�C�X�쐬���̃p�����[�^.
	ZeroMemory(&(d3dpp), sizeof(d3dpp));
#ifdef FULLSCREEN
	d3dpp.Windowed = FALSE;
	//int fx = GetSystemMetrics(SM_CXSCREEN);
	//int fy = GetSystemMetrics(SM_CYSCREEN);
	int fx = WIDTH;
	int fy = HEIGHT;
	_tprintf(_T("�t���X�N���[�� %d x %d\n"), fx, fy);
	d3dpp.BackBufferHeight = fy;
	d3dpp.BackBufferWidth = fx;
#else
	d3dpp.Windowed = TRUE;
#endif
	d3dpp.BackBufferHeight = HEIGHT;
	d3dpp.BackBufferWidth = WIDTH;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// �o�b�N�o�b�t�@��RGB���ꂼ��W�r�b�g��.
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	// Present���ɐ��������ɍ��킹��.
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BHAL&HARD.
	HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("HAL & HARD\n"));
		return hr;
	}
	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BHAL&SOFT.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("HAL & SOFT\n"));
		return hr;
	}
	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BREF&HARD.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("REF & HARD\n"));
		return hr;
	}
	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BREF&SOFT.
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(d3dpp), &(pDevice));
	if (hr == D3D_OK)
	{
		_tprintf(_T("REF & SOFT\n"));
		return hr;
	}
	return hr;
}

// �f�[�^���A�b�v�f�[�g����.
BOOL MyApp::UpdateData()
{
	LONGLONG us; UINT frames;
	timer.GetPassTime(&us, &frames);
	if (us < 1000000 / 60) {
		return FALSE;// 60FPS�ōX�V.
	}
	timer.StartTimer();

	gameTimer++;
	stateTimer++;
	buttons = 0;

	// ���͏����X�V.
	MyInput* pInput = GetInputInst();
	pInput->UpdateInput(hWnd);

	// �Q�[����Ԃɉ����������ɕύX.
	switch (gameState)
	{
	case GAME_STATE_LOAD:
	{
		ResetGameData();

		// ���̏�Ԃ�������Ԃɂ܂����Ă���.
		GameState eNextState = GAME_STATE_TITLE;

		// ��Ԃ�J��.
		gameState = eNextState;
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

		// A�{�^��(SPACE-Key)�����������ꂽ��Q�[���J�n.
		if (buttons == JOY_BTN_BIT_A)
		{
			gameState = GAME_STATE_TUTORIAL;
			stateTimer = 0;
			gameTimer = 0;
		}
		break;
	}

	case GAME_STATE_READY:
	{
		if (pInput->IsPushBtn(JOY_CON_0, JOY_BTN_BIT_B)) {
			buttons |= JOY_BTN_BIT_B;// 
		}
		if (pInput->IsPushKey(DIK_LCONTROL)) {
			buttons |= JOY_BTN_BIT_B;// 
		}

		// B�{�^��(CTRL-Key)�����������ꂽ��Q�[���J�n.
		if (buttons == JOY_BTN_BIT_B)
		{
			GetSoundInst()->PlaySE(SE_START);// �X�^�[�g����炷.
			gameState = GAME_STATE_PLAYING;
			stateTimer = 0;
			gameTimer = 0;
		}
		break;
	}

	case GAME_STATE_TUTORIAL:
	{
		if (pInput->IsPushKey(DIK_W)) {
			buttons |= JOY_BTN_BIT_A;// 
		}

		// W-Key�������ꂽ��Q�[����ʂ�.
		if (buttons == JOY_BTN_BIT_A)
		{
			gameState = GAME_STATE_PLAYING;
		}

		break;
	}

	case GAME_STATE_PLAYING:
	{
		UpdatePlaying(true);

		// �v���C���[�̃p���[���[���ɂȂ�����A�Q�[���I�[�o�[.
		if (playerPower <= 0)
		{
			GetSoundInst()->StopAllBGM();
			GetSoundInst()->PlaySE(SE_DEAD);
			gameState = GAME_STATE_DEAD;
			stateTimer = 0;
		}
		
		/* �f�o�b�O�p
		if (pInput->IsPushKey(DIK_Q)) {
			buttons |= JOY_BTN_BIT_A;// 
		}

		//	Q-Key�������ꂽ��N���A��ʂ�.
		if (buttons == JOY_BTN_BIT_A)
		{
			gameState = GAME_STATE_CLEAR;
		}
		*/

		break;
	}

	case GAME_STATE_DEAD:
	{
		UpdatePlaying(false);
		if (stateTimer > 300)
		{
			ResetGameData();
			gameState = GAME_STATE_READY;
			stateTimer = 0;
			//pStage->Rewind();
		}
		break;
	}

	case GAME_STATE_CLEAR:
	{
		if (pInput->IsPushKey(DIK_BACK)) {
			buttons |= JOY_BTN_BIT_A;// 
		}

		// BACKSPACE-Key�����������ꂽ��^�C�g���ɖ߂�.
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

// GAME_PLAYING��Ԃ̎���Update
// playable��true�ł���Ύ��@����
void MyApp::UpdatePlaying(bool playable)
{
	if (playable) {
		MyInput* pInput = GetInputInst();

		// �W���C�X�e�B�b�N�ł̑���.
		if (pInput->GetJoyCnt() > 0) {
			// ���X�e�b�N�ňړ�.
			int jx = pInput->GetStickLX(JOY_CON_0);
			int jy = pInput->GetStickLY(JOY_CON_0);

			// �����т��z�����邽�߁A���l���傫����(��������)�𔻒肷��.
			if (abs(jx) > 10) {
				//player_pos.x += jx / 50;
			}
			if (abs(jy) > 10) {
				//player_pos.y += jy / 50;
			}

			if (pInput->IsPushBtn(JOY_CON_0, JOY_BTN_BIT_A)) {
				buttons |= JOY_BTN_BIT_A;// �`�{�^���Œe�۔���.
			}
		}

		// �L�[�{�[�h�ł̑���.
		// �v���C���[���X�V�A���������蔲���΍�̂��ߎ���������.
		pPlayer->Update(1.0f / 10);

		// �v���C���[�̍U�����I���������Ƃ����m���A�G�ƃ{�X�̔�_���[�W�t���O��false�ɂ���
		if (pPlayer->AttackEnd()) {
			for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
				if (pEnemyMgr->PPBuffer(i))pEnemyMgr->DamageFlg(false, i);
			}
			for (int i = 0; i < pBossMgr->BossMax(); i++) {
				if (pBossMgr->PPBuffer(i))pBossMgr->DamageReset(0x2 , &i);
			}
		}

		if (!pPlayerBullet->Next() == NULL) {
			for (int i = 0; i < pBossMgr->BossMax(); i++) {
				if (pBossMgr->PPBuffer(i))pBossMgr->DamageReset(0x1, &i);
			}
		}

		bool bossflg = false; // �{�X��|�������ǂ���
		for (int i = 0; i < pBossMgr->BossMax(); i++) {
			if (pBossMgr->PPBuffer(i)) {
				bossflg = true;
			}
		}
		if (!bossflg) gameState = GAME_STATE_CLEAR;

		// ����ʂ������X�V�A���������蔲���΍�̂��ߎ���������.
		pFloorMgr->Update(1.0f / 10);

		// �G���X�V�A���������蔲���΍�̂��ߎ���������.
		pEnemyMgr->Update(1.0f / 10);

		// �e�ۂƃ{�X���X�V�A���������蔲���΍�̂��ߎ���������.
		for (int i = 0; i < 10; i++)
		{
			pPlayerBullet->Update(1.0f / 10);
			pBossBullet->Update(1.0f / 10);
		}
		pBossMgr->Update(1.0f / 10);

		// �����ɂ͓����蔻��͂Ȃ��̂ŁA�������͕s�v.
		pExplMgr->Update(1);

		//Debug
		//Fire
		if (pInput->IsPushKey(DIK_X))
		{
			//_tprintf(_T("x�{�^����������܂���\n"));
			pBossMgr->ChangeAction(1, 0);
		}
		//Jump
		if (pInput->IsPushKey(DIK_Z))
		{
			//_tprintf(_T("x�{�^����������܂���\n"));
			pBossMgr->ChangeAction(2, 0);
		}

		// �G�ƕ���̓����蔻��i������o���Ă���Œ��Ȃ�΁j
		for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
			if (pEnemyMgr->PPBuffer(i)) {
				if (pPlayer->IsWeapon() && Collision(pPlayer->GetWeaponXY(), pPlayer->GetWeaponW() / 2, pEnemyMgr->GetXY(&i), 48.0f / 2) && !pEnemyMgr->IsDamage(i)) { // ����̍��W�A����̔��a�A�G�̍��W�A�G�̔��a
					pEnemyMgr->DamageFlg(true, i);
					// ���S�_�̋��� 
					float lengthX = (pPlayer->GetWeaponXY().x + pEnemyMgr->GetXY(&i).x) / 2;

					float lengthY = (pPlayer->GetWeaponXY().y + pEnemyMgr->GetXY(&i).y) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					pEnemyMgr->Damage(1, i);
					GetPlayer()->PlusMeter(1);
				}
			}
		}

		// �G�ƃv���C���[�̓����蔻��
		// �����蔻�肪���������Ȃ��悤�ɁA�G�̓����蔻��������ڂ�2/3�ɂ���
		for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
			if (pEnemyMgr->PPBuffer(i)) {
				if (Collision(pPlayer->GetXY(), pPlayer->GetSize() / 2, D3DXVECTOR2(pEnemyMgr->GetXY(&i).x + 32.0f / 2, pEnemyMgr->GetXY(&i).y + 32.0f / 2), 32.0f / 2) && !pPlayer->IsDamage()) { // ����̍��W�A����̔��a�A�G�̍��W�A�G�̔��a
					pPlayer->DamageFlg(true);
					// ���S�_�̋��� 
					float lengthX = (pPlayer->GetXY().x + (pEnemyMgr->GetXY(&i).x) + 32.0f / 2) / 2;

					float lengthY = (pPlayer->GetXY().y + (pEnemyMgr->GetXY(&i).y) + 32.0f / 2) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					bool direct = false;
					if (lengthX > pPlayer->GetXY().x - pPlayer->GetSize() / 4) direct = true;
					pPlayer->Damage(1, direct);
				}
			}
		}

		// �{�X�ƃv���C���[�̕���̓����蔻��i������o���Ă���Œ��Ȃ�΁j
		for (int i = 0; i < pBossMgr->BossMax(); i++) {
			if (pBossMgr->PPBuffer(i)) {
				if (pPlayer->IsWeapon() && Collision(pPlayer->GetWeaponXY(), pPlayer->GetWeaponW() / 2, pBossMgr->GetXY(&i), pBossMgr->GetSize(&i) / 2) && (pBossMgr->IsDamage(&i) & 0x1) == 0) { // ����̍��W�A����̔��a�A�{�X�̍��W�A�{�X�̔��a
					pBossMgr->DamageFlg(0x1, &i);
					// ���S�_�̋��� 
					float lengthX = (pPlayer->GetWeaponXY().x + pBossMgr->GetXY(&i).x) / 2;

					float lengthY = (pPlayer->GetWeaponXY().y + pBossMgr->GetXY(&i).y) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					pBossMgr->Damage(1, i);
					GetPlayer()->PlusMeter(1);
				}
			}
		}

		// �{�X�ƃv���C���[�̓����蔻��
		// �����蔻�肪���������Ȃ��悤�ɁA�{�X�̓����蔻��������ڂ�2/3�ɂ���
		for (int i = 0; i < pBossMgr->BossMax(); i++) {
			if (pBossMgr->PPBuffer(i)) {
				if (Collision(pPlayer->GetXY(), pPlayer->GetSize() / 2, D3DXVECTOR2(pBossMgr->GetXY(&i).x + pBossMgr->GetSize(&i) / 3 * 2 / 2, pBossMgr->GetXY(&i).y + pBossMgr->GetSize(&i) / 3 * 2 / 2), pBossMgr->GetSize(&i) / 3 * 2 / 2) && !pPlayer->IsDamage()) { // ����̍��W�A����̔��a�A�G�̍��W�A�G�̔��a
					pPlayer->DamageFlg(true);
					// ���S�_�̋��� 
					float lengthX = (pPlayer->GetXY().x + (pBossMgr->GetXY(&i).x) + pBossMgr->GetSize(&i) / 3 * 2 / 2) / 2;

					float lengthY = (pPlayer->GetXY().y + (pBossMgr->GetXY(&i).y) + pBossMgr->GetSize(&i) / 3 * 2 / 2) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					bool direct = false;
					if (lengthX > pPlayer->GetXY().x - pPlayer->GetSize() / 4) direct = true;
					pPlayer->Damage(1, direct);
				}
			}
		}

		D3DXVECTOR2 pos = { 0, 0 };
		float size = 0;
		// �{�X�ƃv���C���[�̒e�ۂ̓����蔻��
		for (int i = 0; i < pBossMgr->BossMax(); i++) {
			if (pBossMgr->PPBuffer(i)) {
				for (int j = 0; j < pPlayerBullet->BulletMax(); j++) {
					if (pPlayerBullet->PPBuffer(j)) {
						pPlayerBullet->GetXY(&j, pos.x, pos.y);
						pPlayerBullet->GetSize(&j, size);
						if (Collision(pos, size / 2, pBossMgr->GetXY(&i), pBossMgr->GetSize(&i) / 2) && (pBossMgr->IsDamage(&i) & 0x2) == 0) { // ����̍��W�A����̔��a�A�{�X�̍��W�A�{�X�̔��a
							pBossMgr->DamageFlg(0x2, &i);
							// ���S�_�̋��� 
							float lengthX = (pos.x + pBossMgr->GetXY(&i).x) / 2;

							float lengthY = (pos.y + pBossMgr->GetXY(&i).y) / 2;

							pExplMgr->Explose(lengthX, lengthY);
							pBossMgr->Damage(20, i);
							GetPlayer()->PlusMeter(0);
						}
					}
				}
			}
		}
	}
}

// DataSet�Ɋ�Â��āA�X�v���C�g��`��.
// return ���������G���[.
HRESULT MyApp::DrawData()
{
	D3DXVECTOR3 cnt(0, 0, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	D3DCOLOR rgb = D3DCOLOR_XRGB(128, 128, 128);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // ��ʑS�̂�����.

	// �Q�[���̏�Ԃɂ��`�敪��.
	switch (gameState)
	{
	case GAME_STATE_LOAD:
	{
	}
	break;

	case GAME_STATE_TITLE:
	{
		rgb = D3DCOLOR_XRGB(128, 160, 128);
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // ��ʑS�̂�����.
		cnt.x = 128; cnt.y = 32;
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2 - 64;
		//pSprite->Draw(pTitleTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2 + 64;
		if (stateTimer % 30 < 20)
		{
			//pSprite->Draw(pPushSpaceTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		}

		fps.StartTimer();

		sprintf_s(text, "title");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2, 8, 0xFFFFFFFF);

		sprintf_s(text, "press space");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2 + 128, 8, 0xFFFFFFFF);
	}
	break;

	case GAME_STATE_READY:
	{
		cnt.x = 128; cnt.y = 32;
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2;
		if (stateTimer % 30 < 20)
		{
			pSprite->Draw(pReadyTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		}
	}
	break;

	case GAME_STATE_TUTORIAL:
	{
		rgb = D3DCOLOR_XRGB(128, 128, 160);
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // ��ʑS�̂�����.
		sprintf_s(text, "tutorial...");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2, 8, 0xFFFFFFFF);

		sprintf_s(text, "press W-key");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2 + 128, 8, 0xFFFFFFFF);
	}
	break;

	case GAME_STATE_PLAYING:
	{
		// �摜�̕\��.
		DrawDataPlaying();

		/* �f�o�b�O�p
		sprintf_s(text, "game playing...");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2, 8, 0xFFFFFFFF);

		sprintf_s(text, "press Q-key");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2 + 128, 8, 0xFFFFFFFF);
		*/
	}
	break;

	case GAME_STATE_DEAD:
	{
		// �G�ƒe�ۂ̕\��.
		//DrawDataPlaying();
	}
	break;

	case GAME_STATE_CLEAR:
	{
		//fps.StartTimer();

		rgb = D3DCOLOR_XRGB(160, 128, 128);
		pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0); // ��ʑS�̂�����.
		sprintf_s(text, "game clear!!!");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2, 8, 0xFFFFFFFF);

		sprintf_s(text, "press backspace");
		pFont->DrawBmpText(text, WIDTH / 2, HEIGHT / 2 + 128, 8, 0xFFFFFFFF);
	}
	break;
	}

	// �`����J�n�i�V�[���`��̊J�n�j.
	pDevice->BeginScene();

	// �X�v���C�g�̕`����J�n.
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	/******************
	* 
	* ��ɕ\��������
	* 
	******************/

	// �X�v���C�g�̕`����I���A�V�[���̕`����I��.
	pSprite->End();

	// �f�o�b�O�p�̏��\��.
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

	fps.StartTimer();

	//sprintf_s(txt, "explosion:%s", pBossMgr->GetText());
	//pFont->DrawBmpText(txt, 20, 400, 8, 0xFFFFFFFF);

	//sprintf_s(txt, "player:%s", pPlayer->GetXY().x);
	//pFont->DrawBmpText(txt, 20, 500, 8, 0xFFFFFFFF);
#endif

	pDevice->EndScene();

	// ���ۂɉ�ʂɕ\���B�o�b�N�o�b�t�@����t�����g�o�b�t�@�ւ̓]��.
	// �f�o�C�X�������̃t���O�w��ɂ��A������VSYNC��҂�.
	pDevice->Present(NULL, NULL, NULL, NULL);

	return D3D_OK;
}

// �摜�̕\��.
void MyApp::DrawDataPlaying()
{

	// ����ʂ����̕`��.
	pFloorMgr->Show();

	// �G�̕`��.
	pEnemyMgr->Show();

	// �e�ۂ̕\���B�z������ԂɒT�����ĕ\������.
	pPlayerBullet->Draw(pPlayerBltTex);
	pBossBullet->Draw(pEnemyBltTex);

	//�{�X�`��
	pBossMgr->Show();

	//DrawChara(pSprite, tmpBossTex->GetTexture(), D3DXVECTOR3(WIDTH - 200 - 32, 0 + 16, 0), RECT{ 0, 0, 64, 64 }, 0.0f, 0.0f, false, false);

	// �v���C���[�̕`��.
	pPlayer->Show();

	// �����̕`��
	pExplMgr->Draw(pExplosionTex);

	{
		// �v���C���[��HP�o�[���X�V
		float PlayerMaxHP = 0;
		float PlayerHP = 0;
		PlayerMaxHP = pPlayer->GetMaxHP();
		PlayerHP = pPlayer->GetHP();
		PlayerHP = PlayerHP / PlayerMaxHP * 100;
		int Meter = pPlayer->GetMeter();
		/*
		Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 36, HEIGHT - 36, 0), RECT{ 0, 0, 100, 18 }, 0.0f, 0.0f, false, false);
		Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 36, HEIGHT - 36, 0), RECT{ 0, 18, (int)PlayerHP, 36 }, 0.0f, 0.0f, false, false);
		*/
		Draw(pSprite, PlayerHPBarFrame_Tex->GetTexture(), D3DXVECTOR3(0, HEIGHT - 24 * SIZE, 0), RECT{ 0, 0, 138, 24 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 35 * SIZE, HEIGHT - 21 * SIZE, 0), RECT{ 0, 0, 100, 18 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, PlayerHPBar_Tex->GetTexture(), D3DXVECTOR3(0 + 35 * SIZE, HEIGHT - 21 * SIZE, 0), RECT{ 0, 18, (int)PlayerHP, 36 }, 0.0f, 0.0f, false, false, false, 1);

		// �v���C���[�̕K�E�Z�Q�[�W�̕`��
		Draw(pSprite, PlayerMeterFrame_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, PlayerMeter_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
		Draw(pSprite, PlayerMeter_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 12, 16 * Meter, 24 }, 0.0f, 0.0f, false, false, false, 1);
		if (Meter == 5) {
			if(gameTimer % 50 >= 25)Draw(pSprite, PlayerMeterMetsu_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
			Draw(pSprite, PlayerMeterHatsu_Tex->GetTexture(), D3DXVECTOR3(138 * SIZE, HEIGHT - 12 * SIZE, 0), RECT{ 0, 0, 81, 12 }, 0.0f, 0.0f, false, false, false, 1);
		}

		// �{�X��HP�o�[���X�V
		//int b = 0; // �|�C���^���g�p���������߁A�ϐ����`
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
}

// �����蔻��i�~�j
bool MyApp::Collision(D3DXVECTOR2 a, float a_r, D3DXVECTOR2 b, float b_r) {
	// ���S�_�̋���
	float lengthX = (a.x + a_r * SIZE) - (b.x + b_r * SIZE);

	float lengthY = (a.y + a_r * SIZE) - (b.y + b_r * SIZE);
	float centerDistance = sqrtf(lengthX * lengthX + lengthY * lengthY);

	//���a�̘a
	float radiusSum = a_r * SIZE + b_r * SIZE;

	//���S�_�̋�����蔼�a�̘a�̂ق����傫��
	if (centerDistance <= radiusSum)
	{
		//��̉~���������Ă���
		return true;
	}
	return false; // �������Ă��Ȃ�
}

// �`��֐�
void MyApp::Draw(ID3DXSprite* pSprite, IDirect3DTexture9* pTexture,
	D3DXVECTOR3 pos, RECT rc, float textureWidth, float textureHeight, bool flipHorizontal, bool flipVertical, bool damageflg, float size) {
	D3DXVECTOR2 scaling(SIZE * size, SIZE * size); // �X�P�[��
	D3DXVECTOR2 position(pos);     // �`��ʒu
	D3DXVECTOR2 spriteCenter(0.0f, 0.0f); // ��_�i�摜�̍���j

	// �����]
	if (flipHorizontal) {
		scaling.x = -SIZE * size;
		spriteCenter.x = textureWidth; // �������̊���E�[��
		position.x += textureWidth * SIZE * size;
	}

	// �c���]
	if (flipVertical) {
		scaling.y = -SIZE * size;
		spriteCenter.y = textureHeight; // �c�����̊�����[��
		position.y += textureWidth * SIZE * size;
	}

	// �ϊ��s���ݒ�
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scaling, &spriteCenter, 0.0f, &position);
	pSprite->SetTransform(&mat);

	// �`��
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	if (damageflg)pSprite->Draw(pTexture, &rc, NULL, NULL, D3DCOLOR_ARGB(255, 255, 0, 0));
	else pSprite->Draw(pTexture, &rc, NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	pSprite->End();

	// �ϊ��s������Z�b�g
	D3DXMatrixIdentity(&mat);
	pSprite->SetTransform(&mat);
}

// ����ʂ����𐶐�.
void MyApp::CreateFloorSlip(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h)
{
	FloorSlip* pFloorSlip = new FloorSlip(pFloorSlipTex);
	pFloorSlip->Init(pos, v_pos, w, h);
	pFloorMgr->Add(pFloorSlip);
}

// �G�`�𐶐�.
void MyApp::CreateEnemyA(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, int maxhp)
{
	EnemyAAA* pEnemyA = new EnemyAAA(pMatoTex);
	pEnemyA->Init(pos, v_pos,w, h,  maxhp);
	pEnemyMgr->Add(pEnemyA);
}

// �{�X�`�𐶐�.
void MyApp::CreateBossA(float x, float y, float vx, float vy, float w, int maxhp)
{
	BossAAA* pBossA = new BossAAA(pBossTexA);
	pBossA->Init(x, y, vx, vy, w, maxhp);
	pBossMgr->Add(pBossA);
}

// �{�X�`�𐶐�.
void MyApp::CreateBossB(float x, float y, float vx, float vy, float w, int maxhp)
{
	BossBBB* pBossB = new BossBBB(pBossTexB);
	pBossB->Init(x, y, vx, vy, w, maxhp);
    pBossMgr->Add(pBossB);
}