/*
	myFloorManager.cpp

	���̊��N���X�ƊǗ��N���X
*/
#include "stdafx.h"
#include "myFloorManager.h"
#include "myApp.h"


// �G�̈ړ������F���N���X�ł͎�����i�߂邾��.
bool FloorBase::Update(float time)
{
	if (m_timer >= 0.0f) {
		m_timer += time;		// ������i�߂�.
	}
	return true;
}

// �G�̏�����.
void FloorBase::Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h)
{
	// x (�����ʒuX), y (�����ʒuY)
	m_pos = pos;
	m_vel = v_pos;
	m_w = w, m_h = h;				  // ���̃T�C�Y
	m_timer = 0.0f;	// �^�C�}�[��0.0f�ɂ��āA�L��������. 
}

void FloorBase::Show()
{
	assert(m_timer >= 0.0f);// �^�C�}�[�����̏�ԂŌĂ΂��ׂ��ł͂Ȃ��̂ŃA�T�[�g���d�|���Ă���.
	if (m_timer < 0.0f) {
		return;
	}

	D3DXVECTOR3 pos(m_pos.x, m_pos.y, 0);
	MyApp* myapp = GetApp();
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	myapp->Draw(pSpr, m_pTex->GetTexture(), pos, { 0, 0,(long)m_w, (long)m_h }, m_w, m_h, false, false, false, 0.5f);
}

// ���݂�XY���W�l�𓾂�.
D3DXVECTOR2 FloorBase::GetXY()
{
	// ���K�Q
	// TODO �����o�ϐ��̒l���擾���鏈�����L�q���Ă�������.
	return m_pos;
}

// ���݂̏��̃T�C�Y�𓾂�.
float FloorBase::GetSize()
{
	return m_w;
}

// �ʒu(cx, cy)�Ǝ����̈ʒu(posX, posY)�͋���limit2���߂����ǂ���.
// limit2�͋����̓��.
bool FloorBase::Collision(float cx, float cy, float limit2)
{
	return (GetDistance2(cx, cy, m_pos.x, m_pos.y) < limit2);
}

// ���̋󂫈ʒu��T��.
int FloorManager::Next()
{
	// �����O�o�b�t�@���A���݈ʒu(m_index)�������T��.
	for (int i = 0; i < m_floorMax; i++) {
		// ���݈ʒu(m_index)����󂫂�����.
		int index = (m_index + i) % m_floorMax;
		// index ��m_enemyMax�ȏ�ɂȂ�Ȃ��悤�A��]�ɂ��邱�ƂŔz���.
		// ���݈ʒu���� m_index - 1 �̈ʒu�܂ł�����.
		if (m_ppBuffer[index] == NULL) {
			m_index = (index + 1) % m_floorMax;
			return index;
		}
	}
	return -1;
}

// �f�X�g���N�^:�z���������Ă���.
// �z��̒��g���܂�������Ă���S�̂����.
FloorManager::~FloorManager()
{
	for (int i = 0; i < m_floorMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			delete m_ppBuffer[i];
		}
	}
	delete[] m_ppBuffer;
	delete[] m_pTextBuff;
}

// �������̊m�ۓ��̏�����.
bool FloorManager::Init()
{
	if (m_ppBuffer == NULL) {
		m_ppBuffer = new FloorBase * [m_floorMax];	// �z��̒��g��EnemyBase*�^.
		// ��sizeof(EnemyBase*) �� max ���̃��������m�ۂ��Ă��邾���ŁA�R���X�g���N�^�͌Ăяo����Ȃ�.
		// sizeof(EnemyBase*)��x64�ł͂P������8�o�C�g�Ax86�ł͂P������4�o�C�g�̃������T�C�Y.
		if (m_ppBuffer == NULL) {
			return false;
		}

		for (int i = 0; i < m_floorMax; i++) {
			m_ppBuffer[i] = NULL;		// �|�C���^��NULL�ŏ��������A���g�p��ԂƂ���.
		}
	}

	if (m_pTextBuff == NULL) {
		m_pTextBuff = new char[m_floorMax + 1];	// �f�o�b�O�p��������m�ۂ���.
		// +1 �͏I�[�L��'\0'�̕�.
		if (m_pTextBuff == NULL) {
			return false;
		}
	}

	return true;
}

// �z��̑S�Ă̗v�f�����Z�b�g����.
void FloorManager::ResetAll()
{
	for (int i = 0; i < m_floorMax; i++) {
		delete m_ppBuffer[i];
		m_ppBuffer[i] = NULL;// �z��̒��g��NULL�N���A���Ď��̎g�p�ɔ�����.
	}
}
// pEnemy��z��ɒǉ�.
// EnemyBase*�^�������ɂ��Ă��邪�A���ۂɂ�EnemyAAA*�^��EnemyBBB*�^�������Ă���.
// �����̃N���X�́AEnemyBase�̔h���N���X�Ȃ̂ŁAEnemyBase*�^�Ŏ󂯂邱�Ƃ��ł���.
// �Ԉ���Ĉ�����(EnemyBase pEnemy) �Ɓ���t���Y��A�|�C���^�ł͂Ȃ����̂ŋL�q�����.
// �f�[�^���ꕔ�������錻�ۂ��������[���ȃo�O�̌����ƂȂ�.
bool FloorManager::Add(FloorBase* pFloor)
{
	int index = Next();		// �z�񂩂�󂫂�T��.
	if (0 <= index && index < m_floorMax) {
		m_ppBuffer[index] = pFloor;// �G���Ǘ��p�̔z��ɓo�^.
		return true;
	}
	return false;
}

// �z��̊e�v�f���ׂĂɁA������i�߂�悤�w������.
void FloorManager::Update(float time)
{
	for (int i = 0; i < m_floorMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			bool flag = m_ppBuffer[i]->Update(time);
		}
	}
}

// �z��̑S�Ă̓G�ɑ΂��āA�\���𑣂�.
void FloorManager::Show()
{
	for (int i = 0; i < m_floorMax; i++) {
		// m_ppBuffer[i]�̌^��EnemyBase*�����AShow �� virtual�֐��Ȃ̂�.
		// �h���N���X��show���K�؂ɌĂяo�����.
		// �܂�if��switch���g���ĕ��򂳂���K�v���Ȃ��B���ꂪ�I�u�W�F�N�g�w���̗��_�̂P��.
		if (m_ppBuffer[i] != NULL) {
			m_ppBuffer[i]->Show();
		}
	}
}

// ���݂�XY���W�l�𓾂�.
D3DXVECTOR2 FloorManager::GetXY(int* index) {
	return m_ppBuffer[*index]->GetXY();
}

// ���݂̏��̃T�C�Y�𓾂�.
float FloorManager::GetSize(int* index)
{
	return m_ppBuffer[*index]->GetSize();
}

// �z��̑傫����Ԃ�
int FloorManager::FloorMax()
{
	return m_floorMax;
}

// ���̔z�񂪎g�p�\��
bool FloorManager::PPBuffer(int index) {
	if (m_ppBuffer[index] != NULL) {
		return true;
	}
	return false;
}

// �f�o�b�O�p��m_pBuffer�z��̒��g���e�L�X�g������.
// �e�ۂ��L���F*.
// �e�ۂ������F-.
char* FloorManager::GetText()
{
	for (int i = 0; i < m_floorMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			m_pTextBuff[i] = m_ppBuffer[i]->Getch();
		}
		else {
			m_pTextBuff[i] = '-';
		}
	}
	// ������̍Ō�͕K��NULL�����Ń^�[�~�l�[�g.
	m_pTextBuff[m_floorMax] = NULL;
	return m_pTextBuff;
}

// (cx, cy)�̈ʒu�͓G�ƏՓˋ�����������.
// �Փ˂����ꍇ�͈���pIndex�������ꏊ�ɔz��̃C���f�b�N�X������TRUE��Ԃ�.
// �Փ˂����������ꍇ��FALSE�Ԃ�.
bool FloorManager::Collision(float cx, float cy, float limit2, int* pIndex)
{
	for (int i = 0; i < m_floorMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			if (m_ppBuffer[i]->Collision(cx, cy, limit2)) {
				*pIndex = i;
				return true;
			}
		}
	}
	return false;
}