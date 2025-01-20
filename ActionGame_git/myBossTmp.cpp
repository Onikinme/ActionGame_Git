#include "stdafx.h"
#include "myBossTmp.h"
#include "myApp.h"

//�{�X�̓���
bool BossTmp::Update(float time)
{
	if (m_timer >= 0.0f) {
		m_timer += time;		// ������i�߂�.
	}
	return true;
}

//�{�X�̈ʒu������
void BossTmp::Init(float posX, float posY, float vx, float vy, float w, int hp)
{
	m_posX = posX; m_posY = posY;
	m_velX = vx; m_velY = vy;
	m_w = w * 1.5f;
	m_hp = hp;
	m_timer = 0.0f;

	// �O��S�B�@�ǉ�
	m_maxhp = hp;
	//m_boss_damageflg = false;
	m_damageflg = 0x00;
}

//�{�X�`��
void BossTmp::Draw()
{
	switch (m_actionNum)
	{
	case 0:
		Move();
		break;
	case 1:
		Attack();
		break;
	case 2:
		Jump();
		break;
	default:

		break;
	}

	D3DXVECTOR3 cnt(32.0f, 32.0f, 0.0f);	// �\�����̒��S�ʒu.
	D3DXVECTOR3 pos(m_posX, m_posY + 1 * SIZE, 0.0f);	// �\���ʒu.
	int time = (int)(m_timer / 10);			// �o�ߎ��Ԃ�10�t���[���P�ʂɊ��Z.
	int anim = time % 8;					// �e�N�X�`���A�j���[�V������8�p�^�[����.
	int size = 64;
	RECT rc = { 0, m_animNum * size + 1, size, (m_animNum + 1) * size };

	D3DXVECTOR2 playerpos;
	float playersize;
	GetApp()->GetPlayerPos(playerpos);
	GetApp()->GetPlayerSize(playersize);

	if (m_posX + m_w / 2 - playerpos.x + playersize / 2 > 0)
	{
		//�v���C���[���{�X��荶��
		m_leftFlg = true;
	}
	else
	{
		m_leftFlg = false;
		//�v���C���[���{�X���E��
		//�摜���]
	}

	if (m_pTex != NULL) {
		bool damageflg = false;
		// �_���[�W���󂯂����̓_��
		if (m_damagetimer > 0.0f) {
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
			}
		}
		GetApp()->Draw(GetApp()->GetSprite(), m_pTex->GetTexture(), pos, rc, size, size, !m_leftFlg, false, damageflg, 1.5f);
	}
}

/********************
*
* �O��S�B�@�폜
* 
* MyApp��Draw�֐��ŉ摜���]�������s���̂ŁASpriteFlip��SpriteReset�͈�U�폜
*
********************/
/*
void BossTmp::SpriteFlip()
{
	���E���]����
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale,
		-1.0f,  // X���X�P�[��
		1.0f,  // Y���X�P�[��
		1.0f);                 // Z���X�P�[�� (�ʏ��1.0f)

	// �ʒu�̕␳ (���]���ɒ��S�������̂�␳)
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation,
		m_posX * 2,
		0,
		0);

	// �����s�� (�X�P�[�� + �ʒu�␳)
	D3DXMATRIX matTransform = matScale * matTranslation;
	// �X�v���C�g�̕ϊ��s���ݒ�
	GetApp()->GetSprite()->SetTransform(&matTransform);
}

void BossTmp::SpriteReset()
{
	// �s������Z�b�g
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	GetApp()->GetSprite()->SetTransform(&matIdentity);
}
*/

// �ʒu(cx, cy)�Ǝ����̈ʒu(posX, posY)�͋���limit2���߂����ǂ���.
// limit2�͋����̓��.
bool BossTmp::Collision(float cx, float cy, float limit2)
{
	return (GetDistance2(cx, cy, m_posX, m_posY) < limit2);
}

//�A�N�V��������
void BossTmp::ChangeAction(int a)
{
		m_actionTimer = m_timer;
		m_actionFlg = true;
		m_actionNum = a;
}

//�U��
void BossTmp::Attack()
{
	// �p����̃N���X�ōs����݌v����
}

//�W�����v(�󒆂�Player��oo�t���[���Ǐ]�㒅�n)
void BossTmp::Jump()
{
	if (m_actionFlg && m_actionTimer > 0.0f)
	{
		float AttckTime = m_timer - m_actionTimer;
		float fXSpeed = 10.0f;
		float fYSpeed = 15.0f;

		//�W�����v����܂ł̗��ߎ���
		if (AttckTime < 10.5f)
		{
			m_animNum = 6;
			if (AttckTime < 9.5f)// 0.0f ~ 2.5f
			{
				m_animNum = 5;
			}
		}
		else
		{
			D3DXVECTOR2 playerpos;
			float playersize;
			GetApp()->GetPlayerPos(playerpos);
			GetApp()->GetPlayerSize(playersize);
			float dx = (playerpos.x + playersize / 2) - (m_posX + m_w / 2);
			float dy = (playerpos.y + playersize / 2) - (m_posY + m_w / 2);
			float  distance = sqrt(dx * dx + dy * dy);

			//40.5f�󒆂ɂ���
			if (AttckTime < 40.5f)
			{


				//����0�܂ŏ㏸
				if (m_posY > 0)
				{
					m_posY -= fYSpeed;
				}
				else
				{
					//����0�܂ŏ㏸��x���ړ�
					//x����ŏd�Ȃ��Ă��Ȃ�
					if (distance > 0)
					{
						float vx = dx / distance;
						float vy = dy / distance;

						m_posX += vx * fXSpeed;
					}
				}

			}
			else
			{
				//�n�ʂɐݒu�����灨�����ύX�̉\������
				if (m_posY <= HEIGHT - 128 * SIZE)
				{
					m_posY += fYSpeed;
				}
				else
				{
					//�A�N�V�����I��
					m_posY = HEIGHT - 128 * SIZE;
					m_actionTimer = -1.0f;
					m_actionFlg = false;
					m_animNum = 0;
					m_actionNum = 0;
				}

			}


		}


	}

}

//�v���C���[�̕����ɕ���
void BossTmp::Move()
{
	D3DXVECTOR2 playerpos;
	float playersize;
	GetApp()->GetPlayerPos(playerpos);
	GetApp()->GetPlayerSize(playersize);
	if (!m_actionFlg)
	{
		float speed = 1.0f;
		float dx = (playerpos.x + playersize / 2) - (m_posX + m_w / 2);
		float dy = (playerpos.y + playersize / 2) - (m_posY + m_w / 2);
		float  distance = sqrt(dx * dx + dy * dy);
		//����0�܂ŏ㏸��x���ړ�
					//x����ŏd�Ȃ��Ă��Ȃ�
		if (distance > 0)
		{

			float vx = dx / distance;
			float vy = dy / distance;

			m_posX += vx * speed;

			int time = (int)(m_timer / 2);			// �o�ߎ��Ԃ�10�t���[���P�ʂɊ��Z.
			int anim = time % 2 + 1;					// �e�N�X�`���A�j���[�V������8�p�^�[����.
			m_animNum = anim;
		}
	}
}

/********************
*
* �O��S�B�@�ǉ�
* 
* �{�X�ɍU���𓖂Ă��Ƃ��̏���
*
********************/
// ���݂�XY���W�l�𓾂�.
D3DXVECTOR2 BossTmp::GetXY()
{
	// ���K�Q
	// TODO �����o�ϐ��̒l���擾���鏈�����L�q���Ă�������.
	return D3DXVECTOR2(m_posX, m_posY);
}

// ���݂̃{�X�̃T�C�Y�𓾂�.
float BossTmp::GetSize() {
	return m_w;
}

// �ő�HP�𓾂�
int BossTmp::GetMaxHP() {
	return m_maxhp;
}

// ���݂�HP�𓾂�
int BossTmp::GetHP() {
	return m_hp;
}

// �_���[�W���󂯂����̏���
void BossTmp::Damage(int damage)
{
	m_damagetimer = m_timer;
	m_hp -= damage;
}

// �_���[�W�t���O���Ǘ�
void BossTmp::DamageFlg(unsigned char damage)
{
	//m_boss_damageflg = damage;
	m_damageflg |= damage;
}

// �_���[�W�t���O�����Z�b�g
void BossTmp::DamageReset(unsigned char damage) {
	m_damageflg &= damage;
}

// �U���Ń_���[�W�𕉂������H
unsigned char BossTmp::IsDamage()
{
	return m_damageflg;
}



// ���̋󂫈ʒu��T��.
int BossManager::Next()
{
	// �����O�o�b�t�@���A���݈ʒu(m_index)�������T��.
	for (int i = 0; i < m_bossMax; i++) {
		// ���݈ʒu(m_index)����󂫂�����.
		int index = (m_index + i) % m_bossMax;
		// index ��m_enemyMax�ȏ�ɂȂ�Ȃ��悤�A��]�ɂ��邱�ƂŔz���.
		// ���݈ʒu���� m_index - 1 �̈ʒu�܂ł�����.
		if (m_ppBuffer[index] == NULL) {
			m_index = (index + 1) % m_bossMax;
			return index;
		}
	}
	return -1;
}

// �f�X�g���N�^:�z���������Ă���.
// �z��̒��g���܂�������Ă���S�̂����.
BossManager::~BossManager()
{
	for (int i = 0; i < m_bossMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			delete m_ppBuffer[i];
		}
	}
	delete[] m_ppBuffer;
	delete[] m_pTextBuff;
}

// �������̊m�ۓ��̏�����.
bool BossManager::Init()
{
	if (m_ppBuffer == NULL) {
		m_ppBuffer = new BossTmp * [m_bossMax];	// �z��̒��g��EnemyBase*�^.
		// ��sizeof(EnemyBase*) �� max ���̃��������m�ۂ��Ă��邾���ŁA�R���X�g���N�^�͌Ăяo����Ȃ�.
		// sizeof(EnemyBase*)��x64�ł͂P������8�o�C�g�Ax86�ł͂P������4�o�C�g�̃������T�C�Y.
		if (m_ppBuffer == NULL) {
			return false;
		}

		for (int i = 0; i < m_bossMax; i++) {
			m_ppBuffer[i] = NULL;		// �|�C���^��NULL�ŏ��������A���g�p��ԂƂ���.
		}
	}

	if (m_pTextBuff == NULL) {
		m_pTextBuff = new char[m_bossMax + 1];	// �f�o�b�O�p��������m�ۂ���.
		// +1 �͏I�[�L��'\0'�̕�.
		if (m_pTextBuff == NULL) {
			return false;
		}
	}

	return true;
}

// �z��̑S�Ă̗v�f�����Z�b�g����.
void BossManager::ResetAll()
{
	for (int i = 0; i < m_bossMax; i++) {
		if(m_ppBuffer[i] != NULL)Damage(9999, i);
	}
}
// pEnemy��z��ɒǉ�.
// EnemyBase*�^�������ɂ��Ă��邪�A���ۂɂ�EnemyAAA*�^��EnemyBBB*�^�������Ă���.
// �����̃N���X�́AEnemyBase�̔h���N���X�Ȃ̂ŁAEnemyBase*�^�Ŏ󂯂邱�Ƃ��ł���.
// �Ԉ���Ĉ�����(EnemyBase pEnemy) �Ɓ���t���Y��A�|�C���^�ł͂Ȃ����̂ŋL�q�����.
// �f�[�^���ꕔ�������錻�ۂ��������[���ȃo�O�̌����ƂȂ�.
bool BossManager::Add(BossTmp* pEnemy)
{
	int index = Next();		// �z�񂩂�󂫂�T��.
	if (0 <= index && index < m_bossMax) {
		m_ppBuffer[index] = pEnemy;// �G���Ǘ��p�̔z��ɓo�^.
		return true;
	}
	return false;
}

// �z��̊e�v�f���ׂĂɁA������i�߂�悤�w������.
void BossManager::Update(float time)
{
	for (int i = 0; i < m_bossMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			bool flag = m_ppBuffer[i]->Update(time);
			// FLASE���Ԃ�Ƃ��̗v�f�͕s�v�ɂȂ�.
			if (!flag) {
				Damage(9999, i);
			}
		}
	}
}

// �G�Ƀ_���[�W��^����
// �����G��HP��0�ɂȂ�����A�G�z���index�Ԗڂ𖳌��ɂ���.
// m_ppBuffer[index]�͊�{�N���X�̃|�C���^�����A�f�X�g���N�^��virtual�w�肵�Ă��邽��.
// �K�؂Ȕh���N���X(EnemyAAA��EnemyBBB)�̃f�X�g���N�^���Ăяo�����.
void BossManager::Damage(int damage, int index)
{
	if (0 <= index && index < m_bossMax) {
		m_ppBuffer[index]->Damage(damage);
		if (GetHP(&index) <= 0) {
			delete m_ppBuffer[index];
			m_ppBuffer[index] = NULL;// �z��̒��g��NULL�N���A���Ď��̎g�p�ɔ�����.
		}
	}
}

// �z��̑S�Ă̓G�ɑ΂��āA�\���𑣂�.
void BossManager::Show()
{
	for (int i = 0; i < m_bossMax; i++) {
		// m_ppBuffer[i]�̌^��EnemyBase*�����AShow �� virtual�֐��Ȃ̂�.
		// �h���N���X��show���K�؂ɌĂяo�����.
		// �܂�if��switch���g���ĕ��򂳂���K�v���Ȃ��B���ꂪ�I�u�W�F�N�g�w���̗��_�̂P��.
		if (m_ppBuffer[i] != NULL) {
			m_ppBuffer[i]->Draw();
		}
	}
}

// ���݂�XY���W�l�𓾂�.
D3DXVECTOR2 BossManager::GetXY(int* index) {
	return m_ppBuffer[*index]->GetXY();
}

// ���݂̃{�X�̃T�C�Y�𓾂�.
float BossManager::GetSize(int* index) {
	return m_ppBuffer[*index]->GetSize();
}

// �ő�HP�𓾂�
int BossManager::GetMaxHP(int* index) {
	return m_ppBuffer[*index]->GetMaxHP();
}

// ���݂�HP�𓾂�
int BossManager::GetHP(int* index) {
	return m_ppBuffer[*index]->GetHP();
}

// �_���[�W�t���O���Ǘ�
void BossManager::DamageFlg(unsigned char damage, int* index) {
	m_ppBuffer[*index]->DamageFlg(damage);
}

// �_���[�W�t���O�����Z�b�g
void BossManager::DamageReset(unsigned char damage, int* index) {
	m_ppBuffer[*index]->DamageReset(damage);
}

unsigned char BossManager::IsDamage(int* index) {
	return m_ppBuffer[*index]->IsDamage();
}

// �z��̑傫����Ԃ�
int BossManager::BossMax()
{
	return m_bossMax;
}

// ���̔z�񂪎g�p�\��
bool BossManager::PPBuffer(int index) {
	if (m_ppBuffer[index] != NULL) {
		return true;
	}
	return false;
}

// (cx, cy)�̈ʒu�͓G�ƏՓˋ�����������.
// �Փ˂����ꍇ�͈���pIndex�������ꏊ�ɔz��̃C���f�b�N�X������TRUE��Ԃ�.
// �Փ˂����������ꍇ��FALSE�Ԃ�.
bool BossManager::Collision(float cx, float cy, float limit2, int* pIndex)
{
	for (int i = 0; i < m_bossMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			if (m_ppBuffer[i]->Collision(cx, cy, limit2)) {
				*pIndex = i;
				return true;
			}
		}
	}
	return false;
}

//�A�N�V��������
void BossManager::ChangeAction(int a, int index) {
	m_ppBuffer[index]->ChangeAction(a);
}

// �f�o�b�O�p��m_pBuffer�z��̒��g���e�L�X�g������.
// �e�ۂ��L���F*.
// �e�ۂ������F-.
char* BossManager::GetText()
{
	for (int i = 0; i < m_bossMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			m_pTextBuff[i] = m_ppBuffer[i]->Getch();
		}
		else {
			m_pTextBuff[i] = '-';
		}
	}
	// ������̍Ō�͕K��NULL�����Ń^�[�~�l�[�g.
	m_pTextBuff[m_bossMax] = NULL;
	return m_pTextBuff;
}