/*
	myEnemyAAA.cpp

	敵Ａ
*/
#include "stdafx.h"
#include "myEnemyAAA.h"
//#include "myBulletOne.h"
//#include "myBulletBuffer.h"
#include "myApp.h"

// 移動.
bool EnemyAAA::Update(float time)
{
	EnemyBase::Update(time);		// 親のUpdateを呼び出す.


	// 位置を更新.
	m_pos.x += m_vel.x * time;
	m_pos.y += m_vel.y * time;

	// 左右の壁に当たったら反転
	if (m_pos.x > WIDTH - 64.0f) {
		m_pos.x = WIDTH - 64.0f;
		m_vel.x *= -1;
	}
	if (m_pos.x < 0.0f) {
		m_pos.x = 0.0f;
		m_vel.x *= -1;
	}

	if (m_timer > 10.0f) {
		// 一定時間後に、画面外（下）に移動すると消滅.
		if (m_pos.y > HEIGHT + 32.0f) {
			return false;
		}
		// 画面上方に移動する予定はないが保険で画面上方に移動した場合も消滅させる.
		if ( m_pos.y < 0.0f - 32.0f) {
			return false;			
		}
	}

	return true;
}

/*
// プレイヤー方向に発射.
void EnemyAAA::Fire()
{
	if ((rand() % 1000 < 10) && (m_timer < 60)) {
		// プレイヤーへのベクトルを算出.
		float px, py;
		GetApp()->GetPlayerPos(px, py);
		float dx = px - m_posX;
		float dy = py - m_posY;
		float dd = (float)sqrt(dx * dx + dy * dy);
		// 弾の速度を出す.
		dx /= dd; dy /= dd;
		dx *= 5;  dy *= 5;

		BulletOne* pBlt = GetApp()->GetEneBltBuf()->Next();
		if (pBlt != NULL) {
			pBlt->Init(m_posX, m_posY, dx, dy);
		}
	}
}
*/
