#pragma once

#include "myTexture.h"

// ExplosionOne //////////////////////////////////
// �����P�̃A�j���[�V����
class ExplosionOne
{
private:
	float m_posX, m_posY;
	int m_ExplTimer;

public:
	ExplosionOne() : m_posX(0.0f), m_posY(0.0f), m_ExplTimer(-1)
	{
		_tprintf(_T("�������� addr=%p\n"), this);
	}
	virtual ~ExplosionOne();

	void Start(float x, float y); // �����J�n.
	void Update(int tt);		  // �����i�t���[���j�X�V.
	void Draw(MyTexture*);		  // �����A�j���̕`��.
	void Deactive()				  // �����ɂ���.
	{
		m_ExplTimer = -1.0f;
	}
	bool IsActive()
	{
		return (m_ExplTimer >= 0);
	}
};