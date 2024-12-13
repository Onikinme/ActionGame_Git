/*
	myPlayer.cpp

	�v���C���[�̊��N���X
*/
#include "stdafx.h"
#include "myPlayer.h"
#include "myApp.h"
#include "myInput.h"

// �v���C���[�ړ��e�N�X�`���̕\���̈�̂��߂̔z��.
const RECT g_Player_MoveArea[] = {
	{ 0, 32, 32, 64 },   // �ړ��P
	{ 0, 64, 32, 96 },   // �ړ��Q
	{ 0, 96, 32, 128 },  // �ړ��R

    { 0, 0, 32, 32 },    // �~�܂��Ă���Ƃ�

    { 0, 192, 32, 224 }, // �W�����v���̂Ƃ�
};

// �v���C���[�U���e�N�X�`���̕\���̈�̂��߂̔z��.
const RECT g_Player_AttackArea[] = {
	{ 0, 128, 32, 160 }, // �U���P
	{ 0, 160, 32, 192 }, // �U���Q
};

// ����e�N�X�`���̕\���̈�̂��߂̔z��.
const RECT g_WeaponArea[] = {
	{ 0, 0, 48, 48 },    // ����A�j���P
	{ 0, 48, 48, 96 },   // ����A�j���Q
};

// �v���C���[�̏�����.
void Player::Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, float ww, float wh, int maxhp)
{
	m_pos = pos;					  // �v���C���[�̍��W
	m_vel = v_pos;					  // �������ւ̃X�s�[�h ,�W�����v��
	m_w = w, m_h = h;				  // �v���C���[�̃T�C�Y
	m_weapon_w = ww, m_weapon_h = wh; // ����̃T�C�Y
	MaxHP = maxhp;
	HP = MaxHP;
	player_damageflg = false;
	m_isGround = true;				  // ������Ԃł̓v���C���[�͒n�ʂɂ��Ă���
	m_gravity = 0.3f;				  // �d��
	m_timer = 0.0f;					  // �^�C�}�[��0.0f�ɂ��āA�L��������. 
	m_movetimer = 0.0f;				  // �^�C�}�[��0.0f�ɂ��āA�L��������. 
	m_attacktimer = -1.0f;			  // �^�C�}�[��-1.0f�ɂ��āA����������. 
	m_damagetimer = -1.0f;			  // �^�C�}�[��-1.0f�ɂ��āA����������. 
}

// �v���C���[�̈ړ������F���N���X�ł͎�����i�߂邾��.
bool Player::Update(float time)
{
	if (m_timer >= 0.0f) {
		m_timer += time;		// ������i�߂�.
	}

	MyInput* pInput = GetInputInst();

	// �L�[�{�[�h�ł̑���.
	{
		if (!(player_damageflg && m_damagetimer > 0.0f && m_timer - m_damagetimer < 1.0f)) {
			// A�L�[�ō��ɓ���
			if (pInput->IsPushKey(DIK_A) && !pInput->IsPushKey(DIK_D)) {
				m_scaling = true; // ������
				// �󒆂ɂ��āA���E�����ւ̊����������Ă���Ƃ��A
				// �������ɓ����₷���Ȃ�
				if (!m_isGround && 0 < m_vel.x) {
					m_vel.x -= 0.3f;
				}
				else m_vel.x -= 0.1f;
				if (m_vel.x < -5.0f)m_vel.x = -5.0f;
			}
			// D�L�[�ŉE�ɓ���
			else if (pInput->IsPushKey(DIK_D)) {
				m_scaling = false; // �E����
				// �󒆂ɂ��āA���������ւ̊����������Ă���Ƃ��A
				// �E�����ɓ����₷���Ȃ�
				if (!m_isGround && 0 > m_vel.x) {
					m_vel.x += 0.3f;
				}
				else m_vel.x += 0.1f;
				if (m_vel.x > 5.0f)m_vel.x = 5.0f;
			}

			if (pInput->IsPushKey(DIK_A) || pInput->IsPushKey(DIK_D)) {
				if (!m_move) {
					m_movetimer = m_timer;
					m_move = true;
				}
			}
			else {
				m_move = false;
			}

			// �L�[�𗣂������̌�������
			if (!pInput->IsPushKey(DIK_A) && 0 > m_vel.x) {
				if (m_isGround) { // �n��ł̓u���[�L���ǂ�����
					m_vel.x += 0.1f;
					m_vel.x *= 0.95f;
				}
				else { // �󒆂ł͉������Ɋ������������߁A�u���[�L�������Â炢
					m_vel.x += 0.01f;
					m_vel.x *= 0.99f;
				}
				if (m_vel.x > 0) m_vel.x = 0;
			}
			if (!pInput->IsPushKey(DIK_D) && 0 < m_vel.x) {
				if (m_isGround) { // �n��ł̓u���[�L���ǂ�����
					m_vel.x -= 0.1f;
					m_vel.x *= 0.95f;
				}
				else { // �󒆂ł͉������Ɋ������������߁A�u���[�L�������Â炢
					m_vel.x -= 0.01f;
					m_vel.x *= 0.99f;
				}
				if (m_vel.x < 0) m_vel.x = 0;
			}
		}

		// �ړ���
		if ((int)(m_timer - m_movetimer) % 4 >= 3) {
			m_moveanim = 2;
		}
		else if ((int)(m_timer - m_movetimer) % 4 >= 2) {
			m_moveanim = 1;
		}
		else if ((int)(m_timer - m_movetimer) % 4 >= 1) {
			m_moveanim = 0;
		}
		else if ((int)(m_timer - m_movetimer) % 4 >= 0) {
			m_moveanim = 1;
		}

		if (m_pos.x * SIZE < 0) {
			m_pos.x = 0;
			m_vel.x = 0;
		}
		if (m_pos.x > WIDTH - m_w * SIZE) {
			m_pos.x = WIDTH - m_w * SIZE;
			m_vel.x = 0;
		}

		m_pos.x += m_vel.x;

		// �v���C���[���n�ʂɒ����Ă�����SPACE�L�[�ŃW�����v�\
		if (m_isGround) {
			if (pInput->IsPushKey(DIK_SPACE) && m_pos.y == HEIGHT - 64 * SIZE && !m_jumpbutton) {
				m_vel.y = -10.0f;  // �����x�̐ݒ�
				//Y���W�̍X�V
				m_pos.y += m_vel.y;
				m_jumpbutton = true;
				m_isGround = false;
			}
		}
		// �W�����v��
		else {
			//Y�����̑��x�ɉ����x��������
			m_vel.y += m_gravity;
			//Y���W�̍X�V
			m_pos.y += m_vel.y;

			//�㏸���Ƀ{�^���𗣂����Ƃ��̏���
			if (!pInput->IsPushKey(DIK_SPACE))m_shortjump = true;
			if (m_vel.y < 0 && m_shortjump)m_vel.y *= 0.5f;
		}

		// �W�����v�{�^���������ŃW�����v�������Ă��܂�Ȃ��悤�Ƀ`�F�b�N
		if (!pInput->IsPushKey(DIK_SPACE) && m_jumpbutton && m_pos.y == HEIGHT - 64 * SIZE) {
			m_jumpbutton = false;
		}

		// �n�ʂɒ��������`�F�b�N
		if (m_pos.y >= HEIGHT - 64 * SIZE) {
			m_pos.y = HEIGHT - 64 * SIZE;
			m_isGround = true;
			m_shortjump = false;
		}

		// P�L�[�ōU��
		if (pInput->IsPushKey(DIK_P) && !m_attackflg) {
			m_attackflg = true;
			m_attacktimer = m_timer;
		}

		// �U����
		if (m_attackflg && m_attacktimer > 0.0f) {
			if (m_timer - m_attacktimer < 0.5f) { // 0.0f ~ 0.5f
				m_attackanim = 0;
			}
			else if (m_timer - m_attacktimer < 2.5f) { // 0.5f ~ 2.5f
				m_attackanim = 1;
				if (!m_weaponflg)m_weaponflg = true;
				if (m_timer - m_attacktimer < 1.5f) m_weaponanim = 0; // 0.5f ~ 1.5f
				else if (m_timer - m_attacktimer < 2.5f)m_weaponanim = 1; // 1.5f ~ 2.5f
			}
			else if (m_timer - m_attacktimer < 3.0f) { // 2.5f ~ 3.0f
				m_attackanim = 0;
			}
			else { // 3.0f ~
				m_weaponanim = 0;
				m_attacktimer = -2.0f; // �U�����I���������Ƃ�myApp�ɒm�点�邽�߁A
									   // -1.0f�ł͂Ȃ�-2.0f�ŏ���������
				m_weaponflg = false;
			}
		}

		// �U���{�^���������ōU���������Ă��܂�Ȃ��悤�Ƀ`�F�b�N
		if (!pInput->IsPushKey(DIK_P) && m_attackflg && m_attacktimer == -1.0f) {
			m_attackflg = false;
		}
	}

	// ����̍��W�́A�v���C���[�̐i�s�����ɐݒu����
	int scale = 1;
	if (m_scaling)scale = -1;
	m_weapon_pos = D3DXVECTOR2(m_pos.x - (8.0f * SIZE) + 30 * SIZE * scale, m_pos.y - 8.0f * SIZE);

	return true;
}

// �L�����N�^��\������.
void Player::Show() {
	assert(m_timer >= 0.0f);// �^�C�}�[�����̏�ԂŌĂ΂��ׂ��ł͂Ȃ��̂ŃA�T�[�g���d�|���Ă���.
	if (m_timer < 0.0f) {
		return;
	}

	MyApp* myapp = GetApp();
	IDirect3DDevice9* pDev = GetApp()->GetDevice();
	ID3DXSprite* pSpr = GetApp()->GetSprite();

	// �v���C���[�̕`��.
	D3DXVECTOR3 pos(m_pos.x, m_pos.y, 0);
	D3DXVECTOR3 weaponpos(m_weapon_pos.x, m_weapon_pos.y, 0);
	RECT rc = { 0, 0, 0, 0 };
	RECT weaponrc = { 0, 0, 0, 0 };

	// �U����
	if (m_attacktimer > 0.0f) {
		rc = g_Player_AttackArea[m_attackanim];
		weaponrc = g_WeaponArea[m_weaponanim];
		// �v���C���[�̕����`��
		// ���]�ɂ��Ή�
		if (m_weaponflg && m_wTex != NULL)myapp->Draw(pSpr, m_wTex->GetTexture(),
			weaponpos, weaponrc, m_weapon_w, m_weapon_h, m_scaling, false, false);
	}
	// �U�����łȂ��Ƃ�
	else {
		// �n�ʂɒ����Ă���Ƃ�
		if (m_isGround) {
			// �����Ă���Ƃ�
			if (m_vel.x != 0 || m_move) {
				rc = g_Player_MoveArea[m_moveanim];
			}
			// �~�܂��Ă���Ƃ�
			else rc = g_Player_MoveArea[3];
		}
		// �W�����v���̂Ƃ�
		else rc = g_Player_MoveArea[4];
	}
	if (m_pTex != NULL) {
		// �v���C���[��`��
		// ���]�ɂ��Ή�

		bool damageflg = false;
		// �_���[�W���󂯂����̓_��
		if (player_damageflg && m_damagetimer > 0.0f) {
			if (m_timer - m_damagetimer < 0.5f) damageflg = true;
			else if (m_timer - m_damagetimer < 1.0f) {}
			else if (m_timer - m_damagetimer < 1.5f) damageflg = true;
			else if (m_timer - m_damagetimer < 2.0f) {}
			else if (m_timer - m_damagetimer < 2.5f) damageflg = true;
			else if (m_timer - m_damagetimer < 3.0f) {}
			else if (m_timer - m_damagetimer < 3.5f) damageflg = true;
			else if (m_timer - m_damagetimer < 4.0f) {}
			else if (m_timer - m_damagetimer < 4.5f) damageflg = true;
			else {
				m_damagetimer = -1.0f;
				DamageFlg(false);
			}
		}
		myapp->Draw(pSpr, m_pTex->GetTexture(), pos, rc, m_w, m_h, m_scaling, false, damageflg);
	}
}

// ���݂̃v���C���[��XY���W�l�𓾂�.
D3DXVECTOR2 Player::GetXY()
{
	return m_pos;
}

// ���݂̃v���C���[�̃T�C�Y�𓾂�.
float Player::GetSize()
{
	return m_w;
}

// ���݂̕����XY���W�l�𓾂�.
D3DXVECTOR2 Player::GetWeaponXY()
{
	return m_weapon_pos;
}

// ���݂̕���̉����𓾂�.
float Player::GetWeaponW()
{ 
	return m_weapon_w;
}

// ���݂̕���̏c���𓾂�.
float Player::GetWeaponH()
{
	return m_weapon_h;
}

// �ő�HP�𓾂�
int Player::GetMaxHP()
{
	return MaxHP;
}

// ���݂�HP�𓾂�
int Player::GetHP()
{
	return HP;
}

// �_���[�W���󂯂����̏���
void Player::Damage(int damage, bool direct)
{
	m_damagetimer = m_timer;
	if(direct)m_vel.x = -5.0f;
	else m_vel.x = 5.0f;
	HP -= damage;
	if (HP < 0) HP = 0;
}

// �_���[�W�t���O���Ǘ�
void Player::DamageFlg(bool damage)
{
	player_damageflg = damage;
}

// �U���Ń_���[�W�𕉂������H
bool Player::IsDamage()
{
	return player_damageflg;
}

// ������o���Ă��邩�H
bool  Player::IsWeapon()
{
	return m_weaponflg;
}

// �U�����I���������m�点��֐�
bool Player::AttackEnd()
{
	if (m_attacktimer == -2.0f)
	{
		m_attacktimer = -1.0f;
		return true;
	}
	return false;
}