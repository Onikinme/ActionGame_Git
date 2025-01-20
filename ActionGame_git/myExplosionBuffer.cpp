#include "stdafx.h"
#include "myExplosionBuffer.h"
#include "myApp.h"

ExplosionBuffer::~ExplosionBuffer()
{
	if (m_pExplBuf) {
		delete[] m_pExplBuf;
	}
	if (m_pTextBuffer)
	{
		delete[] m_pTextBuffer;
	}
	_tprintf(_T("ExplosionBuffer deleted.\n"));
}

bool ExplosionBuffer::Init()
{
	if (m_pExplBuf == NULL) {
		m_pExplBuf = new ExplosionOne[m_explMax];
		assert(m_pExplBuf != NULL);
		if (m_pExplBuf == NULL) {
			return false;
		}
	}

	if (m_pTextBuffer == NULL) {
		m_pTextBuffer = new char[m_explMax + 1];// �f�o�b�O�p��������m�ۂ���B������Ȃ̂Ł{�P���K�v�B�I�[�L��'\0'�̂��߁B
		assert(m_pTextBuffer != NULL);
		if (m_pTextBuffer == NULL)
		{
			return false;
		}
	}
	return true;
}

void ExplosionBuffer::ResetAll()
{
	for (int i = 0; i < m_explMax; i++) {
		m_pExplBuf[i].Deactive();
	}
}

// �����𔭐�������
void ExplosionBuffer::Explose(float x, float y)
{
	ExplosionOne* pExplOne = Next();
	if (pExplOne)
	{
		pExplOne->Start(x - 32, y - 32);
		_tprintf(_T("���� explosion=%p\n"), pExplOne);
	}
}

ExplosionOne* ExplosionBuffer::Next()
{
	// �����O�o�b�t�@���A���݈ʒu(m_index)�������T��.
	for (int i = 0; i < m_explMax; i++)
	{
		// ���݈ʒu(m_index)����󂫂�����.
		int index = (m_index + i) % m_explMax;
		// index ��m_explMax�ȏ�ɂȂ�Ȃ��悤�A��]�ɂ��邱�ƂŔz���
		// ���݈ʒu���� m_index - 1 �̈ʒu�܂ł�����.
		if (!m_pExplBuf[index].IsActive())
		{
			m_index = (index + 1) % m_explMax;
			return (m_pExplBuf + index);
		}
	}
	// �󂫂Ȃ�
	return NULL;
}

/*
ExplosionOne* ExplosionBuffer::Get(int index)
{
	return (m_pExplBuf + index);
}
*/

void ExplosionBuffer::Update(int time)
{
	for (int i = 0; i < m_explMax; ++i)
	{
		if (m_pExplBuf[i].IsActive())
		{
			m_pExplBuf[i].Update(time);
		}
	}
}

void ExplosionBuffer::Draw(MyTexture* pTex)
{
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	D3DXVECTOR3 cnt(8, 8, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	for (int i = 0; i < m_explMax; ++i)
	{
		// �L���Ȃ�\��.
		if (m_pExplBuf[i].IsActive()) {
			m_pExplBuf[i].Draw(pTex);
		}
	}
}

char* ExplosionBuffer::GetText()
{
	for (int i = 0; i < m_explMax; i++)
	{
		if (m_pExplBuf[i].IsActive())
		{
			m_pTextBuffer[i] = '*';
		}
		else
		{
			m_pTextBuffer[i] = '-';
		}
	}
	// ������̍Ō�͕K�� NULL �Ń^�[�~�l�[�g
	m_pTextBuffer[m_explMax] = NULL;
	return m_pTextBuffer;
}