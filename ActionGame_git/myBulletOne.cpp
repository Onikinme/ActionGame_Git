/*
	myBulletOne.cpp

	弾丸クラス（１発毎）
*/
#include "stdafx.h"
#include "myBulletOne.h"

// BulletOneクラスのコンストラクタ.
// メンバ変数を初期化.
BulletOne::BulletOne()
	: m_posX(0.0f)
	, m_posY(0.0f)
	, m_velX(0.0f)
	, m_velY(0.0f)
	, m_timer(-1.0f)
	, m_player(false)
{}

// BulletOneクラスのデストラクタ.
BulletOne::~BulletOne()
{}

// 弾丸の初期化.
// x 	初期位置X座標.
// y 	初期位置Y座標.
// vx	初速度X成分.
// vy	初速度Y成分.
void BulletOne::Init(float x, float y, float vx, float vy)
{
	m_posX = x;
	m_posY = y;
	m_velX = vx;
	m_velY = vy;
	m_timer = 0.0f;	// timer >= 0.0f で有効な弾丸となる.
	//m_player = player;

}

// 現在位置を取得する。引数が参照型になっているため、引数戻り値となる.
// 複数の値を返したい場合には、引数を使って値を返す方法を取ることが多い.
// x 	現在位置のX成分.
// y 	現在位置のY成分.
void BulletOne::GetXY(float& x, float& y)
{
	x = m_posX;
	y = m_posY;
}

// 速度成分を時刻time分だけ加算して、弾丸の位置を更新する.
// ただし、生成されてからの時刻timerが0以上の場合のみ.
// また、現在位置のY成分が負になったら、timerを負の値にして弾丸を未使用状態にする.
void BulletOne::Update(float time)
{
	if (IsActive()) {
		m_posX += m_velX * time;
		m_posY += m_velY * time;
		m_timer += time;
	}

	// 画面外に出たら弾丸を無効にする.
	if ((m_posY < 0.0f || HEIGHT < m_posY) || (m_posX < 0.0f || WIDTH < m_posX)) {
		Deactive();
	}
}
