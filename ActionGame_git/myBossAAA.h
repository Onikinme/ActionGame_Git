/*
	myBossAAA.h

	�{�XAAA
*/
#pragma once

#include "myBossTmp.h"

//////////////////
// BossAAA
// ��������ABossTmp�̔h���N���X���`���Ă���.
// BossTmp�͒��ۃN���X�Ȃ̂ŁA���̂𐶐����邱�Ƃ͏o���Ȃ�.
// BossTmp����h�������N���X����́A��̓I�Ȏ��̂������Ƃ��ł���.

// BossTmp�̔h���N���X�FBossAAA.
class BossAAA : public BossTmp
{
private:
	float m_nearTimer;//Player�Ƃ̋ߋ����̎��Ԃ�}��
	float m_farTimer;//Player�Ƃ̉������̎��Ԃ�}��
	bool  m_fireflg; //true:fire����ʏ�,false:��ʓ���fire����
public:
	// BossAAA�͐e�N���X�̃R���X�g���N�^�𖾎��I�Ɏw�肵�Ă���.
	// BossTmp(pTex)�������ƁABossTmp()���Ăяo�����ƂɂȂ邪.
	// ���������̃R���X�g���N�^BossTmp::BossTmp()�͑��݂��Ȃ��̂�.
	// �R���p�C���G���[���o��.
	BossAAA(MyTexture* pTex) : BossTmp(pTex), 
	 m_nearTimer(0.0f),
	 m_farTimer(0.0f),
	 m_fireflg(false)
	{
	}
	~BossAAA() override
	{
	}

	//�{�X�̃A�N�V�����p�^�[�����܂܂��
	bool Update(float time);

	//�U��
	void Attack();
	//��(�e)����
	void Fire();
};

