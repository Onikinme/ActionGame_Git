/*
	myApp.h

	�A�v���P�[�V�����f�[�^�̊Ǘ�
*/
#pragma once

#include "myBitmapFont.h"
#include "myTexture.h"
#include "myTimer.h"
#include "myPlayer.h"
#include "myBossTmp.h"
#include "myEnemyManager.h"
#include "myBulletBuffer.h"
#include "myExplosionBuffer.h"

#define STAGE_COUNT 3

// �Q�[���S�̂̏�ԑJ��.
enum GameState
{
	GAME_STATE_TITLE, 	// �^�C�g�����.
	GAME_STATE_LOAD, 	// �X�e�[�W�ǂݍ���.
	GAME_STATE_READY, 	// �Q�[���J�n�҂�.
	GAME_STATE_PLAYING, // �Q�[���i�s��.
	GAME_STATE_CLEAR, 	// �X�e�[�W�N���A�I.
	GAME_STATE_DEAD, 	// �v���C���[�����j�����.

	GAME_STATE_COUNT,
	GAME_STATE_INVALID = -1,
};

// �v���O�����ɕK�v�ȕϐ���MyApp�N���X�̃����o�[�ϐ��ɂ܂Ƃ߂�.
// �Q�[���ɕK�v�ȃf�[�^���ЂƂ܂Ƃ߂ɂ��ĊǗ����₷������.
class MyApp
{
private:
	static MyApp sm_myApp;			//���̃N���X���g�̃C���X�^���X��static�Ő錾.
	MyApp();						// �R���X�g���N�^��private��.

public:
	// ���C���X�^���X�̃|�C���^�𓾂�static�����o�֐�.
	static MyApp* GetAppInst() { return &sm_myApp; }

	bool	InitApp();
	void	MainLoop();
	void	ReleaseData();			// ���\�[�X���������.
	void	ResetGameData();		// �Q�[�������Z�b�g.

	IDirect3DDevice9* GetDevice() { return pDevice; }		// �X�v���C�g�̎擾.
	ID3DXSprite* GetSprite() { return pSprite; }		// �X�v���C�g�̎擾.
	EnemyManager* GetEnemyMgr() { return pEnemyMgr; }	// �G�Ǘ��N���X�̃|�C���^�擾.
	BulletBuffer* GetEneBltBuf() { return pBossBullet; }// �G�e�ۃo�b�t�@�̎擾.
	ExplosionBuffer* GetExplMgr() { return pExplMgr; }	// �����̃|�C���^�擾.
	MyTexture* GetEffectTex() { return pEffectTex; }	// �G�t�F�N�g�̃e�N�X�`��.
	MyTexture* GetEnemyTexA() { return pEnemyTexA; }	// �GA�̃e�N�X�`��.
	MyTexture* GetEnemyTexB() { return pEnemyTexB; }	// �GB�̃e�N�X�`��.
	MyTexture* GetEnemyTexC() { return pEnemyTexC; }	// �GC�̃e�N�X�`��.
	GameState GetGameState() { return gameState; }	// �Q�[����Ԃ̎擾.

	void	AddScore(int add) { score += add; }
	// �v���C���[�̌��ݍ��W���擾
	void	GetPlayerPos(D3DXVECTOR2& pos)
	{
		pos = pPlayer->GetXY();
	}
	// �v���C���[�̃T�C�Y���擾
	void	GetPlayerSize(float& size)
	{
		size = pPlayer->GetSize();
	}

	// �`��֐�
	void Draw(ID3DXSprite* pSprite, IDirect3DTexture9* pTexture,
		D3DXVECTOR3 pos, RECT rc, float textureWidth, float textureHeight, bool flipHorizontal, bool flipVertical, bool damageflg);
	// �����蔻��i�~�j
	bool Collision(D3DXVECTOR2 a, float a_r, D3DXVECTOR2 b, float b_r);

	// �G�`�𐶐�.
	void CreateEnemyA(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, int maxhp);

	// �{�X�`�𐶐�.
	void CreateBossA(float x, float y, float vx, float vy, int maxhp);

private:
	HRESULT	InitDirect3D();			// Direct3D�̏�����
	BOOL	UpdateData();			// �X�V���C��.
	void	UpdatePlaying(bool);	// GAME_PLAYING��Ԃ̎��ɌĂ�Update
	HRESULT	DrawData();				// �`�惁�C��.
	void	DrawDataPlaying();		// �G�ƒe�ۂ̕\��.

private:
	HINSTANCE hInstance;			// �C���X�^���X�n���h��.
	HWND hWnd;						// �\���E�B���h�E.

	IDirect3D9* pD3D;				// Direct3D�C���X�^���X�I�u�W�F�N�g.

	IDirect3DDevice9* pDevice;		// Direct3D�f�o�C�X�I�u�W�F�N�g.
	ID3DXSprite* pSprite;			// �X�v���C�g.
	MyBitmapFont* pFont;			// �r�b�g�}�b�v�t�H���g.

	MyTexture* pPlayerTex;			// ���@.
	MyTexture* pPlayer_WeaponTex;	// �v���C���[�̕���
	MyTexture* pMatoTex;			// �����蔻��`�F�b�N�p�̓I.
	//MyTexture* pBulletTex;		// ���@�e��.

	BulletBuffer* pBossBullet;		// �{�X�̒e��.
	MyTexture* pEnemyBltTex;		// �G�e�ۃe�N�X�`��.
	BossManager* pBossMgr;
	MyTexture* tmpBossTex;			//�{�Xtmp

	MyTexture* PlayerHPBar_Tex;		// �v���C���[��HP�o�[
	MyTexture* PlayerHPBarFrame_Tex;		// �v���C���[��HP�o�[�̃t���[��
	MyTexture* BossHPBar_Tex;		// �{�X��HP�o�[
	MyTexture* BossHPBarFrame_Tex;		// �{�X��HP�o�[�̃t���[��

	int nextFireFrame;				// ���@�����ɒe�����Ă�܂ł̎��ԁi�t���[���j.
	UINT buttons;					// �{�^���̏��.

	Player* pPlayer;

	EnemyManager* pEnemyMgr;		// �G�̔z��.
	MyTexture* pEnemyTexA;			// �GA�̃e�N�X�`��.
	MyTexture* pEnemyTexB;			// �GB�̃e�N�X�`��.
	MyTexture* pEnemyTexC;			// �GC�̃e�N�X�`��.

	MyTexture* pEffectTex;			// �G�t�F�N�g�̃e�N�X�`��.

	MyTexture* pBackGroundTex;		// �w�i.
	float BackGroundY;			    // �w�i�̌��݈ʒu.

	ExplosionBuffer* pExplMgr;		// �����̔z��.
	MyTexture* pExplosionTex;		// ����.

	int score;						// �X�R�A.
	int playerPower;				// ���@�̑ϋv��.

	UINT joyID;						// �Q�[���R���g���[����ID.
	int gameTimer;					// �S�̂̃^�C�}�[.

	GameState gameState;			// �Q�[���S�̂̏��.
	int stateTimer;					// ��ԑJ�ڗp�^�C�}�[.
	MyTexture* pReadyTex;			// READY.
#if defined(Shiken7)
	MyTexture* pTitleTex;			// TITLE.
	MyTexture* pPushSpaceTex;			// PUSH_SPACE.
	MyTexture* pGameClearTex;			// GameClear.
	MyTexture* pPushBackSpaceTex;			// PUSH_BACKSPACE.
#endif
#if defined(Shiken8)
	MyTexture* pStageTex[STAGE_COUNT];		// STAGE�e�N�X�`��.
	int currentStage;
#else
	MyTexture* pStage1Tex;			// STAGE1.
#endif
	MyTexture* pGameOverTex;		// GAMEOVER.

	CTimer timer;					// ���ԊǗ��I�u�W�F�N�g(Update�p).
	CTimer fps;						// ���ԊǗ��I�u�W�F�N�g(FPS�\���p).
};

inline MyApp* GetApp() { return MyApp::GetAppInst(); }

/*
// �v���O�����ɕK�v�ȕϐ����\���̂Ƃ��Ē�`.
// �i�Q�[���ɕK�v�ȃf�[�^����������ɁA�܂Ƃ߂Ĕz�u���������߁j.
// ����ŃV���O���g�����f���̃N���X�ɕύX����\��.
struct DataSet
{
};

extern DataSet mydata;
*/
