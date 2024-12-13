/*
	myBossAAA.cpp

	�{�XAAA
*/
#include "stdafx.h"
#include "myBossAAA.h"
#include "myApp.h"

bool BossAAA::Update(float time)
{
	//player�Ƃ̋���
	D3DXVECTOR2 pPos = {0,0};
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
			//���Œǂ�����
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
	return BossTmp :: Update(time);
}

//�U��
void BossAAA::Attack() {

	//�΂�f���U��
	/*�U����*/
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

void BossAAA::Fire()
{

	/*
	����f���o��
	Fire�N���X��������Bullet�N���X����e�I�u�W�F�N�g�𐶐�����B
	*/
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


