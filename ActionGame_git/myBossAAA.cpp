/*
	myBossAAA.cpp

	ボスAAA
*/
#include "stdafx.h"
#include "myBossAAA.h"
#include "myApp.h"

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


		}
	}
}

void BossAAA::Fire()
{

	/*
	炎を吐き出す
	FireクラスもしくはBulletクラスから弾オブジェクトを生成する。
	*/
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

/*
void BossAAA::Fire()
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
