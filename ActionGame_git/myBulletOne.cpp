/*
	myBulletOne.cpp

	�e�ۃN���X�i�P�����j
*/
#include "stdafx.h"
#include "myBulletOne.h"

// BulletOne�N���X�̃R���X�g���N�^.
// �����o�ϐ���������.
BulletOne::BulletOne()
	: m_posX(0.0f)
	, m_posY(0.0f)
	, m_velX(0.0f)
	, m_velY(0.0f)
	, m_timer(-1.0f)
	, m_player(false)
{}

// BulletOne�N���X�̃f�X�g���N�^.
BulletOne::~BulletOne()
{}

// �e�ۂ̏�����.
// x 	�����ʒuX���W.
// y 	�����ʒuY���W.
// vx	�����xX����.
// vy	�����xY����.
void BulletOne::Init(float x, float y, float vx, float vy)
{
	m_posX = x;
	m_posY = y;
	m_velX = vx;
	m_velY = vy;
	m_timer = 0.0f;	// timer >= 0.0f �ŗL���Ȓe�ۂƂȂ�.
	//m_player = player;

}

// ���݈ʒu���擾����B�������Q�ƌ^�ɂȂ��Ă��邽�߁A�����߂�l�ƂȂ�.
// �����̒l��Ԃ������ꍇ�ɂ́A�������g���Ēl��Ԃ����@����邱�Ƃ�����.
// x 	���݈ʒu��X����.
// y 	���݈ʒu��Y����.
void BulletOne::GetXY(float& x, float& y)
{
	x = m_posX;
	y = m_posY;
}

// ���x����������time���������Z���āA�e�ۂ̈ʒu���X�V����.
// �������A��������Ă���̎���timer��0�ȏ�̏ꍇ�̂�.
// �܂��A���݈ʒu��Y���������ɂȂ�����Atimer�𕉂̒l�ɂ��Ēe�ۂ𖢎g�p��Ԃɂ���.
void BulletOne::Update(float time)
{
	if (IsActive()) {
		m_posX += m_velX * time;
		m_posY += m_velY * time;
		m_timer += time;
	}

	// ��ʊO�ɏo����e�ۂ𖳌��ɂ���.
	if ((m_posY < 0.0f || HEIGHT < m_posY) || (m_posX < 0.0f || WIDTH < m_posX)) {
		Deactive();
	}
}
