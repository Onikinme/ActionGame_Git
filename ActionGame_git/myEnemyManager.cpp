/*
	myEnemyManager.cpp

	�G�̊��N���X�ƊǗ��N���X
*/
#include "stdafx.h"
#include "myEnemyManager.h"
#include "myApp.h"

// �G�̈ړ������F���N���X�ł͎�����i�߂邾��.
bool EnemyBase::Update(float time)
{
	if (m_timer >= 0.0f) {
		m_timer += time;		// ������i�߂�.
	}
	return true;
}

// �G�̏�����.
void EnemyBase::Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, int maxhp)
{
	// x (�����ʒuX), y (�����ʒuY)
	m_pos = pos;
	m_vel = v_pos;
	m_w = w, m_h = h;				  // �G�̃T�C�Y
	MaxHP = maxhp;
	HP = MaxHP;
	enemy_damageflg = false;
	m_timer = 0.0f;	// �^�C�}�[��0.0f�ɂ��āA�L��������. 
}

void EnemyBase::Show()
{
	assert(m_timer >= 0.0f);// �^�C�}�[�����̏�ԂŌĂ΂��ׂ��ł͂Ȃ��̂ŃA�T�[�g���d�|���Ă���.
	if (m_timer < 0.0f) {
		return;
	}

	D3DXVECTOR3 pos(m_pos.x, m_pos.y, 0);
	MyApp* myapp = GetApp();
	ID3DXSprite* pSpr = GetApp()->GetSprite();

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
		myapp->Draw(pSpr, m_pTex->GetTexture(), pos, { 0, 0, (long)m_w, (long)m_h }, m_w, m_h, false, false, damageflg, 1);
	}
}

// ���݂�XY���W�l�𓾂�.
D3DXVECTOR2 EnemyBase::GetXY()
{
	// ���K�Q
	// TODO �����o�ϐ��̒l���擾���鏈�����L�q���Ă�������.
	return m_pos;
}

// ���݂̓G�̃T�C�Y�𓾂�.
float EnemyBase::GetSize()
{
	return m_w;
}

// �ő�HP�𓾂�
int EnemyBase::GetMaxHP() {
	return MaxHP;
}

// ���݂�HP�𓾂�
int EnemyBase::GetHP() {
	return HP;
}

// �_���[�W���󂯂����̏���
void EnemyBase::Damage(int damage)
{
	m_damagetimer = m_timer;
	HP -= damage;
}

// �_���[�W�t���O���Ǘ�
void EnemyBase::DamageFlg(unsigned char damage)
{
	enemy_damageflg |= damage;
}

// �_���[�W�t���O�����Z�b�g
void EnemyBase::DamageReset(unsigned char damage) {
	enemy_damageflg &= damage;
}

// �U���Ń_���[�W�𕉂������H
unsigned char EnemyBase::IsDamage()
{
	return enemy_damageflg;
}

/*
// �^�C�}�[��0�ȏ�Ȃ�L���i�C�����C���֐��j.
bool EnemyBase::IsActive()
{
	// ���K�Q
	// TODO
	return (m_timer >= 0.0f);
}
*/

// �ʒu(cx, cy)�Ǝ����̈ʒu(posX, posY)�͋���limit2���߂����ǂ���.
// limit2�͋����̓��.
bool EnemyBase::Collision(float cx, float cy, float limit2)
{
	return (GetDistance2(cx, cy, m_pos.x, m_pos.y) < limit2);
}

// ���̋󂫈ʒu��T��.
int EnemyManager::Next()
{
	// �����O�o�b�t�@���A���݈ʒu(m_index)�������T��.
	for (int i = 0; i < m_enemyMax; i++) {
		// ���݈ʒu(m_index)����󂫂�����.
		int index = (m_index + i) % m_enemyMax;
		// index ��m_enemyMax�ȏ�ɂȂ�Ȃ��悤�A��]�ɂ��邱�ƂŔz���.
		// ���݈ʒu���� m_index - 1 �̈ʒu�܂ł�����.
		if (m_ppBuffer[index] == NULL) {
			m_index = (index + 1) % m_enemyMax;
			return index;
		}
	}
	return -1;
}

// �f�X�g���N�^:�z���������Ă���.
// �z��̒��g���܂�������Ă���S�̂����.
EnemyManager::~EnemyManager()
{
	for (int i = 0; i < m_enemyMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			delete m_ppBuffer[i];
		}
	}
	delete[] m_ppBuffer;
	delete[] m_pTextBuff;
}

// �������̊m�ۓ��̏�����.
bool EnemyManager::Init()
{
	if (m_ppBuffer == NULL) {
		m_ppBuffer = new EnemyBase * [m_enemyMax];	// �z��̒��g��EnemyBase*�^.
		// ��sizeof(EnemyBase*) �� max ���̃��������m�ۂ��Ă��邾���ŁA�R���X�g���N�^�͌Ăяo����Ȃ�.
		// sizeof(EnemyBase*)��x64�ł͂P������8�o�C�g�Ax86�ł͂P������4�o�C�g�̃������T�C�Y.
		if (m_ppBuffer == NULL) {
			return false;
		}

		for (int i = 0; i < m_enemyMax; i++) {
			m_ppBuffer[i] = NULL;		// �|�C���^��NULL�ŏ��������A���g�p��ԂƂ���.
		}
	}

	if (m_pTextBuff == NULL) {
		m_pTextBuff = new char[m_enemyMax + 1];	// �f�o�b�O�p��������m�ۂ���.
		// +1 �͏I�[�L��'\0'�̕�.
		if (m_pTextBuff == NULL) {
			return false;
		}
	}

	return true;
}

// �z��̑S�Ă̗v�f�����Z�b�g����.
void EnemyManager::ResetAll()
{
	for (int i = 0; i < m_enemyMax; i++) {
		delete m_ppBuffer[i];
		m_ppBuffer[i] = NULL;// �z��̒��g��NULL�N���A���Ď��̎g�p�ɔ�����.
	}
}
// pEnemy��z��ɒǉ�.
// EnemyBase*�^�������ɂ��Ă��邪�A���ۂɂ�EnemyAAA*�^��EnemyBBB*�^�������Ă���.
// �����̃N���X�́AEnemyBase�̔h���N���X�Ȃ̂ŁAEnemyBase*�^�Ŏ󂯂邱�Ƃ��ł���.
// �Ԉ���Ĉ�����(EnemyBase pEnemy) �Ɓ���t���Y��A�|�C���^�ł͂Ȃ����̂ŋL�q�����.
// �f�[�^���ꕔ�������錻�ۂ��������[���ȃo�O�̌����ƂȂ�.
bool EnemyManager::Add(EnemyBase* pEnemy)
{
	int index = Next();		// �z�񂩂�󂫂�T��.
	if (0 <= index && index < m_enemyMax) {
		m_ppBuffer[index] = pEnemy;// �G���Ǘ��p�̔z��ɓo�^.
		return true;
	}
	return false;
}

// �z��̊e�v�f���ׂĂɁA������i�߂�悤�w������.
void EnemyManager::Update(float time)
{
	for (int i = 0; i < m_enemyMax; i++) {
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
void EnemyManager::Damage(int damage, int index)
{
	if (0 <= index && index < m_enemyMax) {
		m_ppBuffer[index]->Damage(damage);
		if (GetHP(&index) <= 0) {
			delete m_ppBuffer[index];
			m_ppBuffer[index] = NULL;// �z��̒��g��NULL�N���A���Ď��̎g�p�ɔ�����.
		}
	}
}

// �z��̑S�Ă̓G�ɑ΂��āA�\���𑣂�.
void EnemyManager::Show()
{
	for (int i = 0; i < m_enemyMax; i++) {
		// m_ppBuffer[i]�̌^��EnemyBase*�����AShow �� virtual�֐��Ȃ̂�.
		// �h���N���X��show���K�؂ɌĂяo�����.
		// �܂�if��switch���g���ĕ��򂳂���K�v���Ȃ��B���ꂪ�I�u�W�F�N�g�w���̗��_�̂P��.
		if (m_ppBuffer[i] != NULL) {
			m_ppBuffer[i]->Show();
		}
	}
}

// ���݂�XY���W�l�𓾂�.
D3DXVECTOR2 EnemyManager::GetXY(int* index) {
	return m_ppBuffer[*index]->GetXY();
}

// ���݂̓G�̃T�C�Y�𓾂�.
float EnemyManager::GetSize(int* index)
{
	return m_ppBuffer[*index]->GetSize();
}

// �ő�HP�𓾂�
int EnemyManager::GetMaxHP(int* index){
	return m_ppBuffer[*index]->GetMaxHP();
}

// ���݂�HP�𓾂�
int EnemyManager::GetHP(int* index) {
	return m_ppBuffer[*index]->GetHP();
}

void EnemyManager::DamageFlg(unsigned char damage, int *index) {
	m_ppBuffer[*index]->DamageFlg(damage);
}

// �_���[�W�t���O�����Z�b�g
void EnemyManager::DamageReset(unsigned char damage, int* index) {
	m_ppBuffer[*index]->DamageReset(damage);
}

unsigned char EnemyManager::IsDamage(int* index) {
	return m_ppBuffer[*index]->IsDamage();
}

// �z��̑傫����Ԃ�
int EnemyManager::EnemyMax()
{
	return m_enemyMax;
}

// ���̔z�񂪎g�p�\��
bool EnemyManager::PPBuffer(int index) {
	if (m_ppBuffer[index] != NULL) {
		return true;
	}
	return false;
}

// �f�o�b�O�p��m_pBuffer�z��̒��g���e�L�X�g������.
// �e�ۂ��L���F*.
// �e�ۂ������F-.
char* EnemyManager::GetText()
{
	for (int i = 0; i < m_enemyMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			m_pTextBuff[i] = m_ppBuffer[i]->Getch();
		}
		else {
			m_pTextBuff[i] = '-';
		}
	}
	// ������̍Ō�͕K��NULL�����Ń^�[�~�l�[�g.
	m_pTextBuff[m_enemyMax] = NULL;
	return m_pTextBuff;
}

// (cx, cy)�̈ʒu�͓G�ƏՓˋ�����������.
// �Փ˂����ꍇ�͈���pIndex�������ꏊ�ɔz��̃C���f�b�N�X������TRUE��Ԃ�.
// �Փ˂����������ꍇ��FALSE�Ԃ�.
bool EnemyManager::Collision(float cx, float cy, float limit2, int* pIndex)
{
	for (int i = 0; i < m_enemyMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			if (m_ppBuffer[i]->Collision(cx, cy, limit2)) {
				*pIndex = i;
				return true;
			}
		}
	}
	return false;
}