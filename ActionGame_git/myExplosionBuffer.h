#pragma once

#include "MyExplosionOne.h"
#include "myTexture.h"

// MyExplosionBuffer ///////////////////////////////
// 爆発アニメーション管理
class ExplosionBuffer
{
private:
	ExplosionOne* m_pExplBuf;
	int m_index;
	int m_explMax;
	char* m_pTextBuffer; // デバッグ用に配列の状況を文字列にする.

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
	void Explose(float x, float y);	// 爆発を発生させる.
	ExplosionOne* Next();
	ExplosionOne* Get(int index) 
	{
		return (m_pExplBuf + index);// &m_pExplBuf[index] と同じ.
	}
	void Update(int time);
	void Draw(MyTexture* pTex);
	char* GetText();
};