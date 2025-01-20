/*
	mySound.cpp

	サウンド処理管理
*/
#include "stdafx.h"
#include "mySound.h"
#include "myFile.h"

////////////////////////// MySound //////////////////////////
void MySound::Release()
{
	if (m_pSnd) {
		m_pSnd->Release();
		m_pSnd = NULL;
	}
}

// 再生。現在再生中の音は止めて、先頭から再生しなおす.
// loopフラグをtrueにすると、ループ再生となる.
void MySound::Play(BOOL loop)
{
	if (m_pSnd) {
		m_pSnd->Stop();
		m_pSnd->SetVolume(GAME_VOLUME_MAX);
		m_pSnd->SetCurrentPosition(0);
		m_pSnd->Play(0, 0, (loop ? DSBPLAY_LOOPING : 0));
	}
}

// 停止。強制的に停止する.
void MySound::Stop()
{
	if (m_pSnd) {
		m_pSnd->Stop();
	}
}

void MySound::Fadein()
{
	if (m_pSnd) {
		m_currentVol = GAME_VOLUME_MIN;
		m_pSnd->SetVolume(GAME_VOLUME_MIN);

		m_targetVol = GAME_VOLUME_MAX;
		m_fadeCnt = 60 * 3;// ３秒でフェードインする設定.
		m_changeVol = (m_targetVol - m_currentVol) / m_fadeCnt;
	}
}

void MySound::Fadeout()
{
	if (m_pSnd) {
		LONG volume;
		m_pSnd->GetVolume(&volume);

		m_targetVol = GAME_VOLUME_MIN;
		m_fadeCnt = 60 * 3;// ３秒でフェードアウトする設定.
		m_changeVol = (m_targetVol - volume) / m_fadeCnt;
	}
}

void MySound::UpdateSound()
{
	if (!m_pSnd) {
		return;
	}

	if (m_fadeCnt > 0) {
		m_fadeCnt--;
		if (m_fadeCnt == 0) {
			m_currentVol = m_targetVol;
		}
		else {
			m_currentVol += m_changeVol;
		}

		DWORD dwStatus;
		HRESULT ret = m_pSnd->GetStatus(&dwStatus);
		assert(ret == DS_OK);
		if (dwStatus & DSBSTATUS_PLAYING) {
			m_pSnd->SetVolume((INT)m_currentVol);
		}

		if (m_currentVol <= GAME_VOLUME_MIN) {
			m_pSnd->Stop();

			// 次の再生に備えるためにMAX音量に戻す.
			m_currentVol = GAME_VOLUME_MAX;
			m_pSnd->SetVolume((INT)m_currentVol);
		}
		printf("m_currentVol = %f\n", m_currentVol);
	}
}

////////////////////////// MySoundMgr //////////////////////////
MySoundMgr MySoundMgr::sm_sndMgrObj;// このクラス自身の唯一のオブジェクト（実体）の宣言.

MySoundMgr::~MySoundMgr()
{
	// デストラクタが呼び出される前にRelease()を呼び出して解放していないとアサート.
	assert(m_pDSound == NULL);
	Release();// 念のためRelease呼び出しはしておく.
}

// DirectSoundを初期化する.
HRESULT MySoundMgr::InitSound(HWND hWnd)
{
	HRESULT hr = S_OK;
	assert(m_pDSound == NULL);
	if (m_pDSound == NULL) {
		// DirectSoundの生成。デフォルトサウンドデバイスを指定する(第一引数NULL）.
		hr = DirectSoundCreate(NULL, &m_pDSound, NULL);
		if (FAILED(hr)){
			MessageBox(NULL, _T("DirectSoundの作成に失敗"), _T("DirectSound"), MB_OK);
			return hr;
		}

		assert(m_pDSound != NULL);
		// 協調レベルを設定する。DSSCL_PRIORITYが推奨されている.
		hr = m_pDSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
		if (FAILED(hr)){
			MessageBox(NULL, _T("SetCooperativeLevelに失敗。他のアプリが占有しています"), _T("DirectSound"), MB_OK);
			return hr;
		}
	}
	return S_OK;
}

// Directsoundを解放する.
void MySoundMgr::Release()
{
	// ダイレクトサウンドの解放の前にサウンドバッファを解放しておかないといけない.
	for (int i = SE_COUNT - 1; i >= 0; i--) {
		m_se[i].Release();
	}
	for (int i = BGM_COUNT - 1; i >= 0; i--) {
		m_bgm[i].Release();
	}
	if (m_pDSound != NULL) {
		m_pDSound->Release();
		m_pDSound = NULL;
	}
}

// サウンドデータ読み込み.
void MySoundMgr::LoadSoundData()
{
	// BGMの読み込み.
	static const TCHAR* pcszBgmFileName[BGM_COUNT] = {
		_T("data/bgm/launch.wav"),		// BGM0.
		_T("data/bgm/C1_0838AB.wav"),	// BGM1.
		_T("data/bgm/C1_0523AB.wav"),	// BGM2.
		_T("data/bgm/tutorial.wav"),		// BGM3.
		_T("data/bgm/boss1.wav"),	// BGM4.
		_T("data/bgm/boss2.wav"),	// BGM5.
	};
	for (int i = BGM_BEGIN; i < BGM_COUNT; i++) {
		if (!LoadWaveFile(&m_bgm[i], pcszBgmFileName[i])) {
			throw -1;
		}
	}

	// SEの読み込み.
	static const TCHAR* pcszSeFileName[SE_COUNT] = {
		_T("data/se/Space_Gun_01.wav"), // 発射音.
		_T("data/se/bakuha.wav"),		// 撃墜音.
		_T("data/se/ganx.wav"),			// 被弾音.
		_T("data/se/daibakuhatsu.wav"),	// 死亡音.
		_T("data/se/S02_AO02.wav"),		// 開始音.
		_T("data/se/jingle_boss.wav"),	// WARNING音.
		_T("data/se/attack.wav"),		// 攻撃音.
		_T("data/se/attack_hit.wav"),	// 攻撃ヒット音.
		_T("data/se/playerattack_hit.wav"),		// 攻撃音.
		_T("data/se/boss1_fire.wav"),	// 炎音.
		_T("data/se/playerbullet.wav"),		// 攻撃音.
		_T("data/se/playerbullet_hit.wav"),		// 攻撃音.
		_T("data/se/clear.wav"),		// 攻撃音.
		_T("data/se/gameover.wav"),		// 攻撃音.
		#if defined(Shiken5)
		_T("data/se/Clear.wav"),		// クリア音.
		#endif
	};
	for (int i = SE_BEGIN; i < SE_COUNT; i++) {
		if (!LoadWaveFile(&m_se[i], pcszSeFileName[i])) {
			throw -1;
		}
	}
}

// ＢＧＭを再生.
void MySoundMgr::PlayBGM(E_BGM no, BOOL loop)
{
	assert(IS_VALID_BGM(no));
	if (IS_VALID_BGM(no)) {
		m_bgm[no].Play(loop);
		m_currentBgm = no;
	}
}

// ＢＧＭをフェードイン.
void MySoundMgr::FadeinBGM()
{
	assert(IS_VALID_BGM(m_currentBgm));
	if (IS_VALID_BGM(m_currentBgm)) {
		m_bgm[m_currentBgm].Fadein();
	}
}

// ＢＧＭをフェードアウト.
void MySoundMgr::FadeoutBGM()
{
	assert(IS_VALID_BGM(m_currentBgm));
	if (IS_VALID_BGM(m_currentBgm)) {
		m_bgm[m_currentBgm].Fadeout();
	}
}

// 全ＢＧＭを停止.
void MySoundMgr::StopAllBGM()
{
	for (int i = 0; i < BGM_COUNT; i++) {
		m_bgm[i].Stop();
	}
}

// 全ＢＧＭを更新.
void MySoundMgr::UpdateAllBGM()
{
	for (int i = 0; i < BGM_COUNT; i++) {
		m_bgm[i].UpdateSound();
	}
}

// 効果音を再生.
void MySoundMgr::PlaySE(E_SE no, BOOL loop)
{
	assert(IS_VALID_SE(no));
	if (IS_VALID_SE(no)) {
		m_se[no].Play(loop);
	}
}

// wavファイルの読み込み.
BOOL MySoundMgr::LoadWaveFile(MySound* pMySound, const TCHAR* pSndFile)
{
	if (m_pDSound == NULL) {
		return NULL;
	}

	// wavファイルからデータを読み込む.
	WAVEFORMATEX fmt;
	int dataSize;
	void* pWaveData = ReadWave(pSndFile, &dataSize, &fmt);

	// NULLでなければ読み込み成功、DirectSoundBufferを作成する.
	if (pWaveData) {
		pMySound->m_pSnd = MakeDSB(&fmt, pWaveData, dataSize);
		FreeWave(pWaveData);
		return TRUE;
	}

	// 読み込み失敗.
	return FALSE;
}

// DirectSoundBufferを作成する.
IDirectSoundBuffer* MySoundMgr::MakeDSB(WAVEFORMATEX* pFmt, void* pData, int dataSize)
{
	HRESULT hr = S_OK;

	// セカンダリバッファ（waveデータを保持しておくバッファ）の作成.
	// descはバッファについて説明する変数.
	// なお、プライマリバッファは音声を再生するためのバッファであり.
	// DirectSoundオブジェクト作成時に自動的に作成されている.
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = 0;
	desc.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;// 再生位置取得可能.
	desc.dwFlags |= DSBCAPS_CTRLVOLUME;		// 音量変更可能.
	desc.dwFlags |= DSBCAPS_CTRLPAN;		// パン変更可能.
	desc.dwFlags |= DSBCAPS_GLOBALFOCUS;	// 常に再生可能.
	desc.dwFlags |= DSBCAPS_LOCDEFER;		// バッファをリソースに割り当てる.
	desc.dwBufferBytes = dataSize;			// データサイズ（バイト）.
	desc.lpwfxFormat = pFmt;				// waveフォーマット.

	// サウンドバッファを作成する。作成したバッファは pDSB に格納される.
	IDirectSoundBuffer* pDSB = NULL;
	hr = m_pDSound->CreateSoundBuffer(&desc, &pDSB, NULL);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("サウンドバッファの作成に失敗"), _T("DirectSound"), MB_OK);
		return NULL;
	}

	// waveデータの転送.
	// サウンドバッファに対して、読み込んだデータをコピーする.
	LPVOID ptr1, ptr2;
	DWORD bytes1, bytes2;

	// まずバッファをロックし、メモリアドレスを取得する.
	// ここでコピー先のメモリアドレスが２つになる場合もある（らしい）.
	hr = pDSB->Lock(0, dataSize, &ptr1, &bytes1, &ptr2, &bytes2, 0);
	if (FAILED(hr)) {
		pDSB->Release();
		MessageBox(NULL, _T("サウンドバッファのロックに失敗"), _T("DirectSound"), MB_OK);
		return NULL;
	}

	// ptr1とptr2は別のメモリ領域なので、それぞれにデータをコピーする.
	if (ptr1) {
		CopyMemory(ptr1, pData, bytes1);
	}
	// ptr1にコピーしきれない量をptr2にコピーする.
	if (ptr2) {
		CopyMemory(ptr2, (BYTE*)pData + bytes1, bytes2);
	}

	// アンロックする.
	// サウンドバッファのロック～アンロックは短時間で行う必要がある.
	pDSB->Unlock(ptr1, bytes1, ptr2, bytes2);

	// 作成したDSBを返す.
	return pDSB;
}

// waveファイルを読み込む.
void* MySoundMgr::ReadWave(LPCTSTR pFname, int* pDataSize, WAVEFORMATEX* pFmt)
{
	// wavファイル読み込みのための構造体を定義.
	struct CHK_HD
	{
		char id[4];
		unsigned long size;
	} hd;

	struct WAVE_HD
	{
		char id[4];
	} whd;

	FILE* fp = OpenFile(pFname, _T("rb"));
	if (fp == NULL) {
		MessageBox(NULL, _T("WAVEファイルが開けません"), pFname, MB_OK);
		return NULL;
	}

	// ヘッダチャンクの読みとり。ヘッダチャンクは常に'RIFF'.
	fread(&hd, sizeof(hd), 1, fp);
	if (strncmp(hd.id, "RIFF", 4)) {
		fprintf(stderr, "not RIFF format.\n");
		return NULL;
	}

	// WAVEチャンクの読みとり.
	fread(&whd, sizeof(whd), 1, fp);
	if (strncmp(whd.id, "WAVE", 4)) {
		fprintf(stderr, "not WAVE format.\n");
		return NULL;
	}

	char* pData = NULL;
	*pDataSize = 0;

	// 各チャンクの読みとり.
	bool bLoop = true;
	while (bLoop) {
		// ヘッダチャンクを読みとる.
		size_t readLen = fread(&hd, sizeof(hd), 1, fp);
		if (readLen == 0) {
			bLoop = false;
			continue;
		}
		if (!strncmp(hd.id, "data", 4)) {
			// dataチャンク、hd.sizeにはdataチャンクのバイト数が入っている.
			pData = new char[hd.size];
			*pDataSize = hd.size;
			fread(pData, 1, hd.size, fp);
		}
		else if (!strncmp(hd.id, "fmt ", 4)) {
			// fmtチャンク.
			char* pChank = new char[hd.size];
			fread(pChank, hd.size, 1, fp);
			printf("sizeof(fmt) = %zu, hd.size=%u\n", sizeof(WAVEFORMATEX), hd.size);

			size_t cpySize = (hd.size < sizeof(WAVEFORMATEX) ? hd.size : sizeof(WAVEFORMATEX));
			memcpy(pFmt, pChank, cpySize);

			printf("cbsize = %d\n", pFmt->cbSize);
			delete[] pChank;
		}
		else {
			 // これら以外のチャンクは読み飛ばす.
			fseek(fp, hd.size, SEEK_CUR);
		}
	}
	fclose(fp);
	return pData;// 配列を返す.
}

// ReadWave()で受け取ったポインタを解放する.
void MySoundMgr::FreeWave(void* pWaveData)
{
	if (pWaveData)
	{
		delete[] pWaveData;// ReadWaveでは配列を確保しているので delete[] で.
	}
}
