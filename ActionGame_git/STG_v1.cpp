// STG_v1.cpp
// ���@�̑���ƃV���b�g.

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <d3d9.h>	   // Direct3D9 (DirectX9 Graphics)�ɕK�v.
#include <d3dx9core.h> // DirectX �X�v���C�g�ɕK�v.
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

// �E�B���h�E�̑傫���ݒ�.
#define WIDTH (1024) // ��.
#define HEIGHT (768) // ����.

// �E�B���h�E�^�C�g���o�[�ɕ\�������o�[�W������.
#define WDATE(x, y) _T(x y)
static const TCHAR* version = WDATE(__DATE__, __TIME__);


/////////////////////////////////////////////////////////////
// ���^�C�}�[�N���X.

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

	void InitTimer();			// �^�C�}�[������.
	void StartTimer();			// �^�C�}�[�J�n.
	void GetPassTime(LONGLONG*, UINT*);	// �J�n����̌o�ߎ��Ԃ𓾂�.

	static CTimer* pGetTimer(){return &s_Timer; }
};

CTimer CTimer::s_Timer;

void CTimer::InitTimer()
{
	QueryPerformanceFrequency(&Frequency);
}

// �^�C�}�[�J�n.
void CTimer::StartTimer()
{
	QueryPerformanceCounter(&StartingTime);
}

// �J�n����̌o�ߎ��Ԃ𓾂�.
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

// ���^�C�}�[�N���X.
/////////////////////////////////////////////////////////////

// �N���X�O���錾:���̂̒�`����ɃN���X���������ɐ錾.
// MyTexture *pTex; �̂悤�ȃ|�C���^�錾�ɑ΂��ėL���ɂȂ�.
class MyTexture;
class BulletOne;
class BulletBuffer;
struct DataSet;

// �֐��錾.
HRESULT InitDirect3D(DataSet* pData);
const TCHAR* D3DErrStr(HRESULT res);
void ReleaseDataSet(DataSet* pData);

#define RELEASE(__xx__)    \
	if (__xx__)            \
	{                      \
		__xx__->Release(); \
		__xx__ = 0;        \
	}

// �v���O�����ɕK�v�ȕϐ����\���̂Ƃ��Ē�`.
// �i�Q�[���ɕK�v�ȃf�[�^����������ɁA�܂Ƃ߂Ĕz�u���������߁j.
// ����ŃV���O���g�����f���̃N���X�ɕύX����\��.
struct DataSet
{
	HINSTANCE hInstance;		// �C���X�^���X�n���h��.
	HWND hWnd;					// �\���E�B���h�E.

	IDirect3D9* pD3D;			// Direct3D�C���X�^���X�I�u�W�F�N�g.
	D3DPRESENT_PARAMETERS d3dpp;// �f�o�C�X�쐬���̃p�����[�^.

	IDirect3DDevice9* pDevice;	// Direct3D�f�o�C�X�I�u�W�F�N�g.
	ID3DXSprite* pSprite;		// �X�v���C�g.
	MyBitmapFont* pFont;		// �r�b�g�}�b�v�t�H���g.

	MyTexture* pPlayerTex;		// ���@.
	MyTexture* pBulletTex;		// ���@�e��.
								// ���ӎ���.
								// MyTexture�̎��̂��܂���`����Ă��Ȃ�����.
								// MyTexture plyaer_tex; �Ƃ͏������Ƃ��o���Ȃ�.
								// �|�C���^�^�ł���Η��p���邱�Ƃ��ł���.
								// MyTexture�^�̑傫���͂��̎��_�ł͂܂��s������.
								// �|�C���^��4�o�C�g�Ȃ̂ő傫�����m�肵�Ă���.

	float player_x, player_y;	// ���@�̌��݈ʒu.
	bool m_isGround; // �n�ʂɒ����Ă��邩�H
	float m_speed_x, m_speed_y; // �����x
	float m_gravity; // �d��
	float jumpbutton; // �W�����v�{�^�������t���O
	float shortjump; // �W�����v�㏸���ɃW�����v�{�^���𗣂����t���O
	int player_nextfire;		// ���@�����ɒe�����Ă�܂ł̎���.
	BulletBuffer* pPlayerBullet;// ���@�̒e��.

	UINT joyID;					// �Q�[���R���g���[����ID.
	UINT buttons;				// �{�^���̏��.

	int game_timer;				// �S�̂̃^�C�}�[.
} mydata;


// BulletOne //////////////////
// �e�ۃN���X:�ꔭ�ꔭ�̒e�ۂ���������A�ړ��Ȃ�.
class BulletOne
{
	// ���K�Q.
	// TODO �K�v�ȃ����o�ϐ���ǉ����Ă�������.
private:
	float m_x, m_y;
	float m_vx, m_vy;
	float m_timer;
public:
	// ���K�Q.
	// TODO �����o�ϐ���ǉ�������R���X�g���N�^�ŏ��������Ă�������.
	BulletOne()
		: m_x(0.0f)
		, m_y(0.0f)
		, m_vx(0.0f)
		, m_vy(0.0f)
		, m_timer(-1.0f)
	{}

	// �f�X�g���N�^.
	~BulletOne(){}

	// �ˏo������.
	void Init(float x, float y, float vx, float vy);

	// �e�ۂ�i�߂�.g
	void Update(float fTime);

	// ���݂�XY���W�l�𓾂�.
	void GetXY(float& x, float& y);

	// �^�C�}�[��0�ȏ�Ȃ�L���i�C�����C���֐��j.
	bool IsActive();
};

// �ˏo������.
void BulletOne::Init(float x, float y, float vx, float vy)
{
	// x (���ʒuX), y (�����ʒuY), vx (�����xX), vy (�����xY).

	// ���K�Q.
	// TODO �e�ۂ����˂��ꂽ�Ƃ��̏�������ݒ肷�鏈�����L�q���Ă�������.
	m_x = x;
	m_y = y;
	m_vx = vx;
	m_vy = vy;
	m_timer = 0.0f;
}

// �e�ۂ�i�߂�.
void BulletOne::Update(float fTime)
{
	// ���K�Q.
	// TODO �e�ۂ���ʏ���Ɉړ����鏈�����L�q���Ă�������.
	if (m_timer >= 0.0f) {
		m_x += (m_vx * fTime);
		m_y += (m_vy * fTime);
		m_timer += fTime;
	}
	if (m_y < 0.0f) {
		m_timer = -1.0f;
	}
}

// ���݂�XY���W�l�𓾂�.
void BulletOne::GetXY(float& x, float& y)
{
	// ���K�Q
	// TODO �����o�ϐ��̒l���擾���鏈�����L�q���Ă�������.
	x = m_x;
	y = m_y;
}

// �^�C�}�[��0�ȏ�Ȃ�L���i�C�����C���֐��j.
bool BulletOne::IsActive()
{
	// ���K�Q
	// TODO
	return (m_timer >= 0.0f);
} 


// BulletBuffer //////////////////
// �e�ۃo�b�t�@���Ǘ�����N���X�BBulletOne�N���X�͒P��̒e�ۂ��Ǘ����Ă���.
// �����̒e�ۂ̊Ǘ��͂�����̃N���X�ōs���B�N���X���ɕ����邱�Ƃ�.
// ���݂��̒S���͈́i�ӔC�͈́j�𕪂��邱�Ƃ��ł��A�v���O�����̌��ʂ���ǂ�����.
class BulletBuffer
{
private:
	BulletOne* m_pBuffer;	// �e�ۂ̔z��.
	int m_index;			// �z���̌��݈ʒu������.
	int m_bulletMax;		// �z��̑傫��.
	char* m_pTextBuffer;	// �f�o�b�O�p�̃e�L�X�g�o�b�t�@.

public:
	BulletBuffer(int max);
	virtual ~BulletBuffer();
	BulletOne* pNext(); // ���̋󂫈ʒu��Ԃ�.
	BulletOne* pGet(int ix)
	{
		return m_pBuffer + ix; // &(m_pBuffer[ix]) �Ɠ����Ӗ�.
	}
	void Update(float fTime); // �z��̓��e�ł���e�ۑS�̂��X�V����.
	void Draw(DataSet* pData);
	char* pGetText();
};

// �R���X�g���N�^:��������z��̍ő�l�𓾂āA�z����쐬���Ă���.
BulletBuffer::BulletBuffer(int max)
	: m_index(0)
	, m_pBuffer(NULL)
	, m_bulletMax(max)
	, m_pTextBuffer(NULL)
{
	m_pTextBuffer = new char[max + 1];
	m_pBuffer = new BulletOne[max];
}

// �f�X�g���N�^:�z���������Ă���.
BulletBuffer::~BulletBuffer()
{
	delete[] m_pBuffer;		// delete m_pBuffer�łȂ����Ƃɒ���.
	delete[] m_pTextBuffer;	// �z��Ŋm�ۂ����������̈��[]��t����delete���邱��.
}

// �����O�o�b�t�@��T�����āA�󂫂�������.
// �߂�l : NULL�@�@�@�ˁ@�󂫂Ȃ�.
//			NULL�ȊO�@�ˁ@�󂫈ʒu.
BulletOne* BulletBuffer::pNext()
{
	// ���K�R.
	// TODO
	for (int i = 0; i < m_bulletMax; i++) {
		int ix = (m_index + i) % m_bulletMax;
		if ( !m_pBuffer[ix].IsActive() ) {

			m_index = (ix + 1) % m_bulletMax;
			return &m_pBuffer[ix];
		}
	}

	return NULL;// �󂫂Ȃ�.
}

// �����O�o�b�t�@���̃f�[�^�������X�V����.
void BulletBuffer::Update(float fTime)
{
	for (int i = 0; i < m_bulletMax; i++)
	{
		m_pBuffer[i].Update(fTime);
	}
}

// �e�ۂ�\��.
void BulletBuffer::Draw(DataSet* pData)
{
	D3DXVECTOR3 cnt(8, 8, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	for (int i = 0; i < m_bulletMax; i++)
	{
		BulletOne& blt = m_pBuffer[i];
		// �擾�����e�ۂ��L���Ȃ�\��.
		if (blt.IsActive())
		{
			blt.GetXY(pos.x, pos.y); // ���݈ʒu���擾.
			if (pData->pBulletTex != NULL) {
				pData->pSprite->Draw(pData->pBulletTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
			}
		}
	}
}

// �f�o�b�O�p��pBuffer�z��̒��g���e�L�X�g������.
// �e�ۂ��L���Ȃ�*�A�����Ȃ�-�ƈꕶ���ɂ���.
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
	// ������̍Ō�͕K��NULL�Ń^�[�~�l�[�g.
	m_pTextBuffer[m_bulletMax] = NULL;
	return m_pTextBuffer;
}

// �f�[�^���A�b�v�f�[�g����.
void UpdateData(DataSet* pData)
{
	MyInput* pInput = GetInputInst();

	pData->game_timer++;
	pData->buttons = 0;

	// ���͏����X�V.
	pInput->UpdateInput(pData->hWnd);

	// �W���C�X�e�B�b�N�ł̑���.
	if (pInput->GetJoyCnt() > 0) {
		// ���X�e�b�N�ňړ�.
		int jx = pInput->GetStickLX(JOY_CON_0);
		int jy = pInput->GetStickLY(JOY_CON_0);

		// �����т��z�����邽�߁A���l���傫����(��������)�𔻒肷��.
		if (abs(jx) > 10) {
			pData->player_x += jx / 50;
		}
		if (abs(jy) > 10) {
			pData->player_y += jy / 50;
		}

		if (pInput->IsPushBtn(JOY_CON_0, JOY_BTN_BIT_A)) {
			pData->buttons |= JOY_BTN_BIT_A;// �`�{�^���Œe�۔���.
		}
	}
	// �L�[�{�[�h�ł̑���.
	{
		// ���ɓ���
		if (pInput->IsPushKey(DIK_A)) {
			// �󒆂ɂ��āA���E�����ւ̊����������Ă���Ƃ��A
			// �������ɓ����₷���Ȃ�
			if (!pData->m_isGround && 0 < pData->m_speed_x) {
				pData->m_speed_x -= 0.3f;
			}
			else pData->m_speed_x -= 0.1f;
			if (pData->m_speed_x < -5.0f)pData->m_speed_x = -5.0f;
		}
		// �E�ɓ���
		if (pInput->IsPushKey(DIK_D)) {
			// �󒆂ɂ��āA���������ւ̊����������Ă���Ƃ��A
			// �E�����ɓ����₷���Ȃ�
			if (!pData->m_isGround && 0 > pData->m_speed_x) {
				pData->m_speed_x += 0.3f;
			}
			else pData->m_speed_x += 0.1f;
			if (pData->m_speed_x > 5.0f)pData->m_speed_x = 5.0f;
		}

		// �L�[�𗣂������̌�������
		if (!pInput->IsPushKey(DIK_A) && 0 > pData->m_speed_x) {
			if (pData->m_isGround) { // �n��ł̓u���[�L���ǂ�����
				pData->m_speed_x += 0.1f;
				pData->m_speed_x *= 0.93f;
			}
			else { // �󒆂ł͉������Ɋ������������߁A�u���[�L�������Â炢
				pData->m_speed_x += 0.01f;
				pData->m_speed_x *= 0.99f;
			}
			if (pData->m_speed_x > 0) pData->m_speed_x = 0;
		}
		if (!pInput->IsPushKey(DIK_D) && 0 < pData->m_speed_x) {
			if (pData->m_isGround) { // �n��ł̓u���[�L���ǂ�����
				pData->m_speed_x -= 0.1f;
				pData->m_speed_x *= 0.93f;
			}
			else { // �󒆂ł͉������Ɋ������������߁A�u���[�L�������Â炢
				pData->m_speed_x -= 0.01f;
				pData->m_speed_x *= 0.99f;
			}
			if (pData->m_speed_x < 0) pData->m_speed_x = 0;
		}
		pData->player_x += pData->m_speed_x;

		if (pData->player_x < 0)pData->player_x = 0;
		if(pData->player_x > 1024)pData->player_x = 1024;

		// �v���C���[���n�ʂɒ����Ă�����W�����v�\
		if (pData->m_isGround) {
			if (pInput->IsPushKey(DIK_SPACE) && pData->player_y == 668 && !pData->jumpbutton) {
				pData->m_speed_y = -10.0f;  // �����x�̐ݒ�
				//Y���W�̍X�V
				pData->player_y += pData->m_speed_y;
				pData->jumpbutton = true;
				pData->m_isGround = false;
			}
		}
		// �W�����v��
		else {
			//Y�����̑��x�ɉ����x��������
			pData->m_speed_y += pData->m_gravity;
			//Y���W�̍X�V
			pData->player_y += pData->m_speed_y;

			//�㏸���Ƀ{�^���𗣂����Ƃ��̏���
			if (!pInput->IsPushKey(DIK_SPACE))pData->shortjump = true;
			if(pData->m_speed_y < 0 && pData->shortjump)pData->m_speed_y *= 0.5f;
		}

		// �W�����v�{�^���������ŃW�����v�������Ă��܂�Ȃ��悤�Ƀ`�F�b�N
		if (!pInput->IsPushKey(DIK_SPACE) && pData->jumpbutton && pData->player_y == 668) {
			pData->jumpbutton = false;
		}

		// �n�ʂɒ��������`�F�b�N
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
			pData->buttons |= JOY_BTN_BIT_A;// �X�y�[�X�L�[�Œe�۔���.
		}
		*/
	}

	// �{�^����������Ă���΁A�e�۔���.
	if ((pData->buttons & JOY_BTN_BIT_A) && pData->player_nextfire <= 0)
	{
		BulletOne* pBulletOne = pData->pPlayerBullet->pNext();
		if (pBulletOne != NULL)
		{
			float fXSpeed = 0.0f;
			float fYSpeed = 20.0f;
			pBulletOne->Init(pData->player_x, pData->player_y, fXSpeed, -fYSpeed);

			_tprintf(_T("���� pBulletOne=%p\n"), pBulletOne);
			// �ˏo��AN�J�E���g(1�J�E���g��1/60sec)�͔��˕s��.
			pData->player_nextfire = 5;
		}
	}
	// ���ˉ\�ɂȂ�܂ł̃^�C�����O���c���Ă��邩�ǂ���.
	if (pData->player_nextfire > 0)
	{
		pData->player_nextfire--;
	}

	// �e�ۂƓG���X�V�A���������蔲���΍�̂��ߎ���������.
	for (int i = 0; i < 10; i++)
	{
		pData->pPlayerBullet->Update(1.0f / 10);
	}
}


// DataSet�Ɋ�Â��āA�X�v���C�g��`��.
// param pData �f�[�^�Z�b�g.
// return ���������G���[.
HRESULT DrawData(DataSet* pData)
{
	D3DCOLOR rgb = D3DCOLOR_XRGB(10, 10, 80);
	// ��ʑS�̂�����.
	pData->pDevice->Clear(0, NULL, D3DCLEAR_TARGET, rgb, 1.0f, 0);

	// �`����J�n�i�V�[���`��̊J�n�j.
	pData->pDevice->BeginScene();

	// �X�v���C�g�̕`����J�n.
	pData->pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	// �e�ۂ̕\���B�z������ԂɒT�����ĕ\������.
	pData->pPlayerBullet->Draw(pData);

	// �v���C���[�@�i���@�j�̕`��.
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

	// �X�v���C�g�̕`����I��.
	pData->pSprite->End();

	// �f�o�b�O���̕\��.
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

	// �`����I���i�V�[���`��̏I���j.
	pData->pDevice->EndScene();

	// ���ۂɉ�ʂɕ\���B�o�b�N�o�b�t�@����t�����g�o�b�t�@�ւ̓]��.
	// �f�o�C�X�������̃t���O�w��ɂ��A������VSYNC��҂�.
	pData->pDevice->Present(NULL, NULL, NULL, NULL);


	return D3D_OK;
}


// �C�x���g�����R�[���o�b�N�i�E�B���h�E�v���V�[�W���j.
LRESULT CALLBACK
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �C�x���g�̎�ނɉ����āAswitch���ɂď�����؂蕪����.
	switch (uMsg)
	{
	case WM_KEYDOWN:
		// ESC�L�[���������ꂽ��I��.
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;

	case WM_CLOSE: // �I���ʒm(CLOSE�{�^���������ꂽ�ꍇ�Ȃ�)���͂����ꍇ.
				   // �v���O�������I�������邽�߁A�C�x���g���[�v��0��ʒm����.
				   // ���̌��ʁAGetMessage�̖߂�l��0�ɂȂ�.
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	// �f�t�H���g�̃E�B���h�E�C�x���g����.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Window���쐬����.
// return �E�B���h�E�n���h��.
HWND InitWindow(DataSet* pData)
{
	// �E�B���h�E�N���X�o�^.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));					 // �ϐ�wc���[���N���A����.
	wc.cbSize = sizeof(WNDCLASSEX);							 // �\���̂̑傫����^����.
	wc.lpfnWndProc = (WNDPROC)WindowProc;					 // �E�B���h�E�v���V�[�W���o�^.
	wc.hInstance = pData->hInstance;						 // �C���X�^���X�n���h���ݒ�.
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);				 // �}�E�X�J�[�\���o�^.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH); // �w�i��GRAY��.
	wc.lpszClassName = _T("Window01");						 // �N���X���ACreateWindow�ƈ�v������.
	RegisterClassEx(&wc);									 // �o�^.

	RECT rc = { 0, 0, WIDTH, HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_tprintf(_T("rc: %d, %d -- %d, %d\n"), rc.left, rc.top, rc.right, rc.bottom);

	// �E�B���h�E���쐬����B�N���X����"Window01"
	pData->hWnd = CreateWindow(_T("Window01"), version, WS_OVERLAPPEDWINDOW,
		0, 0, WIDTH, HEIGHT, NULL, NULL, pData->hInstance, NULL);

	return pData->hWnd;
}

// ���C�����[�v.
void MainLoop(DataSet* pData)
{
	HRESULT hr = E_FAIL;

	// �E�B���h�E���쐬����.
	pData->hWnd = InitWindow(pData);

	// Direct3D������������.
	hr = InitDirect3D(pData);
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("Direct3D���������s"), MB_OK);
		return;
	}

	// �X�v���C�g�̍쐬
	hr = D3DXCreateSprite(pData->pDevice, &(pData->pSprite));
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("�X�v���C�g�쐬���s"), MB_OK);
		return;
	}

	// ���̓f�o�C�X�̏�����.
	GetInputInst()->InitInput(pData->hInstance, pData->hWnd);
	if (GetInputInst()->GetJoyCnt() == 0) {
		MessageBox(NULL, _T("�W���C�X�e�B�b�N���q�����Ă��܂���\r\n�L�[�{�[�h�ő��삵�܂�"), _T("JOYSTICK"), MB_OK);
	}

	// �Ƃ肠���������l�����Ă���.
	pData->player_x = WIDTH / 2;
	pData->player_y = HEIGHT - 100;
	pData->m_isGround = true; // �v���C���[���n�ʂɂ��Ă��邩�H
	pData->m_speed_x = 0.0f; // �������ւ̃X�s�[�h
	pData->m_speed_y = -12.0f; // �W�����v��
	pData->m_gravity = 0.3f; // �d��
	pData->pPlayerBullet = new BulletBuffer(3);// ��ʏ�ɂR���܂őłĂ�.

	// ��O����.
	// �e�N�X�`����ǂݍ��݁A�G���[���ɂ�LoadTexture�͗�O�𔭐�������.
	// LoadTexture���Ƃ�if���������͖̂ʓ|�Ȃ̂ŁA��O�������g�p.
	try
	{
		pData->pFont = MyBitmapFont::LoadFont(pData->pDevice, _T("image/fontBitmap.png"));
		// ���K�P.
		// TODO
		pData->pPlayerTex = MyTexture::LoadTexture(pData->pDevice, _T("image/playerkari.png"));
		pData->pBulletTex = MyTexture::LoadTexture(pData->pDevice, _T("image/bullet.png"));
	}
	// catch����try�̒���ɋL�q����.
	catch (HRESULT /*hr*/)//�����ł͕ϐ� hr �͎g���Ȃ����߁Awarning�΍�̂��߃R�����g�A�E�g���Ă���.
	{
		return;
	}

	ShowWindow(pData->hWnd, SW_SHOWNORMAL);// �쐬�����E�B���h�E��\������.

	CTimer::pGetTimer()->InitTimer();
	CTimer::pGetTimer()->StartTimer();

	// �C�x���g���[�v.
	// �u���b�N�^�֐�GetMessage�ł͂Ȃ��m���u���b�N�^�֐���PeekMessage���g��.
	MSG msg;
	bool flag = true;
	while (flag)
	{
		// ���b�Z�[�W�����邩�ǂ����m�F����.
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			// ���b�Z�[�W������̂ŏ�������.
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
			UpdateData(pData); // �ʒu�̍Čv�Z.
			DrawData(pData);	  // �`��.
		}
		// Sleep�ȂǍs���Ă��Ȃ����ADrawData�֐�����pData->pDevice->Present��.
		// ��ʍX�V�҂����s���̂ŁA���ʓI��1/60���ƂɃC�x���g���[�v�����.
	}
}

// DataSet���������.
void ReleaseDataSet(DataSet* pData)
{
	// �e�N�X�`���̊J��.
	MyTexture::ReleaseAll();

	// ���I�I�u�W�F�N�g�̊J��.
	if (pData->pFont)			{ delete pData->pFont; }
	if (pData->pPlayerBullet)	{ delete pData->pPlayerBullet; }

	// ���̓f�o�C�X�̉��.
	GetInputInst()->Release();

	// DirectX�f�o�C�X�̊J��.
	RELEASE(pData->pSprite);
	RELEASE(pData->pDevice);
	RELEASE(pData->pD3D);
}


// param argc �R�}���h���C������n���ꂽ�����̐�.
// param argv �����̎��̂ւ̃|�C���^�z��.
// return 0 ����I��.
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Japanese"); // ���{��Ƀ��P�[����ݒ�.

#ifdef MEMORY_LEAK
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	// ���̃v���O���������s�����Ƃ��̃C���X�^���X�n���h�����擾.
	mydata.hInstance = GetModuleHandle(NULL);
	MainLoop(&mydata);

	ReleaseDataSet(&mydata);

#ifdef MEMORY_LEAK
	_CrtDumpMemoryLeaks(); // ���������[�N�����o����.
#endif

	return 0;
}

// Direct3D������������.
// param pData �f�[�^�Z�b�g.
// return ���������G���[�܂���D3D_OK.
//
HRESULT InitDirect3D(DataSet* pData)
{
	HRESULT hr;

	// Direct3D�C���X�^���X�I�u�W�F�N�g�𐶐�����.
	// D3D_SDK_VERSION�ƁA�����^�C���o�[�W�����ԍ����K�؂łȂ��ƁANULL���Ԃ�.
	pData->pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	// NULL�Ȃ烉���^�C�����s�K��.
	if (pData->pD3D == NULL)
	{
		return E_FAIL;
	}

	// �f�B�X�v���C���[�h�̎擾.
	D3DDISPLAYMODE d3dd;
	hr = pData->pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dd);
	if (FAILED(hr))
	{
		MessageBox(NULL, D3DErrStr(hr), _T("�f�B�X�v���C���[�h�擾���s"), MB_OK);
		return hr;
	}

	// PRESENT�p�����[�^���[���N���A���A�K�؂ɏ�����.
	ZeroMemory(&(pData->d3dpp), sizeof(pData->d3dpp));
	// �E�B���h�E���[�h��.
#ifdef FULLSCREEN
	pData->d3dpp.Windowed = FALSE;
	// int fx = GetSystemMetrics(SM_CXSCREEN);
	// int fy = GetSystemMetrics(SM_CYSCREEN);
	int fx = WIDTH;
	int fy = HEIGHT;
	_tprintf(_T("�t���X�N���[�� %d x %d\n"), fx, fy);
	pData->d3dpp.BackBufferHeight = fy;
	pData->d3dpp.BackBufferWidth = fx;
#else
	pData->d3dpp.Windowed = TRUE;
#endif
	pData->d3dpp.BackBufferHeight = HEIGHT;
	pData->d3dpp.BackBufferWidth = WIDTH;
	pData->d3dpp.BackBufferCount = 1;
	pData->d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// �o�b�N�o�b�t�@��RGB���ꂼ��W�r�b�g��.
	pData->d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	// Present���ɐ��������ɍ��킹��.
	pData->d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BHAL&HARD.
	hr = pData->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pData->hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &(pData->d3dpp), &(pData->pDevice));
	if (hr == D3D_OK)
	{
		// ���_�v�Z�ƃ��X�^���C�Y�i�f�B�X�v���C�ւ̓��e�v�Z�j�𗼕��Ƃ�GPU�łł���΍ő�.
		_tprintf(_T("HAL & HARD\n"));
		return hr;
	}

	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BHAL&SOFT.
	hr = pData->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pData->hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(pData->d3dpp), &(pData->pDevice));
	if (hr == D3D_OK)
	{
		// ���_�v�Z��CPU�A���X�^���C�Y��GPU�ŏo����Ύ��_�ō̗p.
		_tprintf(_T("HAL & SOFT\n"));
		return hr;
	}

	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BREF&HARD.
	hr = pData->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, pData->hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &(pData->d3dpp), &(pData->pDevice));
	if (hr == D3D_OK)
	{
		// ���_�v�Z��GPU�A���X�^���C�Y��CPU�ŏo����΍̗p.
		_tprintf(_T("REF & HARD\n"));
		return hr;
	}

	// D3D�f�o�C�X�I�u�W�F�N�g�̍쐬�BREF&SOFT.
	hr = pData->pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, pData->hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &(pData->d3dpp), &(pData->pDevice));
	if (hr == D3D_OK)
	{
		// ���_�v�Z�����X�^���C�Y��GPU�ŏo���Ȃ��ƍł��x��.
		_tprintf(_T("REF & SOFT\n"));
		return hr;
	}

	// D3D�f�o�C�X���쐬�ł��Ȃ�����.
	return hr;
}

// �G���[��������HRESULT�𕶎���ɕϊ����邽�߂̕⏕�֐�.
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
