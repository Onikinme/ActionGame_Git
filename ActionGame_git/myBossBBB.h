/*
	myBossBBB.h

	ボスBBB
*/
#pragma once
#include "myBossTmp.h"

//////////////////
// BossBBB
// BossTmpの派生クラス：BossAAA.
class BossBBB : public BossTmp
{
private:
	float m_nearTimer;//Playerとの近距離の時間を図る
	float m_farTimer;//Playerとの遠距離の時間を図る
	bool  m_dashflg;
	float m_fXSpeed;
	float m_fYSpeed;

public:
	// BossBBBは親クラスのコンストラクタを明示的に指定している.
	// BossTmp(pTex)が無いと、BossTmp()を呼び出すことになるが.
	// 引数無しのコンストラクタBossTmp::BossTmp()は存在しないので.
	// コンパイルエラーが出る.
	BossBBB(MyTexture* pTex) : BossTmp(pTex),
		m_nearTimer(0.0f),
		m_farTimer(0.0f),
		m_dashflg(false),
		m_fXSpeed(0.0f),
		m_fYSpeed(0.0f)
	{
	}
	~BossBBB() override
	{
	}

	//ボスのアクションパターンも含まれる
	bool Update(float time);

	//攻撃
	void Attack();

	//ダッシュ
	void Dash();
};
