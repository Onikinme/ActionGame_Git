/*
	myBulletOne.h

	�e�ۃN���X�i�P�����j
*/
#pragma once

#include "myTexture.h"

//////////////////////////////////
// BulletOne
// �e�ۂ̂��߂̃N���X�F���ꂼ��̒e�ۂ��������A�ړ��Ȃǒe�ۂ̊Ǘ�.
class BulletOne
{
private:
	float m_posX, m_posY;	// ���݈ʒu.
	float m_velX, m_velY;	// �e�ۑ��x.
	float m_size;           // �e�ۂ̃T�C�Y
	float m_timer;			// �e�ێˏo����̎���.
	bool m_player;          // �v���C���[�̒e���H

public:
	BulletOne();			// �R���X�g���N�^.
	virtual ~BulletOne();
	void Init(float x, float y, float vx, float vy, float size);	// �ˏo������.
	void GetXY(float& x, float& y);	// ���݂�XY���W�l�𓾂�.
	void GetSize(float& size);      // �e�ۂ̃T�C�Y�𓾂�
	void Update(float time);		// �e�ۂ�i�߂�.
	void Draw(MyTexture*);		// �e�ۃe�N�X�`�����w�肵�ĕ`��.
	float GetTime()					// �����𓾂�.
	{
		return m_timer;
	}
	//bool IsPlayer(){
	void Deactive()			// �����ɂ���.
	{
		m_timer = -1.0f;
	}
	bool IsActive()			// �^�C�}�[��0�ȏ�Ȃ�ΗL���Ȓe��.
	{
		return (m_timer >= 0.0f);
	}
};
