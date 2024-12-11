/*
	myEnemyAAA.cpp

	�G�`
*/
#include "stdafx.h"
#include "myEnemyAAA.h"
//#include "myBulletOne.h"
//#include "myBulletBuffer.h"
#include "myApp.h"

// �ړ�.
bool EnemyAAA::Update(float time)
{
	EnemyBase::Update(time);		// �e��Update���Ăяo��.


	// �ʒu���X�V.
	m_pos.x += m_vel.x * time;
	m_pos.y += m_vel.y * time;

	// ���E�̕ǂɓ��������甽�]
	if (m_pos.x > WIDTH - 64.0f) {
		m_pos.x = WIDTH - 64.0f;
		m_vel.x *= -1;
	}
	if (m_pos.x < 0.0f) {
		m_pos.x = 0.0f;
		m_vel.x *= -1;
	}

	if (m_timer > 10.0f) {
		// ��莞�Ԍ�ɁA��ʊO�i���j�Ɉړ�����Ə���.
		if (m_pos.y > HEIGHT + 32.0f) {
			return false;
		}
		// ��ʏ���Ɉړ�����\��͂Ȃ����ی��ŉ�ʏ���Ɉړ������ꍇ�����ł�����.
		if ( m_pos.y < 0.0f - 32.0f) {
			return false;			
		}
	}

	return true;
}

/*
// �v���C���[�����ɔ���.
void EnemyAAA::Fire()
{
	if ((rand() % 1000 < 10) && (m_timer < 60)) {
		// �v���C���[�ւ̃x�N�g�����Z�o.
		float px, py;
		GetApp()->GetPlayerPos(px, py);
		float dx = px - m_posX;
		float dy = py - m_posY;
		float dd = (float)sqrt(dx * dx + dy * dy);
		// �e�̑��x���o��.
		dx /= dd; dy /= dd;
		dx *= 5;  dy *= 5;

		BulletOne* pBlt = GetApp()->GetEneBltBuf()->Next();
		if (pBlt != NULL) {
			pBlt->Init(m_posX, m_posY, dx, dy);
		}
	}
}
*/
