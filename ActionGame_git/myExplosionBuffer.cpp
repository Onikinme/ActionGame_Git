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
		m_pTextBuffer = new char[m_explMax + 1];// デバッグ用文字列も確保する。文字列なので＋１が必要。終端記号'\0'のため。
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

// 爆発を発生させる
void ExplosionBuffer::Explose(float x, float y)
{
	ExplosionOne* pExplOne = Next();
	if (pExplOne)
	{
		pExplOne->Start(x - 32, y - 32);
		_tprintf(_T("爆発 explosion=%p\n"), pExplOne);
	}
}

ExplosionOne* ExplosionBuffer::Next()
{
	// リングバッファを、現在位置(m_index)から一周探索.
	for (int i = 0; i < m_explMax; i++)
	{
		// 現在位置(m_index)から空きを検索.
		int index = (m_index + i) % m_explMax;
		// index がm_explMax以上にならないよう、剰余にすることで配列の
		// 現在位置から m_index - 1 の位置までを検索.
		if (!m_pExplBuf[index].IsActive())
		{
			m_index = (index + 1) % m_explMax;
			return (m_pExplBuf + index);
		}
	}
	// 空きなし
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
		// 有効なら表示.
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
	// 文字列の最後は必ず NULL でターミネート
	m_pTextBuffer[m_explMax] = NULL;
	return m_pTextBuffer;
}