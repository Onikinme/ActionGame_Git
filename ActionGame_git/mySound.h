/*
	mySound.h

	サウンド処理管理
*/
#pragma once
#include <dsound.h>

//#define GAME_VOLUME_MIN	(DSBVOLUME_MIN)
//#define GAME_VOLUME_MAX	(DSBVOLUME_MAX)
  #define GAME_VOLUME_MIN	(-8000)
  #define GAME_VOLUME_MAX	(-5000)

// ＢＧＭ一覧.
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

// 効果音一覧.
enum E_SE
{
	SE_BEAM,		// 発射音.
	SE_TEKI_DEAD,	// 撃墜音.
	SE_HIDAN,		// 被弾音.
	SE_DEAD,		// 死亡音.
	SE_START,		// 開始音.
#if defined(Shiken5)
	SE_CLEAR,		// クリア音.
#endif

	SE_COUNT,
	SE_INVALID = -1,
	SE_BEGIN,
};
#define	IS_VALID_SE(no) ((SE_BEGIN <= (no)) && ((no) < SE_COUNT))

////////////////////////// MySound //////////////////////////
// サウンド１つを扱うためのクラス.
class MySound
{
	friend class MySoundMgr;
private:
	IDirectSoundBuffer* m_pSnd;		// サウンドバッファ.
	float m_currentVol;				// 現在ボリューム.
	float m_targetVol;				// フェードイン／アウト用目標ボリューム.
	float m_changeVol;				// ボリューム変化量.
	INT m_fadeCnt;

	MySound()
	: m_pSnd(NULL)
	, m_currentVol(GAME_VOLUME_MAX)
	, m_targetVol(0)
	, m_changeVol(0)
	, m_fadeCnt(0)
	{}

public:
	~MySound(){ assert(m_pSnd == NULL); }// サウンドバッファの解放はMySoundMgrが責任を持つ.
										// ↑デストラクタが呼び出される前にRelease()するのが望ましい.
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
	static MySoundMgr sm_sndMgrObj;// このクラス自身のオブジェクト（実体）の宣言.

	// 勝手にオブジェクトを追加できないようにコンストラクタをprivate領域に配置.
	MySoundMgr()
	: m_pDSound(NULL)
	, m_currentBgm(BGM_INVALID)
	{}

	MySound	m_bgm[BGM_COUNT];
	MySound	m_se[SE_COUNT];
	E_BGM m_currentBgm;			// 現在ＢＧＭ.

public:
	~MySoundMgr();
	static MySoundMgr* GetInst() { return &sm_sndMgrObj; }

	HRESULT InitSound(HWND);	// 音まわりの初期化.
	void Release();				// サウンドデバイスの解放.

	void LoadSoundData();		// サウンドデータ読み込み.
	void PlayBGM(E_BGM, BOOL lp=TRUE);	// ＢＧＭを再生.
	void FadeinBGM();					// ＢＧＭをフェードイン.
	void FadeoutBGM();					// ＢＧＭをフェードアウト.
	void StopAllBGM();					// ＢＧＭを停止.
	void UpdateAllBGM();				// ＢＧＭを更新.
	void PlaySE(E_SE, BOOL lp=FALSE);	// 効果音を再生.

private:
	IDirectSound* m_pDSound;// DirectSoundデバイス.
	BOOL LoadWaveFile(MySound*, const TCHAR* fname);// wavファイルの読み込み.
	IDirectSoundBuffer* MakeDSB(WAVEFORMATEX*, void* pData, int dataSize);
	void* ReadWave(LPCTSTR pFname, int* pDataSize, WAVEFORMATEX*);
	void FreeWave(void* pData);
};

inline 	MySoundMgr* GetSoundInst() { return MySoundMgr::GetInst(); }
