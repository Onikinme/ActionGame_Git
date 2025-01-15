/*
	myBulletOne.cpp

	弾丸クラス（１発毎）
*/
#include "stdafx.h"
#include "myBulletOne.h"
#include "myApp.h"

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
void BulletOne::Init(float x, float y, float vx, float vy, float size)
{
	m_posX = x;
	m_posY = y;
	m_velX = vx;
	m_velY = vy;
	m_size = size;
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

void BulletOne::GetSize(float& size)
{
	size = m_size;
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
	if ((m_posY < 0.0f || HEIGHT < m_posY) || (m_posX < 0.0f - m_size * 2 || WIDTH < m_posX)) {
		Deactive();
	}
}

void BulletOne::Draw(MyTexture* pTex)		// 弾丸テクスチャを指定して描画.
{
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	D3DXVECTOR3 cnt(8, 8, 0);
	D3DXVECTOR3 pos(0, 0, 0);
	RECT rc[] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	};

	rc[0] = {0, 0, (int)m_size, (int)m_size};
	rc[1] = { 0, (int)m_size, (int)m_size, (int)m_size * 2 };
	rc[2] = { 0, (int)m_size * 2, (int)m_size, (int)m_size * 3 };
	rc[3] = { 0, (int)m_size, (int)m_size, (int)m_size * 2 };

	int nTime = m_timer - 1;// Drawよりも先にUpdateが呼び出されるため、前フレームの値で計算.
	int animTime = nTime / 8;	// ５フレーム毎に更新.
	int anim = animTime % 4;	// ８パターンのアニメーション.　配列のオーバーアクセス回避のため剰余算にしておく.

	// 現在位置をpos構造体型のデータに格納して表示.
	GetXY(pos.x, pos.y);
	//pSpr->Draw(pTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
	if(m_velX > 0)GetApp()->Draw(GetApp()->GetSprite(), pTex->GetTexture(), pos, rc[anim], m_size, 0.0f, false, false, false, 1);
	else GetApp()->Draw(GetApp()->GetSprite(), pTex->GetTexture(), pos, rc[anim], m_size, 0.0f, true, false, false, 1);
}