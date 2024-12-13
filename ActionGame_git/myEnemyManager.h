/*
	myEnemyManager.h

	�G�̊��N���X�ƊǗ��N���X
*/
#pragma once

#include "myTexture.h"

//////////////////////////////////
// EnemyBase
// �G�P�̂��Ǘ������{�N���X.
class EnemyBase
{
protected:
	// �ȉ��̕ϐ��́u�h���N���X�v����p�ɂɃA�N�Z�X����邽��.
// private�ł͂Ȃ�protected�Ɏw�肵�Ă���.
	MyTexture* m_pTex;		// �\������e�N�X�`��.
	D3DXVECTOR2 m_pos;		// �G�̍��W
	D3DXVECTOR2 m_vel;		// �G�̈ړ����W
	float m_w, m_h;				  // �G�̃T�C�Y
	int MaxHP;				// �G�̍ő�̗�
	int HP;					// �G�̌��ݑ̗�
	float m_timer;			// �������Ă���̎���.
	bool enemy_damageflg;	// �G���_���[�W���󂯂����i�����U���ŕ�����_���[�W���󂯂Ȃ��悤�ɂ���j

	// ���̃N���X�͒��ۃN���X�Ȃ̂ŁA�h���N���X�ȊO�����new�ł��Ȃ��悤�ɂ���.
	// ���̂��߁A�R���X�g���N�^��protected�G���A�ɋL�q����.
	// �R���X�g���N�^
	EnemyBase(MyTexture* pTex)
		: m_pTex(pTex)
		, m_pos(0.0f, 0.0f)
		, m_vel(0.0f, 0.0f)
		, MaxHP(0)
		, HP(0)
		, m_timer(-1.0f)
		, enemy_damageflg(false)
	{
	}

public:

	// �f�X�g���N�^�́A�q�ȊO������A�N�Z�X�����\�������邽�߁upublic�v.
// �h�����Ƃ��Đ݌v���Ă���̂Ńf�X�g���N�^��virtual�w�肵�Ă���.
	// �f�X�g���N�^.
	virtual ~EnemyBase() {}

	// �G�L�����̓���.
	virtual bool Update(float time);

	// �f�o�b�O�p�ɕ�����Ԃ�.
	virtual char Getch() { return '*'; }

	// �G�̏�����.
	virtual void Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, int maxhp);

	// �L�����N�^��\������.
	virtual void Show();

	// ���݂�XY���W�l�𓾂�.
	//void GetXY(float& x, float& y);
	virtual D3DXVECTOR2 GetXY();

	// ���݂̃v���C���[�̃T�C�Y�𓾂�.
	virtual float GetSize();

	// �ő�HP�𓾂�
	virtual int GetMaxHP();

	// ���݂�HP�𓾂�
	virtual int GetHP();

	// �_���[�W���󂯂����̏���
	virtual void Damage(int damage);

	// �_���[�W�t���O���Ǘ�
	virtual void DamageFlg(bool damage);

	// �U���Ń_���[�W�𕉂������H
	virtual bool IsDamage();

	// �^�C�}�[��0�ȏ�Ȃ�L���i�C�����C���֐��j.
	//virtual bool IsActive();

	// �ʒu(cx,cy)�Ƃ̋�����limit2�ȉ����ǂ����𔻒�.
	virtual bool Collision(float cx, float cy, float limit2);
};

//////////////////////////////////
// EnemyManager
// �G�S�̂��Ǘ�����.
class EnemyManager
{
private:
	EnemyBase** m_ppBuffer;	// �G�̔z��A�����O�o�b�t�@�Ƃ��Ďg��.
	int m_enemyMax;			// �z��̍ő�l.
	int m_index;			// �z��̎g�p���݈ʒu.
	char* m_pTextBuff;		// �f�o�b�O�p�ɔz��̏󋵂𕶎���ɂ���.

	int Next();				// ���̈ʒu���m�ۂ���i�������炵���R�[�����Ȃ��̂�private�֐��Ƃ���j.

public:
	EnemyManager(int max)
		: m_ppBuffer(NULL)
		, m_enemyMax(max)
		, m_index(0)
		, m_pTextBuff(NULL)
	{}

	~EnemyManager();
	//EnemyBase* pNext(); // ���̋󂫈ʒu��Ԃ�.
	/*
	EnemyBase* pGet(int ix)
	{
		return m_pBuffer + ix; // &(m_pBuffer[ix]) �Ɠ����Ӗ�.
	}
	*/

	bool Init();			// �������̊m�ۓ��̏�����.
	void ResetAll();		// �z��̑S�Ă̗v�f�����Z�b�g����.
	bool Add(EnemyBase*);	// �z��ւ̓G�L�����̒ǉ�.
	void Update(float t);	// �G�z��S�̂��X�V.
	// void Purge(int index);	// �z�񂩂�ʒuindex�̓G����菜��.
	void Show();			// �G�L�����N�^��\������.

	// ���݂�XY���W�l�𓾂�.
	D3DXVECTOR2 GetXY(int* index);

	// ���݂̃v���C���[�̃T�C�Y�𓾂�.
	float GetSize();

	// �ő�HP�𓾂�
	int GetMaxHP(int* index);

	// ���݂�HP�𓾂�
	int GetHP(int* index);

	// �_���[�W���󂯂����̏���
	void Damage(int damage, int index);

	// �_���[�W�t���O���Ǘ�
	void DamageFlg(bool damage, int index);

	// �U���Ń_���[�W�𕉂������H
	bool IsDamage(int index);

	// �z��̑傫����Ԃ�
	int EnemyMax();

	// ���̔z�񂪎g�p�\��
	bool PPBuffer(int index);

	char* GetText();				// �f�o�b�O�p�ɔz��̏󋵂𕶎��񉻂���.

	bool Collision(float cx, float cy, float limit2, int* pIndex);
	// ���ʒu(cx, cy)����݂ċ���limit2�ȓ��̓G��pIndex�ɕԋp.
};