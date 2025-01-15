#pragma once

#include "myTexture.h"

//Boss�̊��N���X
class BossTmp
{
protected:
	MyTexture* m_pTex;
	float m_posX, m_posY;	// ���݈ʒu.
	float m_velX, m_velY;	// XY�����ւ̑��x����.
	float m_w;         // �{�X�̃T�C�Y
	float m_timer;			// �������Ă���̎���.
	int   m_animNum;        //�{�X�̃A�j���[�V�����̓Y����

	// �O��S�B�@�ǉ�
	int   m_maxhp;          //  �ő�ϋv�l
	//bool  m_boss_damageflg;	// �{�X���_���[�W���󂯂����i�����U���ŕ�����_���[�W���󂯂Ȃ��悤�ɂ���j
	unsigned char m_damageflg; // �{�X���_���[�W���󂯂����i�r�b�g���Z�ŊǗ�����j

	int   m_hp;             //  �ϋv�l
	float m_actionTimer;    //�U�����̎���	
	bool  m_actionFlg;      //�s�������Ă��邩�ǂ����̃t���O
	int   m_actionNum;      //�s���p�^�[���̓Y����
	bool  m_leftFlg;         //True:�v���C���[�̓{�X�̍��� False:�v���C���[�̓{�X�̉E��

	// ���̃N���X�͒��ۃN���X�Ȃ̂ŁA�h���N���X�ȊO�����new�ł��Ȃ��悤�ɂ���.
	// ���̂��߁A�R���X�g���N�^��protected�G���A�ɋL�q����.
	// �R���X�g���N�^
	BossTmp(MyTexture* pTex)
		: m_pTex(pTex)
		, m_posX(0.0f)
		, m_posY(0.0f)
		, m_velX(0.0f)
		, m_velY(0.0f)
		, m_w(0.0f)
		, m_timer(0.0f)
		, m_animNum(0)
		, m_hp(0)
		, m_actionTimer(-1.0f)
		, m_actionFlg(false)
		, m_actionNum(0)
		, m_leftFlg(true)

		// �O��S�B�@�ǉ�
		, m_maxhp(0)
		//, m_boss_damageflg(false)
		, m_damageflg(0x00)
	{
	}

public:

	// �f�X�g���N�^�́A�q�ȊO������A�N�Z�X�����\�������邽�߁upublic�v.
	// �h�����Ƃ��Đ݌v���Ă���̂Ńf�X�g���N�^��virtual�w�肵�Ă���.
	// �f�X�g���N�^.
	virtual ~BossTmp()
	{
	}

	//�{�X�̓���
	virtual bool Update(float time);
	//�{�X�̈ʒu������
	virtual void Init(float posX, float posY, float vx, float vy, float w, int hp);
	//�{�X�`��
	virtual virtual void Draw();
	virtual void Deactive()			// �����ɂ���.
	{
		m_timer = -1.0f;
	}

	/********************
	*
	* �O��S�B�@�폜
	*
	* MyApp��Draw�֐��ŉ摜���]�������s���̂ŁASpriteFlip��SpriteReset�͈�U�폜
	*
	********************/
	/*
	//�摜���]
	virtual void SpriteFlip();
	//�摜�̍s��̃��Z�b�g
	virtual void SpriteReset();
	*/

	//�����蔻��
	virtual bool Collision(float cx, float cy, float limit2);
	//�A�N�V��������
	virtual void ChangeAction(int a);
	//�U��
	virtual void Attack();
	//�W�����v(�󒆂�Player��oo�t���[���Ǐ]�㒅�n)
	virtual void Jump();
	//�v���C���[�̕����ɕ���
	virtual void Move();

	/********************
	* 
	* �O��S�B�@�ǉ�
	* 
	* �{�X�ɍU���𓖂Ă��Ƃ��̏���
	* 
	********************/
	// ���݂�XY���W�l�𓾂�.
	virtual D3DXVECTOR2 GetXY();

	// ���݂̃{�X�̃T�C�Y�𓾂�.
	virtual float GetSize();

	// �ő�HP�𓾂�
	virtual int GetMaxHP();

	// ���݂�HP�𓾂�
	virtual int GetHP();

	// �_���[�W���󂯂����̏���
	virtual void Damage(int damage);

	// �_���[�W�t���O���Ǘ�
	virtual void DamageFlg(unsigned char damage);

	// �_���[�W�t���O�����Z�b�g
	virtual void DamageReset(unsigned char damage);

	// �U���Ń_���[�W�𕉂������H
	virtual unsigned char IsDamage();

	// �f�o�b�O�p�ɕ�����Ԃ�.
	virtual char Getch() { return '*'; }
};

//////////////////////////////////
// BossManager
// �{�X�S�̂��Ǘ�����.
class BossManager
{
private:
	BossTmp** m_ppBuffer;	// �{�X�̔z��A�����O�o�b�t�@�Ƃ��Ďg��.
	int m_bossMax;			// �z��̍ő�l.
	int m_index;			// �z��̎g�p���݈ʒu.
	char* m_pTextBuff;		// �f�o�b�O�p�ɔz��̏󋵂𕶎���ɂ���.

	int Next();				// ���̈ʒu���m�ۂ���i�������炵���R�[�����Ȃ��̂�private�֐��Ƃ���j.

public:
	BossManager(int max)
		: m_ppBuffer(NULL)
		, m_bossMax(max)
		, m_index(0)
		, m_pTextBuff(NULL)
	{
	}

	~BossManager();

	bool Init();			// �������̊m�ۓ��̏�����.
	void ResetAll();		// �z��̑S�Ă̗v�f�����Z�b�g����.
	bool Add(BossTmp*);		// �z��ւ̃{�X�L�����̒ǉ�.
	void Update(float t);	// �{�X�z��S�̂��X�V.
	void Show();			// �{�X�L�����N�^��\������.

	// ���݂�XY���W�l�𓾂�.
	D3DXVECTOR2 GetXY(int* index);

	// ���݂̃{�X�̃T�C�Y�𓾂�.
	float GetSize(int* index);

	// �ő�HP�𓾂�
	int GetMaxHP(int* index);

	// ���݂�HP�𓾂�
	int GetHP(int* index);

	// �_���[�W���󂯂����̏���
	void Damage(int damage, int index);

	// �_���[�W�t���O���Ǘ�
	void DamageFlg(unsigned char damage, int* index);

	// �_���[�W�t���O�����Z�b�g
	void DamageReset(unsigned char damage, int* index);

	// �U���Ń_���[�W�𕉂������H
	unsigned char IsDamage(int* index);

	// �z��̑傫����Ԃ�
	int BossMax();

	// ���̔z�񂪎g�p�\��
	bool PPBuffer(int index);

	bool Collision(float cx, float cy, float limit2, int* pIndex);
	// ���ʒu(cx, cy)����݂ċ���limit2�ȓ��̓G��pIndex�ɕԋp.

	//�A�N�V��������
	void ChangeAction(int a, int index);

	char* GetText();
};