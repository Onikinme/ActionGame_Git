/*
	myPlayer.h

	�v���C���[�̊��N���X
*/
#pragma once

#include "myTexture.h"

// �v���C���[�ړ��e�N�X�`���̕\���̈�̂��߂̔z��.
extern const RECT g_Player_MoveArea[];

// �v���C���[�U���e�N�X�`���̕\���̈�̂��߂̔z��.
extern const RECT g_Player_AttackArea[];

// �v���C���[����e�N�X�`���̕\���̈�̂��߂̔z��.
extern const RECT g_WeaponArea[];

//////////////////////////////////
// Player
// �v���C���[���Ǘ������{�N���X.
class Player
{
private:
	MyTexture* m_pTex;			  // �\������e�N�X�`��.
	MyTexture* m_wTex;			  // �\�����镐��e�N�X�`��.
	D3DXVECTOR2 m_pos;			  // �v���C���[�̍��W
	D3DXVECTOR2 m_vel;			  // �v���C���[�̈ړ����W
	D3DXVECTOR2 m_weapon_pos;	  // �v���C���[�̕���̍��W
	int MaxHP;					  // �v���C���[�̍ő�̗�
	int HP;						  // �v���C���[�̌��ݑ̗�
	float m_timer;				  // �������Ă���̎���.
	float m_movetimer;			  // �ړ�����
	float m_attacktimer;		  // �U������
	float m_w, m_h;				  // �v���C���[�̃T�C�Y
	float m_weapon_w, m_weapon_h; // �v���C���[�̕���̃T�C�Y
	bool player_damageflg;		  // �v���C���[���_���[�W���󂯂����i�����U���ŕ�����_���[�W���󂯂Ȃ��悤�ɂ���j
	bool m_scaling;				  // �X�P�[���itrue�Ȃ獶�����Afalse�Ȃ�E�����j
	bool m_move;				  // ���E�L�[�������Ă���
	bool m_isGround;			  // �n�ʂɒ����Ă��邩�H
	float m_gravity;			  // �d��
	float m_jumpbutton;			  // �W�����v�{�^�������t���O
	float m_shortjump;			  // �W�����v�㏸���ɃW�����v�{�^���𗣂����t���O
	bool m_attackflg;			  // �U�����ł���Ƃ����t���O
	int m_moveanim;				  // �ړ��A�j���[�V����
	int m_attackanim;			  // �U���A�j���[�V����
	int m_weaponanim;			  // ����A�j���[�V����
	bool m_weaponflg;			  // �����\������t���O

public:
// �R���X�g���N�^
	Player(MyTexture* pTex, MyTexture* wTex)
		: m_pTex(pTex)
		, m_wTex(wTex)
		, m_pos(0.0f, 0.0f)
		, m_vel(0.0f, 0.0f)
		, m_weapon_pos(0.0f, 0.0f)
		, MaxHP(0)
		, HP(0)
		, m_timer(-1.0f)
		, m_movetimer(-1.0f)
		, m_attacktimer(-1.0f)
		, m_w(0.0f)
		, m_h(0.0f)
		, m_weapon_w(0.0f)
		, m_weapon_h(0.0f)
		, player_damageflg(false)
		, m_scaling(false)
		, m_move(false)
		, m_isGround(false)
		, m_gravity(0.0f)
		, m_jumpbutton(0.0f)
		, m_shortjump(0.0f)
		, m_attackflg(false)
		, m_moveanim(0)
		, m_attackanim(0)
		, m_weaponanim(0)
		, m_weaponflg(false)
	{
	}

	// �f�X�g���N�^.
	~Player() {}

	// �v���C���[�̏�����.
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, float ww, float wh, int maxhp);

	// �v���C���[�L�����̓���.
	bool Update(float time);

	// �f�o�b�O�p�ɕ�����Ԃ�.
	char Getch() { return '*'; }

	// �L�����N�^��\������.
	void Show();

	// ���݂̃v���C���[��XY���W�l�𓾂�.
	D3DXVECTOR2 GetXY();

	// ���݂̃v���C���[�̃T�C�Y�𓾂�.
	float GetSize();

	// ���݂̕����XY���W�l�𓾂�.
	D3DXVECTOR2 GetWeaponXY();

	// ���݂̕���̉����𓾂�.
	float GetWeaponW();

	// ���݂̕���̏c���𓾂�.
	float GetWeaponH();

	// �ő�HP�𓾂�
	 int GetMaxHP();

	// ���݂�HP�𓾂�
	 int GetHP();

	// �_���[�W���󂯂����̏���
	 void Damage(int damage);

	// �_���[�W�t���O���Ǘ�
	 void DamageFlg(bool damage);

	// �U���Ń_���[�W�𕉂������H
	 bool IsDamage();

	 // ������o���Ă��邩�H
	 bool IsWeapon();

	 // �U�����I���������m�点��֐�
	 bool AttackEnd();

	// �^�C�}�[��0�ȏ�Ȃ�L���i�C�����C���֐��j.
	//virtual bool IsActive();
};