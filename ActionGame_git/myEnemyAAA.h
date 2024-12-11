/*
	myEnemyAAA.h

	敵Ａ
*/
#pragma once

#include "myEnemyManager.h"

//////////////////
// EnemyAAA
// ここから、EnemyBaseの派生クラスを定義していく.
// EnemyBaseは抽象クラスなので、実体を生成することは出来ない.
// EnemyBaseから派生したクラスからは、具体的な実体を持つことができる.

// EnemyBaseの派生クラス：EnemyAAA（直進）.
class EnemyAAA : public EnemyBase
{
public:
	// EnemyAAAは親クラスのコンストラクタを明示的に指定している.
	// EnemyBase(pTex)が無いと、EnemyBase()を呼び出すことになるが.
	// 引数無しのコンストラクタEnemyBase::EnemyBase()は存在しないので.
	// コンパイルエラーが出る.
	EnemyAAA(MyTexture* pTex) : EnemyBase(pTex)
	{
		//_tprintf(_T("敵AAA生成 addr=%p\n"), this);
	}
	~EnemyAAA() override
	{
		//_tprintf(_T("敵AAA消滅 addr=%p\n"), this);
	}
	bool Update(float time) override;
	//void Fire() override;
	char Getch() override	{ return 'A'; }
	//int Score() override	{ return 10; }
};

