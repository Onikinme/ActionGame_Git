/*
	myPlayer.h

	プレイヤーの基底クラス
*/
#pragma once

#include "myTexture.h"

// プレイヤー移動テクスチャの表示領域のための配列.
extern const RECT g_Player_MoveArea[];

// プレイヤー攻撃テクスチャの表示領域のための配列.
extern const RECT g_Player_AttackArea[];

// プレイヤー武器テクスチャの表示領域のための配列.
extern const RECT g_WeaponArea[];

//////////////////////////////////
// Player
// プレイヤーを管理する基本クラス.
class Player
{
private:
	MyTexture* m_pTex;			  // 表示するテクスチャ.
	MyTexture* m_wTex;			  // 表示する武器テクスチャ.
	D3DXVECTOR2 m_pos;			  // プレイヤーの座標
	D3DXVECTOR2 m_vel;			  // プレイヤーの移動座標
	D3DXVECTOR2 m_weapon_pos;	  // プレイヤーの武器の座標
	int MaxHP;					  // プレイヤーの最大体力
	int HP;						  // プレイヤーの現在体力
	float m_timer;				  // 生成してからの時刻.
	float m_movetimer;			  // 移動時間
	float m_attacktimer;		  // 攻撃時間
	float m_w, m_h;				  // プレイヤーのサイズ
	float m_weapon_w, m_weapon_h; // プレイヤーの武器のサイズ
	bool player_damageflg;		  // プレイヤーがダメージを受けたか（同じ攻撃で複数回ダメージを受けないようにする）
	bool m_scaling;				  // スケール（trueなら左向き、falseなら右向き）
	bool m_move;				  // 左右キーを押している
	bool m_isGround;			  // 地面に着いているか？
	float m_gravity;			  // 重力
	float m_jumpbutton;			  // ジャンプボタン押下フラグ
	float m_shortjump;			  // ジャンプ上昇中にジャンプボタンを離したフラグ
	bool m_attackflg;			  // 攻撃中であるというフラグ
	int m_moveanim;				  // 移動アニメーション
	int m_attackanim;			  // 攻撃アニメーション
	int m_weaponanim;			  // 武器アニメーション
	bool m_weaponflg;			  // 武器を表示するフラグ

public:
// コンストラクタ
	Player(MyTexture* pTex, MyTexture* wTex)
		: m_pTex(pTex)
		, m_wTex(wTex)
		, m_pos(0.0f, 0.0f)
		, m_vel(0.0f, 0.0f)
		, m_weapon_pos(0.0f, 0.0f)
		, MaxHP(0)
		, HP(0)
		, m_timer(-1.0f)
		, m_movetimer(-1.0f)
		, m_attacktimer(-1.0f)
		, m_w(0.0f)
		, m_h(0.0f)
		, m_weapon_w(0.0f)
		, m_weapon_h(0.0f)
		, player_damageflg(false)
		, m_scaling(false)
		, m_move(false)
		, m_isGround(false)
		, m_gravity(0.0f)
		, m_jumpbutton(0.0f)
		, m_shortjump(0.0f)
		, m_attackflg(false)
		, m_moveanim(0)
		, m_attackanim(0)
		, m_weaponanim(0)
		, m_weaponflg(false)
	{
	}

	// デストラクタ.
	~Player() {}

	// プレイヤーの初期化.
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, float ww, float wh, int maxhp);

	// プレイヤーキャラの動作.
	bool Update(float time);

	// デバッグ用に文字を返す.
	char Getch() { return '*'; }

	// キャラクタを表示する.
	void Show();

	// 現在のプレイヤーのXY座標値を得る.
	D3DXVECTOR2 GetXY();

	// 現在のプレイヤーのサイズを得る.
	float GetSize();

	// 現在の武器のXY座標値を得る.
	D3DXVECTOR2 GetWeaponXY();

	// 現在の武器の横幅を得る.
	float GetWeaponW();

	// 現在の武器の縦幅を得る.
	float GetWeaponH();

	// 最大HPを得る
	 int GetMaxHP();

	// 現在のHPを得る
	 int GetHP();

	// ダメージを受けた時の処理
	 void Damage(int damage);

	// ダメージフラグを管理
	 void DamageFlg(bool damage);

	// 攻撃でダメージを負ったか？
	 bool IsDamage();

	 // 武器を出しているか？
	 bool IsWeapon();

	 // 攻撃が終了したか知らせる関数
	 bool AttackEnd();

	// タイマーが0以上なら有効（インライン関数）.
	//virtual bool IsActive();
};