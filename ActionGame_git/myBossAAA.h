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
public:
	// BossAAA�͐e�N���X�̃R���X�g���N�^�𖾎��I�Ɏw�肵�Ă���.
	// BossTmp(pTex)�������ƁABossTmp()���Ăяo�����ƂɂȂ邪.
	// ���������̃R���X�g���N�^BossTmp::BossTmp()�͑��݂��Ȃ��̂�.
	// �R���p�C���G���[���o��.
	BossAAA(MyTexture* pTex) : BossTmp(pTex)
	{
	}
	~BossAAA() override
	{
	}

	//�U��
	void Attack();
	//��(�e)����
	void Fire();
};

