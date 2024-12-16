/*
	myBossBBB.h

	�{�XBBB
*/
#pragma once
#include "myBossTmp.h"

//////////////////
// BossBBB
// BossTmp�̔h���N���X�FBossAAA.
class BossBBB : public BossTmp
{
private:
	float m_nearTimer;//Player�Ƃ̋ߋ����̎��Ԃ�}��
	float m_farTimer;//Player�Ƃ̉������̎��Ԃ�}��
	bool  m_dashflg;
	float m_fXSpeed;
	float m_fYSpeed;

public:
	// BossBBB�͐e�N���X�̃R���X�g���N�^�𖾎��I�Ɏw�肵�Ă���.
	// BossTmp(pTex)�������ƁABossTmp()���Ăяo�����ƂɂȂ邪.
	// ���������̃R���X�g���N�^BossTmp::BossTmp()�͑��݂��Ȃ��̂�.
	// �R���p�C���G���[���o��.
	BossBBB(MyTexture* pTex) : BossTmp(pTex),
		m_nearTimer(0.0f),
		m_farTimer(0.0f),
		m_dashflg(false),
		m_fXSpeed(0.0f),
		m_fYSpeed(0.0f)
	{
	}
	~BossBBB() override
	{
	}

	//�{�X�̃A�N�V�����p�^�[�����܂܂��
	bool Update(float time);

	//�U��
	void Attack();

	//�_�b�V��
	void Dash();
};
