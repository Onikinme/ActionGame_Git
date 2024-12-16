/*
	myFloorSlip.cpp

	����ʂ���
*/
#include "stdafx.h"
#include "myFloorSlip.h"
#include "myApp.h"

bool FloorSlip::Update(float time)
{
	Collision();

	return true;
}

// �v���C���[�����̂���ʂ����ɏ���������肷��֐�
void FloorSlip::Collision() {

	// MyApp��ʂ��ăv���C���[�̍��W�ƃT�C�Y���擾
	D3DXVECTOR2 playerpos;
	float playersize;
	playerpos = GetApp()->GetPlayer()->GetXY();
	playersize = GetApp()->GetPlayer()->GetSize();

	// �v���C���[���ォ�炷��ʂ����ɍ~���Ă�����
	if (m_pos.x < playerpos.x + playersize && playerpos.x < m_pos.x + m_w && m_pos.y - m_h < playerpos.y && m_pos.y - m_h + 8 > playerpos.y && GetApp()->GetPlayer()->IsFall()) {
		GetApp()->GetPlayer()->SetXY(D3DXVECTOR2(playerpos.x, m_pos.y - m_h));
		// �v���C���[���ڒn�������Ƃɂ���
		GetApp()->GetPlayer()->IsGround();
		// �v���C���[�����̏��ɏ�����Ƃ����t���O��true�ɂ���
		if(!floorflg)floorflg = true;
	}

	// ���̏�����W�����v������floorflg��false�ɂ���
	if (floorflg && m_pos.y - m_h > playerpos.y) {
		floorflg = false;
	}

	// �v���C���[�����̏��̉�������~�肽�Ƃ�
	if (floorflg && (m_pos.x > playerpos.x + playersize || playerpos.x > m_pos.x + m_w)) {
		// �v���C���[���󒆂ɂ��邱�Ƃɂ���
		GetApp()->GetPlayer()->IsAir();
		floorflg = false;
	}
}

/*

//�U��
void FloorSlip::Attack() {

	//�΂�f���U��
	�U����
	if (m_actionFlg && m_actionTimer > 0.0f)
	{
		float AttckTime = m_timer - m_actionTimer;
		if (AttckTime < 8.5f)// 0.0f ~ 2.5f
		{

			m_animNum = 3;
		}
		else if (AttckTime < 10.5f)  // 2.5f ~ 12.5f
		{
			m_animNum = 4;
			//m_animNum = 3;
			//�Δ���
			Fire();

		}
		else if (AttckTime < 15.0f)// 12.5f ~ 15.0f
		{

		}
		else  if (AttckTime > 17.0f)
		{
			//�A�N�V�����I��
			m_actionTimer = -1.0f;
			m_actionFlg = false;
			m_animNum = 0;
			m_actionNum = 0;


		}
	}
}

void FloorSlip::Fire()
{

	
	����f���o��
	Fire�N���X��������Bullet�N���X����e�I�u�W�F�N�g�𐶐�����B
	
	_tprintf(_T("����\n"));

	BulletOne* pBulletOne = GetApp()->GetEneBltBuf()->Next();
	if (pBulletOne != NULL)
	{
		float ADx = 10.0f;
		float fXSpeed = -5.0f;
		if (!m_leftFlg)//�v���C���[���E��
		{
			fXSpeed *= -1;
			ADx *= -1;
		}
		float fYSpeed = 0.0f;
		pBulletOne->Init(m_posX - ADx, m_posY, fXSpeed, -fYSpeed);
	}

}
*/