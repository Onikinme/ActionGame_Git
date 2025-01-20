/*
	myBossAAA.cpp

	ボスAAA
*/
#include "stdafx.h"
#include "myBossAAA.h"
#include "myApp.h"
#include "mySound.h"

bool BossAAA::Update(float time)
{
	//playerとの距離
	D3DXVECTOR2 pPos = {0,0};
	GetApp()->GetPlayerPos(pPos);
	//アクションが終わっている(もしくはしていない)かつあったm_attackTimerが正でない時
	if (!m_actionFlg && m_actionTimer < 0)
	{
		//PlayerとBossの距離が10.0f以内なら
		if (fabs(pPos.x - m_posX) < 60.0f)
		{
			m_nearTimer += time;
		}
		else//10.0fより遠い
		{
			m_farTimer += time;
		}

		//25.0f時間離れていたら
		if (m_farTimer > 25.0f)
		{
			//炎で追い払う
			ChangeAction(1);
			m_farTimer = 0.0f;
		}

		if (m_nearTimer > 25.0f)
		{
			//ジャンプ攻撃をする
			ChangeAction(2);
			m_nearTimer = 0.0f;
		}
	}
	
	//タイマーを進める
	return BossTmp :: Update(time);
}

//攻撃
void BossAAA::Attack() {

	//火を吐く攻撃
	/*攻撃中*/
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
			m_fireflg = false;

		}
	}
}

void BossAAA::Fire()
{

	/*
	炎を吐き出す
	FireクラスもしくはBulletクラスから弾オブジェクトを生成する。
	*/
	if (!m_fireflg)
	{
		_tprintf(_T("発射\n"));

		BulletOne* pBulletOne = GetApp()->GetEneBltBuf()->Next();
		if (pBulletOne != NULL)
		{
			float ADx = 64.0f;
			if (!m_leftFlg)//プレイヤーが右側
			{
				//ADx *= -1;
			}
			float ADy = 64.0f;
			D3DXVECTOR2 playerpos;
			float playersize;
			GetApp()->GetPlayerPos(playerpos);
			GetApp()->GetPlayerSize(playersize);
			float vx =  playerpos.x + playersize * SIZE / 2 - (m_posX + ADx);
			float vy =  playerpos.y + playersize * SIZE / 2 - (m_posY + ADy);
			float distance = sqrt(vx * vx + vy * vy);
			float dx = vx / distance;
			float dy = vy / distance;

			float speed = 5.0f;
			float fXSpeed = speed * dx;
			float fYSpeed = speed * dy;

			pBulletOne->Init(m_posX + ADx, m_posY + ADy, fXSpeed, fYSpeed, 32);
			m_fireflg = true;
			GetSoundInst()->PlaySE(SE_FIRE);
		}
		
	}
	else
	{
		//nothing to do
	}
	

}


