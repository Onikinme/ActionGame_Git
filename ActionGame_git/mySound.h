/*
	mySound.h

	�T�E���h�����Ǘ�
*/
#pragma once
#include <dsound.h>

//#define GAME_VOLUME_MIN	(DSBVOLUME_MIN)
//#define GAME_VOLUME_MAX	(DSBVOLUME_MAX)
  #define GAME_VOLUME_MIN	(-8000)
  #define GAME_VOLUME_MAX	(-5000)

// �a�f�l�ꗗ.
enum E_BGM
{
	BGM_0,
	BGM_1,
	BGM_2,

	BGM_COUNT,
	BGM_INVALID = -1,
	BGM_BEGIN,
};
#define	IS_VALID_BGM(no) ((BGM_BEGIN <= (no)) && ((no) < BGM_COUNT))

// ���ʉ��ꗗ.
enum E_SE
{
	SE_BEAM,		// ���ˉ�.
	SE_TEKI_DEAD,	// ���ĉ�.
	SE_HIDAN,		// ��e��.
	SE_DEAD,		// ���S��.
	SE_START,		// �J�n��.
#if defined(Shiken5)
	SE_CLEAR,		// �N���A��.
#endif

	SE_COUNT,
	SE_INVALID = -1,
	SE_BEGIN,
};
#define	IS_VALID_SE(no) ((SE_BEGIN <= (no)) && ((no) < SE_COUNT))

////////////////////////// MySound //////////////////////////
// �T�E���h�P���������߂̃N���X.
class MySound
{
	friend class MySoundMgr;
private:
	IDirectSoundBuffer* m_pSnd;		// �T�E���h�o�b�t�@.
	float m_currentVol;				// ���݃{�����[��.
	float m_targetVol;				// �t�F�[�h�C���^�A�E�g�p�ڕW�{�����[��.
	float m_changeVol;				// �{�����[���ω���.
	INT m_fadeCnt;

	MySound()
	: m_pSnd(NULL)
	, m_currentVol(GAME_VOLUME_MAX)
	, m_targetVol(0)
	, m_changeVol(0)
	, m_fadeCnt(0)
	{}

public:
	~MySound(){ assert(m_pSnd == NULL); }// �T�E���h�o�b�t�@�̉����MySoundMgr���ӔC������.
										// ���f�X�g���N�^���Ăяo�����O��Release()����̂��]�܂���.
	void Release();
	void Play(BOOL loop);
	void Stop();
	void Fadein();
	void Fadeout();
	void UpdateSound();
};

////////////////////////// MySoundMgr //////////////////////////
class MySoundMgr
{
private:
	static MySoundMgr sm_sndMgrObj;// ���̃N���X���g�̃I�u�W�F�N�g�i���́j�̐錾.

	// ����ɃI�u�W�F�N�g��ǉ��ł��Ȃ��悤�ɃR���X�g���N�^��private�̈�ɔz�u.
	MySoundMgr()
	: m_pDSound(NULL)
	, m_currentBgm(BGM_INVALID)
	{}

	MySound	m_bgm[BGM_COUNT];
	MySound	m_se[SE_COUNT];
	E_BGM m_currentBgm;			// ���݂a�f�l.

public:
	~MySoundMgr();
	static MySoundMgr* GetInst() { return &sm_sndMgrObj; }

	HRESULT InitSound(HWND);	// ���܂��̏�����.
	void Release();				// �T�E���h�f�o�C�X�̉��.

	void LoadSoundData();		// �T�E���h�f�[�^�ǂݍ���.
	void PlayBGM(E_BGM, BOOL lp=TRUE);	// �a�f�l���Đ�.
	void FadeinBGM();					// �a�f�l���t�F�[�h�C��.
	void FadeoutBGM();					// �a�f�l���t�F�[�h�A�E�g.
	void StopAllBGM();					// �a�f�l���~.
	void UpdateAllBGM();				// �a�f�l���X�V.
	void PlaySE(E_SE, BOOL lp=FALSE);	// ���ʉ����Đ�.

private:
	IDirectSound* m_pDSound;// DirectSound�f�o�C�X.
	BOOL LoadWaveFile(MySound*, const TCHAR* fname);// wav�t�@�C���̓ǂݍ���.
	IDirectSoundBuffer* MakeDSB(WAVEFORMATEX*, void* pData, int dataSize);
	void* ReadWave(LPCTSTR pFname, int* pDataSize, WAVEFORMATEX*);
	void FreeWave(void* pData);
};

inline 	MySoundMgr* GetSoundInst() { return MySoundMgr::GetInst(); }
