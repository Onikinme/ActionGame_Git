/*
	myBulletOne.h

	弾丸クラス（１発毎）
*/
#pragma once

//////////////////////////////////
// BulletOne
// 弾丸のためのクラス：それぞれの弾丸を初期化、移動など弾丸の管理.
class BulletOne
{
private:
	float m_posX, m_posY;	// 現在位置.
	float m_velX, m_velY;	// 弾丸速度.
	float m_timer;			// 弾丸射出からの時刻.
	bool m_player;          // プレイヤーの弾か？

public:
	BulletOne();			// コンストラクタ.
	virtual ~BulletOne();
	void Init(float x, float y, float vx, float vy);	// 射出初期化.
	void GetXY(float& x, float& y);	// 現在のXY座標値を得る.
	void Update(float time);		// 弾丸を進める.
	float GetTime()					// 時刻を得る.
	{
		return m_timer;
	}
	//bool IsPlayer(){
	void Deactive()			// 無効にする.
	{
		m_timer = -1.0f;
	}
	bool IsActive()			// タイマーが0以上ならば有効な弾丸.
	{
		return (m_timer >= 0.0f);
	}
};
