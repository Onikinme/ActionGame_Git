/*
	myFloorSlip.cpp

	すりぬけ床
*/
#include "stdafx.h"
#include "myFloorSlip.h"
#include "myApp.h"

bool FloorSlip::Update(float time)
{
	Collision();

	return true;
}

// プレイヤーがこのすりぬけ床に乗ったか判定する関数
void FloorSlip::Collision() {

	// MyAppを通じてプレイヤーの座標とサイズを取得
	D3DXVECTOR2 playerpos;
	float playersize;
	playerpos = GetApp()->GetPlayer()->GetXY();
	playersize = GetApp()->GetPlayer()->GetSize();

	// プレイヤーが上からすりぬけ床に降ってきた時
	if (m_pos.x < playerpos.x + playersize && playerpos.x < m_pos.x + m_w && m_pos.y - m_h < playerpos.y && m_pos.y - m_h + 8 > playerpos.y && GetApp()->GetPlayer()->IsFall()) {
		GetApp()->GetPlayer()->SetXY(D3DXVECTOR2(playerpos.x, m_pos.y - m_h));
		// プレイヤーが接地したことにする
		GetApp()->GetPlayer()->IsGround();
		// プレイヤーがこの床に乗ったというフラグをtrueにする
		if(!floorflg)floorflg = true;
	}

	// この床からジャンプしたらfloorflgをfalseにする
	if (floorflg && m_pos.y - m_h > playerpos.y) {
		floorflg = false;
	}

	// プレイヤーがこの床の横側から降りたとき
	if (floorflg && (m_pos.x > playerpos.x + playersize || playerpos.x > m_pos.x + m_w)) {
		// プレイヤーが空中にいることにする
		GetApp()->GetPlayer()->IsAir();
		floorflg = false;
	}
}

/*

//攻撃
void FloorSlip::Attack() {

	//火を吐く攻撃
	攻撃中
	if (m_actionFlg && m_actionTimer > 0.0f)
	{
		float AttckTime = m_timer - m_actionTimer;
		if (AttckTime < 8.5f)// 0.0f ~ 2.5f
		{

			m_animNum = 3;
		}
		else if (AttckTime < 10.5f)  // 2.5f ~ 12.5f
		{
			m_animNum = 4;
			//m_animNum = 3;
			//火発射
			Fire();

		}
		else if (AttckTime < 15.0f)// 12.5f ~ 15.0f
		{

		}
		else  if (AttckTime > 17.0f)
		{
			//アクション終了
			m_actionTimer = -1.0f;
			m_actionFlg = false;
			m_animNum = 0;
			m_actionNum = 0;


		}
	}
}

void FloorSlip::Fire()
{

	
	炎を吐き出す
	FireクラスもしくはBulletクラスから弾オブジェクトを生成する。
	
	_tprintf(_T("発射\n"));

	BulletOne* pBulletOne = GetApp()->GetEneBltBuf()->Next();
	if (pBulletOne != NULL)
	{
		float ADx = 10.0f;
		float fXSpeed = -5.0f;
		if (!m_leftFlg)//プレイヤーが右側
		{
			fXSpeed *= -1;
			ADx *= -1;
		}
		float fYSpeed = 0.0f;
		pBulletOne->Init(m_posX - ADx, m_posY, fXSpeed, -fYSpeed);
	}

}
*/