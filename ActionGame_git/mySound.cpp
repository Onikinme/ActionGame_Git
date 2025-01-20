/*
	mySound.cpp

	�T�E���h�����Ǘ�
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

// �Đ��B���ݍĐ����̉��͎~�߂āA�擪����Đ����Ȃ���.
// loop�t���O��true�ɂ���ƁA���[�v�Đ��ƂȂ�.
void MySound::Play(BOOL loop)
{
	if (m_pSnd) {
		m_pSnd->Stop();
		m_pSnd->SetVolume(GAME_VOLUME_MAX);
		m_pSnd->SetCurrentPosition(0);
		m_pSnd->Play(0, 0, (loop ? DSBPLAY_LOOPING : 0));
	}
}

// ��~�B�����I�ɒ�~����.
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
		m_fadeCnt = 60 * 3;// �R�b�Ńt�F�[�h�C������ݒ�.
		m_changeVol = (m_targetVol - m_currentVol) / m_fadeCnt;
	}
}

void MySound::Fadeout()
{
	if (m_pSnd) {
		LONG volume;
		m_pSnd->GetVolume(&volume);

		m_targetVol = GAME_VOLUME_MIN;
		m_fadeCnt = 60 * 3;// �R�b�Ńt�F�[�h�A�E�g����ݒ�.
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

			// ���̍Đ��ɔ����邽�߂�MAX���ʂɖ߂�.
			m_currentVol = GAME_VOLUME_MAX;
			m_pSnd->SetVolume((INT)m_currentVol);
		}
		printf("m_currentVol = %f\n", m_currentVol);
	}
}

////////////////////////// MySoundMgr //////////////////////////
MySoundMgr MySoundMgr::sm_sndMgrObj;// ���̃N���X���g�̗B��̃I�u�W�F�N�g�i���́j�̐錾.

MySoundMgr::~MySoundMgr()
{
	// �f�X�g���N�^���Ăяo�����O��Release()���Ăяo���ĉ�����Ă��Ȃ��ƃA�T�[�g.
	assert(m_pDSound == NULL);
	Release();// �O�̂���Release�Ăяo���͂��Ă���.
}

// DirectSound������������.
HRESULT MySoundMgr::InitSound(HWND hWnd)
{
	HRESULT hr = S_OK;
	assert(m_pDSound == NULL);
	if (m_pDSound == NULL) {
		// DirectSound�̐����B�f�t�H���g�T�E���h�f�o�C�X���w�肷��(������NULL�j.
		hr = DirectSoundCreate(NULL, &m_pDSound, NULL);
		if (FAILED(hr)){
			MessageBox(NULL, _T("DirectSound�̍쐬�Ɏ��s"), _T("DirectSound"), MB_OK);
			return hr;
		}

		assert(m_pDSound != NULL);
		// �������x����ݒ肷��BDSSCL_PRIORITY����������Ă���.
		hr = m_pDSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
		if (FAILED(hr)){
			MessageBox(NULL, _T("SetCooperativeLevel�Ɏ��s�B���̃A�v������L���Ă��܂�"), _T("DirectSound"), MB_OK);
			return hr;
		}
	}
	return S_OK;
}

// Directsound���������.
void MySoundMgr::Release()
{
	// �_�C���N�g�T�E���h�̉���̑O�ɃT�E���h�o�b�t�@��������Ă����Ȃ��Ƃ����Ȃ�.
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

// �T�E���h�f�[�^�ǂݍ���.
void MySoundMgr::LoadSoundData()
{
	// BGM�̓ǂݍ���.
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

	// SE�̓ǂݍ���.
	static const TCHAR* pcszSeFileName[SE_COUNT] = {
		_T("data/se/Space_Gun_01.wav"), // ���ˉ�.
		_T("data/se/bakuha.wav"),		// ���ĉ�.
		_T("data/se/ganx.wav"),			// ��e��.
		_T("data/se/daibakuhatsu.wav"),	// ���S��.
		_T("data/se/S02_AO02.wav"),		// �J�n��.
		_T("data/se/jingle_boss.wav"),	// WARNING��.
		_T("data/se/attack.wav"),		// �U����.
		_T("data/se/attack_hit.wav"),	// �U���q�b�g��.
		_T("data/se/playerattack_hit.wav"),		// �U����.
		_T("data/se/boss1_fire.wav"),	// ����.
		_T("data/se/playerbullet.wav"),		// �U����.
		_T("data/se/playerbullet_hit.wav"),		// �U����.
		_T("data/se/clear.wav"),		// �U����.
		_T("data/se/gameover.wav"),		// �U����.
		#if defined(Shiken5)
		_T("data/se/Clear.wav"),		// �N���A��.
		#endif
	};
	for (int i = SE_BEGIN; i < SE_COUNT; i++) {
		if (!LoadWaveFile(&m_se[i], pcszSeFileName[i])) {
			throw -1;
		}
	}
}

// �a�f�l���Đ�.
void MySoundMgr::PlayBGM(E_BGM no, BOOL loop)
{
	assert(IS_VALID_BGM(no));
	if (IS_VALID_BGM(no)) {
		m_bgm[no].Play(loop);
		m_currentBgm = no;
	}
}

// �a�f�l���t�F�[�h�C��.
void MySoundMgr::FadeinBGM()
{
	assert(IS_VALID_BGM(m_currentBgm));
	if (IS_VALID_BGM(m_currentBgm)) {
		m_bgm[m_currentBgm].Fadein();
	}
}

// �a�f�l���t�F�[�h�A�E�g.
void MySoundMgr::FadeoutBGM()
{
	assert(IS_VALID_BGM(m_currentBgm));
	if (IS_VALID_BGM(m_currentBgm)) {
		m_bgm[m_currentBgm].Fadeout();
	}
}

// �S�a�f�l���~.
void MySoundMgr::StopAllBGM()
{
	for (int i = 0; i < BGM_COUNT; i++) {
		m_bgm[i].Stop();
	}
}

// �S�a�f�l���X�V.
void MySoundMgr::UpdateAllBGM()
{
	for (int i = 0; i < BGM_COUNT; i++) {
		m_bgm[i].UpdateSound();
	}
}

// ���ʉ����Đ�.
void MySoundMgr::PlaySE(E_SE no, BOOL loop)
{
	assert(IS_VALID_SE(no));
	if (IS_VALID_SE(no)) {
		m_se[no].Play(loop);
	}
}

// wav�t�@�C���̓ǂݍ���.
BOOL MySoundMgr::LoadWaveFile(MySound* pMySound, const TCHAR* pSndFile)
{
	if (m_pDSound == NULL) {
		return NULL;
	}

	// wav�t�@�C������f�[�^��ǂݍ���.
	WAVEFORMATEX fmt;
	int dataSize;
	void* pWaveData = ReadWave(pSndFile, &dataSize, &fmt);

	// NULL�łȂ���Γǂݍ��ݐ����ADirectSoundBuffer���쐬����.
	if (pWaveData) {
		pMySound->m_pSnd = MakeDSB(&fmt, pWaveData, dataSize);
		FreeWave(pWaveData);
		return TRUE;
	}

	// �ǂݍ��ݎ��s.
	return FALSE;
}

// DirectSoundBuffer���쐬����.
IDirectSoundBuffer* MySoundMgr::MakeDSB(WAVEFORMATEX* pFmt, void* pData, int dataSize)
{
	HRESULT hr = S_OK;

	// �Z�J���_���o�b�t�@�iwave�f�[�^��ێ����Ă����o�b�t�@�j�̍쐬.
	// desc�̓o�b�t�@�ɂ��Đ�������ϐ�.
	// �Ȃ��A�v���C�}���o�b�t�@�͉������Đ����邽�߂̃o�b�t�@�ł���.
	// DirectSound�I�u�W�F�N�g�쐬���Ɏ����I�ɍ쐬����Ă���.
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = 0;
	desc.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;// �Đ��ʒu�擾�\.
	desc.dwFlags |= DSBCAPS_CTRLVOLUME;		// ���ʕύX�\.
	desc.dwFlags |= DSBCAPS_CTRLPAN;		// �p���ύX�\.
	desc.dwFlags |= DSBCAPS_GLOBALFOCUS;	// ��ɍĐ��\.
	desc.dwFlags |= DSBCAPS_LOCDEFER;		// �o�b�t�@�����\�[�X�Ɋ��蓖�Ă�.
	desc.dwBufferBytes = dataSize;			// �f�[�^�T�C�Y�i�o�C�g�j.
	desc.lpwfxFormat = pFmt;				// wave�t�H�[�}�b�g.

	// �T�E���h�o�b�t�@���쐬����B�쐬�����o�b�t�@�� pDSB �Ɋi�[�����.
	IDirectSoundBuffer* pDSB = NULL;
	hr = m_pDSound->CreateSoundBuffer(&desc, &pDSB, NULL);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("�T�E���h�o�b�t�@�̍쐬�Ɏ��s"), _T("DirectSound"), MB_OK);
		return NULL;
	}

	// wave�f�[�^�̓]��.
	// �T�E���h�o�b�t�@�ɑ΂��āA�ǂݍ��񂾃f�[�^���R�s�[����.
	LPVOID ptr1, ptr2;
	DWORD bytes1, bytes2;

	// �܂��o�b�t�@�����b�N���A�������A�h���X���擾����.
	// �����ŃR�s�[��̃������A�h���X���Q�ɂȂ�ꍇ������i�炵���j.
	hr = pDSB->Lock(0, dataSize, &ptr1, &bytes1, &ptr2, &bytes2, 0);
	if (FAILED(hr)) {
		pDSB->Release();
		MessageBox(NULL, _T("�T�E���h�o�b�t�@�̃��b�N�Ɏ��s"), _T("DirectSound"), MB_OK);
		return NULL;
	}

	// ptr1��ptr2�͕ʂ̃������̈�Ȃ̂ŁA���ꂼ��Ƀf�[�^���R�s�[����.
	if (ptr1) {
		CopyMemory(ptr1, pData, bytes1);
	}
	// ptr1�ɃR�s�[������Ȃ��ʂ�ptr2�ɃR�s�[����.
	if (ptr2) {
		CopyMemory(ptr2, (BYTE*)pData + bytes1, bytes2);
	}

	// �A�����b�N����.
	// �T�E���h�o�b�t�@�̃��b�N�`�A�����b�N�͒Z���Ԃōs���K�v������.
	pDSB->Unlock(ptr1, bytes1, ptr2, bytes2);

	// �쐬����DSB��Ԃ�.
	return pDSB;
}

// wave�t�@�C����ǂݍ���.
void* MySoundMgr::ReadWave(LPCTSTR pFname, int* pDataSize, WAVEFORMATEX* pFmt)
{
	// wav�t�@�C���ǂݍ��݂̂��߂̍\���̂��`.
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
		MessageBox(NULL, _T("WAVE�t�@�C�����J���܂���"), pFname, MB_OK);
		return NULL;
	}

	// �w�b�_�`�����N�̓ǂ݂Ƃ�B�w�b�_�`�����N�͏��'RIFF'.
	fread(&hd, sizeof(hd), 1, fp);
	if (strncmp(hd.id, "RIFF", 4)) {
		fprintf(stderr, "not RIFF format.\n");
		return NULL;
	}

	// WAVE�`�����N�̓ǂ݂Ƃ�.
	fread(&whd, sizeof(whd), 1, fp);
	if (strncmp(whd.id, "WAVE", 4)) {
		fprintf(stderr, "not WAVE format.\n");
		return NULL;
	}

	char* pData = NULL;
	*pDataSize = 0;

	// �e�`�����N�̓ǂ݂Ƃ�.
	bool bLoop = true;
	while (bLoop) {
		// �w�b�_�`�����N��ǂ݂Ƃ�.
		size_t readLen = fread(&hd, sizeof(hd), 1, fp);
		if (readLen == 0) {
			bLoop = false;
			continue;
		}
		if (!strncmp(hd.id, "data", 4)) {
			// data�`�����N�Ahd.size�ɂ�data�`�����N�̃o�C�g���������Ă���.
			pData = new char[hd.size];
			*pDataSize = hd.size;
			fread(pData, 1, hd.size, fp);
		}
		else if (!strncmp(hd.id, "fmt ", 4)) {
			// fmt�`�����N.
			char* pChank = new char[hd.size];
			fread(pChank, hd.size, 1, fp);
			printf("sizeof(fmt) = %zu, hd.size=%u\n", sizeof(WAVEFORMATEX), hd.size);

			size_t cpySize = (hd.size < sizeof(WAVEFORMATEX) ? hd.size : sizeof(WAVEFORMATEX));
			memcpy(pFmt, pChank, cpySize);

			printf("cbsize = %d\n", pFmt->cbSize);
			delete[] pChank;
		}
		else {
			 // �����ȊO�̃`�����N�͓ǂݔ�΂�.
			fseek(fp, hd.size, SEEK_CUR);
		}
	}
	fclose(fp);
	return pData;// �z���Ԃ�.
}

// ReadWave()�Ŏ󂯎�����|�C���^���������.
void MySoundMgr::FreeWave(void* pWaveData)
{
	if (pWaveData)
	{
		delete[] pWaveData;// ReadWave�ł͔z����m�ۂ��Ă���̂� delete[] ��.
	}
}
