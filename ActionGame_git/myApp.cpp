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
#include "myEnemyAAA.h"

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
	, pEffectTex(NULL)			// �U���G�t�F�N�g
	, pBulletTex(NULL)			// ���@�e��.
	, BossHPBar_BlackTex(NULL)
	, BossHPBar_PinkTex(NULL)
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
	, pEnemyBltTex(NULL)		// �G�e�ۃe�N�X�`��.
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

	// �G�Ǘ��N���X�̃I�u�W�F�N�g�𐶐�.
	pEnemyMgr = new EnemyManager(70);
	if (pEnemyMgr == NULL || !pEnemyMgr->Init()) {
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
		pBulletTex = MyTexture::LoadTexture(pDevice, _T("data/image/bullet.png"));
		pMatoTex = MyTexture::LoadTexture(pDevice, _T("data/image/mato.png"));
		pExplosionTex = MyTexture::LoadTexture(pDevice, _T("data/image/effect.png"));

		BossHPBar_BlackTex = MyTexture::LoadTexture(pDevice, _T("data/image/BossHPbar_Black.png"));
		BossHPBar_PinkTex = MyTexture::LoadTexture(pDevice, _T("data/image/BossHPbar_Pink.png"));

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
	if (pEnemyMgr) { delete pEnemyMgr; }
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
	pEnemyMgr->ResetAll();
	pExplMgr->ResetAll();

	// �v���C���[�N���X�̃I�u�W�F�N�g�𐶐�
	pPlayer = new Player(pPlayerTex, pPlayer_WeaponTex);
	pPlayer->Init(D3DXVECTOR2(WIDTH / 2, HEIGHT - 50), D3DXVECTOR2(0.0f, -10.0f), 32.0f, 32.0f, 48.0f, 48.0f, 50);

	CreateEnemyA(300, HEIGHT - 66, 0.0f, 0.0f, 30);
	//CreateEnemyA(200, HEIGHT - 66, 0.0f, 0.0f, 5);
	//CreateEnemyA(600, HEIGHT - 66, 1.0f, 0.0f, 3);

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
		gameState = GAME_STATE_PLAYING;
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
			gameState = GAME_STATE_READY;
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
			gameState = GAME_STATE_TITLE;
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

		// �v���C���[�̍U�����I���������Ƃ����m���A�G�̔�_���[�W�t���O��false�ɂ���
		if (pPlayer->AttackEnd()) {
			for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
				if (pEnemyMgr->PPBuffer(i))pEnemyMgr->DamageFlg(false, i);
			}
		}

		// �G���X�V�A���������蔲���΍�̂��ߎ���������.
		pEnemyMgr->Update(1.0f / 10);

		// �����ɂ͓����蔻��͂Ȃ��̂ŁA�������͕s�v.
		pExplMgr->Update(1);
		// 
		// �G�ƕ���̓����蔻��i������o���Ă���Œ��Ȃ�΁j
		for (int i = 0; i < pEnemyMgr->EnemyMax(); i++) {
			if (pEnemyMgr->PPBuffer(i)) {
				if (pPlayer->IsWeapon() && Collision(pPlayer->GetWeaponXY(), pPlayer->GetWeaponW() / 2, pEnemyMgr->GetXY(&i), 48.0f / 2) && !pEnemyMgr->IsDamage(i)) { // ����̍��W�A����̔��a�A�I�̍��W�A�I�̔��a
					pEnemyMgr->DamageFlg(true, i);
					// ���S�_�̋���
					float lengthX = (pPlayer->GetWeaponXY().x + pEnemyMgr->GetXY(&i).x) / 2;

					float lengthY = (pPlayer->GetWeaponXY().y + pEnemyMgr->GetXY(&i).y) / 2;

					pExplMgr->Explose(lengthX, lengthY);
					pEnemyMgr->Damage(1, i);
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
	// ��ʑS�̂�����.
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0);

	// �`����J�n�i�V�[���`��̊J�n�j.
	pDevice->BeginScene();

	// �X�v���C�g�̕`����J�n.
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// �Q�[���̏�Ԃɂ��`�敪��.
	switch (gameState)
	{
	case GAME_STATE_LOAD:
	{
	}
	break;

	case GAME_STATE_TITLE:
	{
		cnt.x = 128; cnt.y = 32;
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2 - 64;
		//pSprite->Draw(pTitleTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		pos.x = WIDTH / 2; pos.y = HEIGHT / 2 + 64;
		if (stateTimer % 30 < 20)
		{
			//pSprite->Draw(pPushSpaceTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		}

		fps.StartTimer();
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

	case GAME_STATE_PLAYING:
	{
		// �摜�̕\��.
		DrawDataPlaying();
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
		fps.StartTimer();
	}
	break;
	}

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

	/*
	sprintf_s(txt, "bullet:%s", pPlayerBullet->GetText());
	pFont->DrawBmpText(txt, 20, 700, 8, 0xFFFFFFFF);
	*/

	//sprintf_s(txt, "enemies:%s", pEnemyMgr->GetText());
	//pFont->DrawBmpText(txt, 20, 500, 8, 0xFFFFFFFF);

	fps.StartTimer();

	//sprintf_s(txt, "explosion:%s", pExplMgr->GetText());
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
	// �G�̕`��.
	pEnemyMgr->Show();

	// �v���C���[�̕`��.
	pPlayer->Show();

	// �����̕`��
	pExplMgr->Draw(pExplosionTex);

	// �{�X��HP�o�[���X�V
	int u = 0;
	float BossMaxHP = 0;
	float BossHP = 0;
	if (pEnemyMgr->PPBuffer(u)) {
		BossMaxHP = pEnemyMgr->GetMaxHP(&u);
		BossHP = pEnemyMgr->GetHP(&u);
		BossHP = BossHP / BossMaxHP * 200;
	}
	DrawChara(pSprite, BossHPBar_BlackTex->GetTexture(), D3DXVECTOR3(WIDTH - 200 - 32, 0 + 16, 0), RECT{ 0, 0, 200, 18 }, 0.0f, 0.0f, false, false);
	DrawChara(pSprite, BossHPBar_PinkTex->GetTexture(), D3DXVECTOR3(WIDTH - 200 - 32, 0 + 16, 0), RECT{ 0, 0, (int)BossHP, 18 }, 0.0f, 0.0f, false, false);
}

// �����蔻��i�~�j
bool MyApp::Collision(D3DXVECTOR2 a, float a_r, D3DXVECTOR2 b, float b_r) {
	// ���S�_�̋���
	float lengthX = (a.x + a_r) - (b.x + b_r);

	float lengthY = (a.y + a_r) - (b.y + b_r);
	float centerDistance = sqrtf(lengthX * lengthX + lengthY * lengthY);

	//���a�̘a
	float radiusSum = a_r + b_r;

	//���S�_�̋�����蔼�a�̘a�̂ق����傫��
	if (centerDistance <= radiusSum)
	{
		//��̉~���������Ă���
		return true;
	}
	return false; // �������Ă��Ȃ�
}

void MyApp::DrawChara(ID3DXSprite* pSprite, IDirect3DTexture9* pTexture,
	D3DXVECTOR3 pos, RECT rc, float textureWidth, float textureHeight, bool flipHorizontal, bool flipVertical) {
	D3DXVECTOR2 scaling(1.0f, 1.0f); // �X�P�[��
	D3DXVECTOR2 position(pos);     // �`��ʒu
	D3DXVECTOR2 spriteCenter(0.0f, 0.0f); // ��_�i�摜�̍���j

	// �����]
	if (flipHorizontal) {
		scaling.x = -1.0f;
		spriteCenter.x = textureWidth; // �������̊���E�[��
		position.x += textureWidth;
	}

	// �c���]
	if (flipVertical) {
		scaling.y = -1.0f;
		spriteCenter.y = textureHeight; // �c�����̊�����[��
		position.y += textureWidth;
	}

	// �ϊ��s���ݒ�
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scaling, &spriteCenter, 0.0f, &position);
	pSprite->SetTransform(&mat);

	// �`��
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pSprite->Draw(pTexture, &rc, NULL, NULL, D3DCOLOR_XRGB(255, 255, 255));
	pSprite->End();

	// �ϊ��s������Z�b�g
	D3DXMatrixIdentity(&mat);
	pSprite->SetTransform(&mat);
}

// �G�`�𐶐�.
void MyApp::CreateEnemyA(float x, float y, float vx, float vy, int maxhp)
{
	EnemyAAA* pEnemyA = new EnemyAAA(pMatoTex);
	pEnemyA->Init(x, y, vx, vy, maxhp);
	pEnemyMgr->Add(pEnemyA);
}