/*
	myFloorSlip.h

	すりぬけ床
*/
#pragma once

#include "myFloorManager.h"

//////////////////
// FloorSlip
// ここから、FloorBaseの派生クラスを定義していく.
// FloorBaseは抽象クラスなので、実体を生成することは出来ない.
// FloorBaseから派生したクラスからは、具体的な実体を持つことができる.

// FloorBaseの派生クラス：FloorSlip.
class FloorSlip : public FloorBase
{
private:
public:
	// FloorSlipは親クラスの明示的に指定している.
	// FloorBase(pTex)が無いと、コンストラクタを()を呼び出すことになるが.
	// 引数無しのコンストラクタFloorBase::FloorBase()は存在しないので.
	// コンパイルエラーが出る.
	FloorSlip(MyTexture* pTex) : FloorBase(pTex)
	{
	}
	~FloorSlip() override
	{
	}

	bool Update(float time);

	// プレイヤーがこのすりぬけ床に乗ったか判定する関数
	void Collision();
};

