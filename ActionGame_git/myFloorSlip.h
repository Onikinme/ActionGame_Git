/*
	myFloorSlip.h

	����ʂ���
*/
#pragma once

#include "myFloorManager.h"

//////////////////
// FloorSlip
// ��������AFloorBase�̔h���N���X���`���Ă���.
// FloorBase�͒��ۃN���X�Ȃ̂ŁA���̂𐶐����邱�Ƃ͏o���Ȃ�.
// FloorBase����h�������N���X����́A��̓I�Ȏ��̂������Ƃ��ł���.

// FloorBase�̔h���N���X�FFloorSlip.
class FloorSlip : public FloorBase
{
private:
public:
	// FloorSlip�͐e�N���X�̖����I�Ɏw�肵�Ă���.
	// FloorBase(pTex)�������ƁA�R���X�g���N�^��()���Ăяo�����ƂɂȂ邪.
	// ���������̃R���X�g���N�^FloorBase::FloorBase()�͑��݂��Ȃ��̂�.
	// �R���p�C���G���[���o��.
	FloorSlip(MyTexture* pTex) : FloorBase(pTex)
	{
	}
	~FloorSlip() override
	{
	}

	bool Update(float time);

	// �v���C���[�����̂���ʂ����ɏ���������肷��֐�
	void Collision();
};

