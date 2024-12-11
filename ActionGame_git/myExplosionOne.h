#pragma once

#include "myTexture.h"

// ExplosionOne //////////////////////////////////
// 爆発１つのアニメーション
class ExplosionOne
{
private:
	float m_posX, m_posY;
	int m_ExplTimer;

public:
	ExplosionOne() : m_posX(0.0f), m_posY(0.0f), m_ExplTimer(-1)
	{
		_tprintf(_T("爆発生成 addr=%p\n"), this);
	}
	virtual ~ExplosionOne();

	void Start(float x, float y); // 爆発開始.
	void Update(int tt);		  // 時刻（フレーム）更新.
	void Draw(MyTexture*);		  // 爆発アニメの描画.
	void Deactive()				  // 無効にする.
	{
		m_ExplTimer = -1.0f;
	}
	bool IsActive()
	{
		return (m_ExplTimer >= 0);
	}
};