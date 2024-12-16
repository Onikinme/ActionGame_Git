/*
	myBossBBB.cpp

	�{�XAAA
*/
#include "stdafx.h"
#include "myBossBBB.h"
#include "myApp.h"

bool BossBBB::Update(float time)
{
	//player�Ƃ̋���
	D3DXVECTOR2 pPos = { 0,0 };
	GetApp()->GetPlayerPos(pPos);
	//�A�N�V�������I����Ă���(�������͂��Ă��Ȃ�)��������m_attackTimer�����łȂ���
	if (!m_actionFlg && m_actionTimer < 0)
	{
		//Player��Boss�̋�����10.0f�ȓ��Ȃ�
		if (fabs(pPos.x - m_posX) < 60.0f)
		{
			m_nearTimer += time;
		}
		else//10.0f��艓��
		{
			m_farTimer += time;
		}

		//25.0f���ԗ���Ă�����
		if (m_farTimer > 25.0f)
		{
			//�_�b�V���U��
			ChangeAction(1);
			m_farTimer = 0.0f;
		}

		if (m_nearTimer > 25.0f)
		{
			//�W�����v�U��������
			ChangeAction(2);
			m_nearTimer = 0.0f;
		}
	}
	//�^�C�}�[��i�߂�
	return BossTmp::Update(time);
}

void BossBBB::Attack()
{
	/*�U����*/
	if (m_actionFlg && m_actionTimer > 0.0f)
	{
		float AttckTime = m_timer - m_actionTimer;
		if (AttckTime < 8.5f)// 0.0f ~ 2.5f
		{
			//�^���`���[�W
			m_animNum = 3;
		}
		else if (AttckTime < 10.5f)  // 2.5f ~ 12.5f
		{
			
			Dash();

		}
		else if (AttckTime < 15.0f)// 12.5f ~ 15.0f
		{
			m_animNum = 4;
		}
		else  if (AttckTime > 17.0f)
		{
			//�A�N�V�����I��
			m_actionTimer = -1.0f;
			m_actionFlg = false;
			m_dashflg = false;
			m_animNum = 0;
			m_actionNum = 0;

		}
	}
}

void BossBBB::Dash()
{

	BulletOne* pBulletOne = GetApp()->GetEneBltBuf()->Next();
	if (!m_dashflg)
	{
		D3DXVECTOR2 playerpos;
		GetApp()->GetPlayerPos(playerpos);
		float vx = playerpos.x - m_posX;
		float vy = playerpos.y - m_posY;
		float distance = sqrt(vx * vx + vy * vy);
		float dx = vx / distance;
		float dy = vy / distance;


		float speed = 15.0f;
	     m_fXSpeed = speed * dx;
	     m_fYSpeed = speed * dy;

		 m_dashflg = true;
	}
	else
	{
		//nothing to do
	}
	m_posX += m_fXSpeed;
}
