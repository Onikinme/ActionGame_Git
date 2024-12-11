#pragma once

#include "MyExplosionOne.h"
#include "myTexture.h"

// MyExplosionBuffer ///////////////////////////////
// �����A�j���[�V�����Ǘ�
class ExplosionBuffer
{
private:
	ExplosionOne* m_pExplBuf;
	int m_index;
	int m_explMax;
	char* m_pTextBuffer; // �f�o�b�O�p�ɔz��̏󋵂𕶎���ɂ���.

public:
	ExplosionBuffer(int max)
		: m_pExplBuf(NULL)
		, m_index(0)
		, m_explMax(max)
		, m_pTextBuffer(NULL)
	{}
	virtual ~ExplosionBuffer();

	bool Init();
	void ResetAll();
	void Explose(float x, float y);	// �����𔭐�������.
	ExplosionOne* Next();
	ExplosionOne* Get(int index) 
	{
		return (m_pExplBuf + index);// &m_pExplBuf[index] �Ɠ���.
	}
	void Update(int time);
	void Draw(MyTexture* pTex);
	char* GetText();
};