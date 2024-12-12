#include "stdafx.h"
#include "myExplosionOne.h"
#include "myApp.h"

// �N���X�̃f�X�g���N�^.
ExplosionOne::~ExplosionOne()
{}

void ExplosionOne::Start(float x, float y)
{
	m_posX = x;
	m_posY = y;
	m_ExplTimer = 0;
}

void ExplosionOne::Update(int time)
{
	assert(0 <= m_ExplTimer);

	// �W�p�^�[���̃A�j���[�V�������T�t���[�����ɍX�V����̂łS�O�t���[���Ŕ����A�j�����I��������.
	m_ExplTimer += time;
	if (m_ExplTimer > 20)
	{
		m_ExplTimer = -1;// ���̒l�ŁA������~��ԂƂ���.
	}
}

// �����A�j���̕`��.
void ExplosionOne::Draw(MyTexture* pTex)
{

	// �����e�N�X�`���̕\���̈�̂��߂̔z��
	const static RECT rcExplImageUV[] =
	{
	{ 0, 0, 64, 64 },
	{ 0, 64, 64, 128 },
	};

	//assert(0 < m_ExplTimer && m_ExplTimer <= 40);
	int nTime = m_ExplTimer - 1;// Draw�������Update���Ăяo����邽�߁A�O�t���[���̒l�Ōv�Z.

	D3DXVECTOR3 cnt(32.f, 32.f, 0.f);		// �摜�̃Z���^�[�ʒu���w��.
	//D3DXVECTOR3 cnt(48.f, 48.f, 0.f);		// �摜�̃Z���^�[�ʒu���w��.
	D3DXVECTOR3 pos(m_posX, m_posY, 0.f);	// �\���ʒu���w��.

	int animTime = nTime / 10;	// �T�t���[�����ɍX�V.
	int anim = animTime % 2;	// �W�p�^�[���̃A�j���[�V����.�@�z��̃I�[�o�[�A�N�Z�X����̂��ߏ�]�Z�ɂ��Ă���.
	//int anim = animTime % 9;	// �X�p�^�[���̃A�j���[�V����.�@�z��̃I�[�o�[�A�N�Z�X����̂��ߏ�]�Z�ɂ��Ă���.
	//pSpr->Draw(pTex->GetTexture(), &(rcExplImageUV[anim]), &cnt, &pos, 0xFFFFFFFF);

	MyApp* myapp = GetApp();
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	myapp->Draw(pSpr, pTex->GetTexture(), pos, rcExplImageUV[anim], 64.0f, 64.0f, false, false);
}
