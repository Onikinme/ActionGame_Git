/*
	myBossAAA.h

	ボスAAA
*/
#pragma once

#include "myBossTmp.h"

//////////////////
// BossAAA
// ここから、BossTmpの派生クラスを定義していく.
// BossTmpは抽象クラスなので、実体を生成することは出来ない.
// BossTmpから派生したクラスからは、具体的な実体を持つことができる.

// BossTmpの派生クラス：BossAAA.
class BossAAA : public BossTmp
{
private:
	float m_nearTimer;//Playerとの近距離の時間を図る
	float m_farTimer;//Playerとの遠距離の時間を図る
	bool  m_fireflg; //true:fireが画面上,false:画面内にfire無し
public:
	// BossAAAは親クラスのコンストラクタを明示的に指定している.
	// BossTmp(pTex)が無いと、BossTmp()を呼び出すことになるが.
	// 引数無しのコンストラクタBossTmp::BossTmp()は存在しないので.
	// コンパイルエラーが出る.
	BossAAA(MyTexture* pTex) : BossTmp(pTex), 
	 m_nearTimer(0.0f),
	 m_farTimer(0.0f),
	 m_fireflg(false)
	{
	}
	~BossAAA() override
	{
	}

	//ボスのアクションパターンも含まれる
	bool Update(float time);

	//攻撃
	void Attack();
	//火(弾)発射
	void Fire();
};

