/*
	myPlayer.cpp

	プレイヤーの基底クラス
*/
#include "stdafx.h"
#include "myPlayer.h"
#include "myApp.h"
#include "myInput.h"

// プレイヤー移動テクスチャの表示領域のための配列.
const RECT g_Player_MoveArea[] = {
	{ 0, 32, 32, 64 },   // 移動１
	{ 0, 64, 32, 96 },   // 移動２
	{ 0, 96, 32, 128 },  // 移動３

    { 0, 0, 32, 32 },    // 止まっているとき

    { 0, 192, 32, 224 }, // ジャンプ中のとき
};

// プレイヤー攻撃テクスチャの表示領域のための配列.
const RECT g_Player_AttackArea[] = {
	{ 0, 128, 32, 160 }, // 攻撃１
	{ 0, 160, 32, 192 }, // 攻撃２
};

// 武器テクスチャの表示領域のための配列.
const RECT g_WeaponArea[] = {
	{ 0, 0, 48, 48 },    // 武器アニメ１
	{ 0, 48, 48, 96 },   // 武器アニメ２
};

// プレイヤーの初期化.
void Player::Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, float ww, float wh, int maxhp)
{
	m_pos = pos;					  // プレイヤーの座標
	m_vel = v_pos;					  // 横方向へのスピード ,ジャンプ力
	m_w = w, m_h = h;				  // プレイヤーのサイズ
	m_weapon_w = ww, m_weapon_h = wh; // 武器のサイズ
	MaxHP = maxhp;
	HP = MaxHP;
	player_damageflg = false;
	m_isGround = true;				  // 初期状態ではプレイヤーは地面についている
	m_gravity = 0.3f;				  // 重力
	m_timer = 0.0f;					  // タイマーを0.0fにして、有効化する. 
	m_movetimer = 0.0f;				  // タイマーを0.0fにして、有効化する. 
	m_attacktimer = -1.0f;			  // タイマーを-1.0fにして、無効化する. 
	m_damagetimer = -1.0f;			  // タイマーを-1.0fにして、無効化する. 
}

// プレイヤーの移動処理：基底クラスでは時刻を進めるだけ.
bool Player::Update(float time)
{
	if (m_timer >= 0.0f) {
		m_timer += time;		// 時刻を進める.
	}

	MyInput* pInput = GetInputInst();

	// キーボードでの操作.
	{
		if (!(player_damageflg && m_damagetimer > 0.0f && m_timer - m_damagetimer < 1.0f)) {
			// Aキーで左に動く
			if (pInput->IsPushKey(DIK_A) && !pInput->IsPushKey(DIK_D)) {
				m_scaling = true; // 左向き
				// 空中にいて、かつ右方向への慣性が働いているとき、
				// 左方向に動きやすくなる
				if (!m_isGround && 0 < m_vel.x) {
					m_vel.x -= 0.3f;
				}
				else m_vel.x -= 0.1f;
				if (m_vel.x < -5.0f)m_vel.x = -5.0f;
			}
			// Dキーで右に動く
			else if (pInput->IsPushKey(DIK_D)) {
				m_scaling = false; // 右向き
				// 空中にいて、かつ左方向への慣性が働いているとき、
				// 右方向に動きやすくなる
				if (!m_isGround && 0 > m_vel.x) {
					m_vel.x += 0.3f;
				}
				else m_vel.x += 0.1f;
				if (m_vel.x > 5.0f)m_vel.x = 5.0f;
			}

			if (pInput->IsPushKey(DIK_A) || pInput->IsPushKey(DIK_D)) {
				if (!m_move) {
					m_movetimer = m_timer;
					m_move = true;
				}
			}
			else {
				m_move = false;
			}

			// キーを離した時の減速調整
			if (!pInput->IsPushKey(DIK_A) && 0 > m_vel.x) {
				if (m_isGround) { // 地上ではブレーキが良く効く
					m_vel.x += 0.1f;
					m_vel.x *= 0.95f;
				}
				else { // 空中では横方向に慣性が働くため、ブレーキが効きづらい
					m_vel.x += 0.01f;
					m_vel.x *= 0.99f;
				}
				if (m_vel.x > 0) m_vel.x = 0;
			}
			if (!pInput->IsPushKey(DIK_D) && 0 < m_vel.x) {
				if (m_isGround) { // 地上ではブレーキが良く効く
					m_vel.x -= 0.1f;
					m_vel.x *= 0.95f;
				}
				else { // 空中では横方向に慣性が働くため、ブレーキが効きづらい
					m_vel.x -= 0.01f;
					m_vel.x *= 0.99f;
				}
				if (m_vel.x < 0) m_vel.x = 0;
			}
		}

		// 移動中
		if ((int)(m_timer - m_movetimer) % 4 >= 3) {
			m_moveanim = 2;
		}
		else if ((int)(m_timer - m_movetimer) % 4 >= 2) {
			m_moveanim = 1;
		}
		else if ((int)(m_timer - m_movetimer) % 4 >= 1) {
			m_moveanim = 0;
		}
		else if ((int)(m_timer - m_movetimer) % 4 >= 0) {
			m_moveanim = 1;
		}

		if (m_pos.x * SIZE < 0) {
			m_pos.x = 0;
			m_vel.x = 0;
		}
		if (m_pos.x > WIDTH - m_w * SIZE) {
			m_pos.x = WIDTH - m_w * SIZE;
			m_vel.x = 0;
		}

		m_pos.x += m_vel.x;

		// プレイヤーが地面に着いていたらSPACEキーでジャンプ可能
		if (m_isGround) {
			if (pInput->IsPushKey(DIK_SPACE) && m_pos.y == HEIGHT - 64 * SIZE && !m_jumpbutton) {
				m_vel.y = -10.0f;  // 初速度の設定
				//Y座標の更新
				m_pos.y += m_vel.y;
				m_jumpbutton = true;
				m_isGround = false;
			}
		}
		// ジャンプ中
		else {
			//Y方向の速度に加速度を加える
			m_vel.y += m_gravity;
			//Y座標の更新
			m_pos.y += m_vel.y;

			//上昇中にボタンを離したときの処理
			if (!pInput->IsPushKey(DIK_SPACE))m_shortjump = true;
			if (m_vel.y < 0 && m_shortjump)m_vel.y *= 0.5f;
		}

		// ジャンプボタン長押しでジャンプし続けてしまわないようにチェック
		if (!pInput->IsPushKey(DIK_SPACE) && m_jumpbutton && m_pos.y == HEIGHT - 64 * SIZE) {
			m_jumpbutton = false;
		}

		// 地面に着いたかチェック
		if (m_pos.y >= HEIGHT - 64 * SIZE) {
			m_pos.y = HEIGHT - 64 * SIZE;
			m_isGround = true;
			m_shortjump = false;
		}

		// Pキーで攻撃
		if (pInput->IsPushKey(DIK_P) && !m_attackflg) {
			m_attackflg = true;
			m_attacktimer = m_timer;
		}

		// 攻撃中
		if (m_attackflg && m_attacktimer > 0.0f) {
			if (m_timer - m_attacktimer < 0.5f) { // 0.0f ~ 0.5f
				m_attackanim = 0;
			}
			else if (m_timer - m_attacktimer < 2.5f) { // 0.5f ~ 2.5f
				m_attackanim = 1;
				if (!m_weaponflg)m_weaponflg = true;
				if (m_timer - m_attacktimer < 1.5f) m_weaponanim = 0; // 0.5f ~ 1.5f
				else if (m_timer - m_attacktimer < 2.5f)m_weaponanim = 1; // 1.5f ~ 2.5f
			}
			else if (m_timer - m_attacktimer < 3.0f) { // 2.5f ~ 3.0f
				m_attackanim = 0;
			}
			else { // 3.0f ~
				m_weaponanim = 0;
				m_attacktimer = -2.0f; // 攻撃が終了したことをmyAppに知らせるため、
									   // -1.0fではなく-2.0fで初期化する
				m_weaponflg = false;
			}
		}

		// 攻撃ボタン長押しで攻撃し続けてしまわないようにチェック
		if (!pInput->IsPushKey(DIK_P) && m_attackflg && m_attacktimer == -1.0f) {
			m_attackflg = false;
		}
	}

	// 武器の座標は、プレイヤーの進行方向に設置する
	int scale = 1;
	if (m_scaling)scale = -1;
	m_weapon_pos = D3DXVECTOR2(m_pos.x - (8.0f * SIZE) + 30 * SIZE * scale, m_pos.y - 8.0f * SIZE);

	return true;
}

// キャラクタを表示する.
void Player::Show() {
	assert(m_timer >= 0.0f);// タイマーが負の状態で呼ばれるべきではないのでアサートを仕掛けておく.
	if (m_timer < 0.0f) {
		return;
	}

	MyApp* myapp = GetApp();
	IDirect3DDevice9* pDev = GetApp()->GetDevice();
	ID3DXSprite* pSpr = GetApp()->GetSprite();

	// プレイヤーの描画.
	D3DXVECTOR3 pos(m_pos.x, m_pos.y, 0);
	D3DXVECTOR3 weaponpos(m_weapon_pos.x, m_weapon_pos.y, 0);
	RECT rc = { 0, 0, 0, 0 };
	RECT weaponrc = { 0, 0, 0, 0 };

	// 攻撃中
	if (m_attacktimer > 0.0f) {
		rc = g_Player_AttackArea[m_attackanim];
		weaponrc = g_WeaponArea[m_weaponanim];
		// プレイヤーの武器を描画
		// 反転にも対応
		if (m_weaponflg && m_wTex != NULL)myapp->Draw(pSpr, m_wTex->GetTexture(),
			weaponpos, weaponrc, m_weapon_w, m_weapon_h, m_scaling, false, false);
	}
	// 攻撃中でないとき
	else {
		// 地面に着いているとき
		if (m_isGround) {
			// 歩いているとき
			if (m_vel.x != 0 || m_move) {
				rc = g_Player_MoveArea[m_moveanim];
			}
			// 止まっているとき
			else rc = g_Player_MoveArea[3];
		}
		// ジャンプ中のとき
		else rc = g_Player_MoveArea[4];
	}
	if (m_pTex != NULL) {
		// プレイヤーを描画
		// 反転にも対応

		bool damageflg = false;
		// ダメージを受けた時の点滅
		if (player_damageflg && m_damagetimer > 0.0f) {
			if (m_timer - m_damagetimer < 0.5f) damageflg = true;
			else if (m_timer - m_damagetimer < 1.0f) {}
			else if (m_timer - m_damagetimer < 1.5f) damageflg = true;
			else if (m_timer - m_damagetimer < 2.0f) {}
			else if (m_timer - m_damagetimer < 2.5f) damageflg = true;
			else if (m_timer - m_damagetimer < 3.0f) {}
			else if (m_timer - m_damagetimer < 3.5f) damageflg = true;
			else if (m_timer - m_damagetimer < 4.0f) {}
			else if (m_timer - m_damagetimer < 4.5f) damageflg = true;
			else {
				m_damagetimer = -1.0f;
				DamageFlg(false);
			}
		}
		myapp->Draw(pSpr, m_pTex->GetTexture(), pos, rc, m_w, m_h, m_scaling, false, damageflg);
	}
}

// 現在のプレイヤーのXY座標値を得る.
D3DXVECTOR2 Player::GetXY()
{
	return m_pos;
}

// 現在のプレイヤーのサイズを得る.
float Player::GetSize()
{
	return m_w;
}

// 現在の武器のXY座標値を得る.
D3DXVECTOR2 Player::GetWeaponXY()
{
	return m_weapon_pos;
}

// 現在の武器の横幅を得る.
float Player::GetWeaponW()
{ 
	return m_weapon_w;
}

// 現在の武器の縦幅を得る.
float Player::GetWeaponH()
{
	return m_weapon_h;
}

// 最大HPを得る
int Player::GetMaxHP()
{
	return MaxHP;
}

// 現在のHPを得る
int Player::GetHP()
{
	return HP;
}

// ダメージを受けた時の処理
void Player::Damage(int damage, bool direct)
{
	m_damagetimer = m_timer;
	if(direct)m_vel.x = -5.0f;
	else m_vel.x = 5.0f;
	HP -= damage;
	if (HP < 0) HP = 0;
}

// ダメージフラグを管理
void Player::DamageFlg(bool damage)
{
	player_damageflg = damage;
}

// 攻撃でダメージを負ったか？
bool Player::IsDamage()
{
	return player_damageflg;
}

// 武器を出しているか？
bool  Player::IsWeapon()
{
	return m_weaponflg;
}

// 攻撃が終了したか知らせる関数
bool Player::AttackEnd()
{
	if (m_attacktimer == -2.0f)
	{
		m_attacktimer = -1.0f;
		return true;
	}
	return false;
}