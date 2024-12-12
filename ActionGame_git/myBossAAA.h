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
public:
	// BossAAAは親クラスのコンストラクタを明示的に指定している.
	// BossTmp(pTex)が無いと、BossTmp()を呼び出すことになるが.
	// 引数無しのコンストラクタBossTmp::BossTmp()は存在しないので.
	// コンパイルエラーが出る.
	BossAAA(MyTexture* pTex) : BossTmp(pTex)
	{
	}
	~BossAAA() override
	{
	}

	//攻撃
	void Attack();
	//火(弾)発射
	void Fire();
};

