/*
	myFloorManager.h

	���̊��N���X�ƊǗ��N���X
*/
#pragma once

#include "myTexture.h"

//Floor�̊��N���X
class FloorBase
{
protected:
	// �ȉ��̕ϐ��́u�h���N���X�v����p�ɂɃA�N�Z�X����邽��.
// private�ł͂Ȃ�protected�Ɏw�肵�Ă���.
	MyTexture* m_pTex;		// �\������e�N�X�`��.
	D3DXVECTOR2 m_pos;		// ���̍��W
	D3DXVECTOR2 m_vel;		// ���̈ړ����W
	float m_w, m_h;				  // ���̃T�C�Y
	float m_timer;			// �������Ă���̎���.
	bool floorflg; // �v���C���[�����̏��̏�ɂ���

	// ���̃N���X�͒��ۃN���X�Ȃ̂ŁA�h���N���X�ȊO�����new�ł��Ȃ��悤�ɂ���.
	// ���̂��߁A�R���X�g���N�^��protected�G���A�ɋL�q����.
	// �R���X�g���N�^
	FloorBase(MyTexture* pTex)
		: m_pTex(pTex)
		, m_pos(0.0f, 0.0f)
		, m_vel(0.0f, 0.0f)
		, m_w(0.0f), m_h(0.0f)
		, m_timer(-1.0f)
		, floorflg(false)
	{
	}

public:

	// �f�X�g���N�^�́A�q�ȊO������A�N�Z�X�����\�������邽�߁upublic�v.
// �h�����Ƃ��Đ݌v���Ă���̂Ńf�X�g���N�^��virtual�w�肵�Ă���.
	// �f�X�g���N�^.
	virtual ~FloorBase() {}

	// ���L�����̓���.
	virtual bool Update(float time);

	// �f�o�b�O�p�ɕ�����Ԃ�.
	virtual char Getch() { return '*'; }

	// ���̏�����.
	virtual void Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h);

	// �L�����N�^��\������.
	virtual void Show();

	// ���݂�XY���W�l�𓾂�.
	virtual D3DXVECTOR2 GetXY();

	// ���݂̏��̃T�C�Y�𓾂�.
	virtual float GetSize();

	// �ʒu(cx,cy)�Ƃ̋�����limit2�ȉ����ǂ����𔻒�.
	virtual bool Collision(float cx, float cy, float limit2);
};

//////////////////////////////////
// FloorManager
// ���S�̂��Ǘ�����.
class FloorManager
{
private:
	FloorBase** m_ppBuffer;	// ���̔z��A�����O�o�b�t�@�Ƃ��Ďg��.
	int m_floorMax;			// �z��̍ő�l.
	int m_index;			// �z��̎g�p���݈ʒu.
	char* m_pTextBuff;		// �f�o�b�O�p�ɔz��̏󋵂𕶎���ɂ���.

	int Next();				// ���̈ʒu���m�ۂ���i�������炵���R�[�����Ȃ��̂�private�֐��Ƃ���j.

public:
	FloorManager(int max)
		: m_ppBuffer(NULL)
		, m_floorMax(max)
		, m_index(0)
		, m_pTextBuff(NULL)
	{
	}

	~FloorManager();

	bool Init();			// �������̊m�ۓ��̏�����.
	void ResetAll();		// �z��̑S�Ă̗v�f�����Z�b�g����.
	bool Add(FloorBase*);	// �z��ւ̓G�L�����̒ǉ�.
	void Update(float t);	// �G�z��S�̂��X�V.
	void Show();			// �G�L�����N�^��\������.

	// ���݂�XY���W�l�𓾂�.
	D3DXVECTOR2 GetXY(int* index);

	// ���݂̏��̃T�C�Y�𓾂�.
	float GetSize(int* index);

	// �z��̑傫����Ԃ�
	int FloorMax();

	// ���̔z�񂪎g�p�\��
	bool PPBuffer(int index);

	char* GetText();				// �f�o�b�O�p�ɔz��̏󋵂𕶎��񉻂���.

	bool Collision(float cx, float cy, float limit2, int* pIndex);
	// ���ʒu(cx, cy)����݂ċ���limit2�ȓ��̓G��pIndex�ɕԋp.
};