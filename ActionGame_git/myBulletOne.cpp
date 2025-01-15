/*
	myBulletOne.cpp

	�e�ۃN���X�i�P�����j
*/
#include "stdafx.h"
#include "myBulletOne.h"
#include "myApp.h"

// BulletOne�N���X�̃R���X�g���N�^.
// �����o�ϐ���������.
BulletOne::BulletOne()
	: m_posX(0.0f)
	, m_posY(0.0f)
	, m_velX(0.0f)
	, m_velY(0.0f)
	, m_timer(-1.0f)
	, m_player(false)
{}

// BulletOne�N���X�̃f�X�g���N�^.
BulletOne::~BulletOne()
{}

// �e�ۂ̏�����.
// x 	�����ʒuX���W.
// y 	�����ʒuY���W.
// vx	�����xX����.
// vy	�����xY����.
void BulletOne::Init(float x, float y, float vx, float vy, float size)
{
	m_posX = x;
	m_posY = y;
	m_velX = vx;
	m_velY = vy;
	m_size = size;
	m_timer = 0.0f;	// timer >= 0.0f �ŗL���Ȓe�ۂƂȂ�.
	//m_player = player;

}

// ���݈ʒu���擾����B�������Q�ƌ^�ɂȂ��Ă��邽�߁A�����߂�l�ƂȂ�.
// �����̒l��Ԃ������ꍇ�ɂ́A�������g���Ēl��Ԃ����@����邱�Ƃ�����.
// x 	���݈ʒu��X����.
// y 	���݈ʒu��Y����.
void BulletOne::GetXY(float& x, float& y)
{
	x = m_posX;
	y = m_posY;
}

void BulletOne::GetSize(float& size)
{
	size = m_size;
}

// ���x����������time���������Z���āA�e�ۂ̈ʒu���X�V����.
// �������A��������Ă���̎���timer��0�ȏ�̏ꍇ�̂�.
// �܂��A���݈ʒu��Y���������ɂȂ�����Atimer�𕉂̒l�ɂ��Ēe�ۂ𖢎g�p��Ԃɂ���.
void BulletOne::Update(float time)
{
	if (IsActive()) {
		m_posX += m_velX * time;
		m_posY += m_velY * time;
		m_timer += time;
	}

	// ��ʊO�ɏo����e�ۂ𖳌��ɂ���.
	if ((m_posY < 0.0f || HEIGHT < m_posY) || (m_posX < 0.0f - m_size * 2 || WIDTH < m_posX)) {
		Deactive();
	}
}

void BulletOne::Draw(MyTexture* pTex)		// �e�ۃe�N�X�`�����w�肵�ĕ`��.
{
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	D3DXVECTOR3 cnt(8, 8, 0);
	D3DXVECTOR3 pos(0, 0, 0);
	RECT rc[] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	};

	rc[0] = {0, 0, (int)m_size, (int)m_size};
	rc[1] = { 0, (int)m_size, (int)m_size, (int)m_size * 2 };
	rc[2] = { 0, (int)m_size * 2, (int)m_size, (int)m_size * 3 };
	rc[3] = { 0, (int)m_size, (int)m_size, (int)m_size * 2 };

	int nTime = m_timer - 1;// Draw�������Update���Ăяo����邽�߁A�O�t���[���̒l�Ōv�Z.
	int animTime = nTime / 8;	// �T�t���[�����ɍX�V.
	int anim = animTime % 4;	// �W�p�^�[���̃A�j���[�V����.�@�z��̃I�[�o�[�A�N�Z�X����̂��ߏ�]�Z�ɂ��Ă���.

	// ���݈ʒu��pos�\���̌^�̃f�[�^�Ɋi�[���ĕ\��.
	GetXY(pos.x, pos.y);
	//pSpr->Draw(pTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
	if(m_velX > 0)GetApp()->Draw(GetApp()->GetSprite(), pTex->GetTexture(), pos, rc[anim], m_size, 0.0f, false, false, false, 1);
	else GetApp()->Draw(GetApp()->GetSprite(), pTex->GetTexture(), pos, rc[anim], m_size, 0.0f, true, false, false, 1);
}