#include "stdafx.h"
#include "myBossTmp.h"
#include "myApp.h"

//ボスの動作
bool BossTmp::Update(float time)
{
	if (m_timer >= 0.0f) {
		m_timer += time;		// 時刻を進める.
	}
	return true;
}

//ボスの位置初期化
void BossTmp::Init(float posX, float posY, float vx, float vy, float w, int hp)
{
	m_posX = posX; m_posY = posY;
	m_velX = vx; m_velY = vy;
	m_w = w * 1.5f;
	m_hp = hp;
	m_timer = 0.0f;

	// 前川祐毅　追加
	m_maxhp = hp;
	//m_boss_damageflg = false;
	m_damageflg = 0x00;
}

//ボス描画
void BossTmp::Draw()
{
	switch (m_actionNum)
	{
	case 0:
		Move();
		break;
	case 1:
		Attack();
		break;
	case 2:
		Jump();
		break;
	default:

		break;
	}

	D3DXVECTOR3 cnt(32.0f, 32.0f, 0.0f);	// 表示時の中心位置.
	D3DXVECTOR3 pos(m_posX, m_posY + 1 * SIZE, 0.0f);	// 表示位置.
	int time = (int)(m_timer / 10);			// 経過時間を10フレーム単位に換算.
	int anim = time % 8;					// テクスチャアニメーションを8パターンで.
	int size = 64;
	RECT rc = { 0, m_animNum * size + 1, size, (m_animNum + 1) * size };

	D3DXVECTOR2 playerpos;
	float playersize;
	GetApp()->GetPlayerPos(playerpos);
	GetApp()->GetPlayerSize(playersize);

	if (m_posX + m_w / 2 - playerpos.x + playersize / 2 > 0)
	{
		//プレイヤーがボスより左側
		m_leftFlg = true;
	}
	else
	{
		m_leftFlg = false;
		//プレイヤーがボスより右側
		//画像反転
	}

	if (m_pTex != NULL) {
		bool damageflg = false;
		// ダメージを受けた時の点滅
		if (m_damagetimer > 0.0f) {
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
			}
		}
		GetApp()->Draw(GetApp()->GetSprite(), m_pTex->GetTexture(), pos, rc, size, size, !m_leftFlg, false, damageflg, 1.5f);
	}
}

/********************
*
* 前川祐毅　削除
* 
* MyAppのDraw関数で画像反転処理を行うので、SpriteFlipとSpriteResetは一旦削除
*
********************/
/*
void BossTmp::SpriteFlip()
{
	左右反転処理
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale,
		-1.0f,  // X軸スケール
		1.0f,  // Y軸スケール
		1.0f);                 // Z軸スケール (通常は1.0f)

	// 位置の補正 (反転時に中心がずれるのを補正)
	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation,
		m_posX * 2,
		0,
		0);

	// 合成行列 (スケール + 位置補正)
	D3DXMATRIX matTransform = matScale * matTranslation;
	// スプライトの変換行列を設定
	GetApp()->GetSprite()->SetTransform(&matTransform);
}

void BossTmp::SpriteReset()
{
	// 行列をリセット
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	GetApp()->GetSprite()->SetTransform(&matIdentity);
}
*/

// 位置(cx, cy)と自分の位置(posX, posY)は距離limit2より近いかどうか.
// limit2は距離の二乗.
bool BossTmp::Collision(float cx, float cy, float limit2)
{
	return (GetDistance2(cx, cy, m_posX, m_posY) < limit2);
}

//アクション決定
void BossTmp::ChangeAction(int a)
{
		m_actionTimer = m_timer;
		m_actionFlg = true;
		m_actionNum = a;
}

//攻撃
void BossTmp::Attack()
{
	// 継承先のクラスで行動を設計する
}

//ジャンプ(空中でPlayerをooフレーム追従後着地)
void BossTmp::Jump()
{
	if (m_actionFlg && m_actionTimer > 0.0f)
	{
		float AttckTime = m_timer - m_actionTimer;
		float fXSpeed = 10.0f;
		float fYSpeed = 15.0f;

		//ジャンプするまでの溜め時間
		if (AttckTime < 10.5f)
		{
			m_animNum = 6;
			if (AttckTime < 9.5f)// 0.0f ~ 2.5f
			{
				m_animNum = 5;
			}
		}
		else
		{
			D3DXVECTOR2 playerpos;
			float playersize;
			GetApp()->GetPlayerPos(playerpos);
			GetApp()->GetPlayerSize(playersize);
			float dx = (playerpos.x + playersize / 2) - (m_posX + m_w / 2);
			float dy = (playerpos.y + playersize / 2) - (m_posY + m_w / 2);
			float  distance = sqrt(dx * dx + dy * dy);

			//40.5f空中にいる
			if (AttckTime < 40.5f)
			{


				//高さ0まで上昇
				if (m_posY > 0)
				{
					m_posY -= fYSpeed;
				}
				else
				{
					//高さ0まで上昇後x軸移動
					//x軸上で重なっていない
					if (distance > 0)
					{
						float vx = dx / distance;
						float vy = dy / distance;

						m_posX += vx * fXSpeed;
					}
				}

			}
			else
			{
				//地面に設置したら→条件変更の可能性あり
				if (m_posY <= HEIGHT - 128 * SIZE)
				{
					m_posY += fYSpeed;
				}
				else
				{
					//アクション終了
					m_posY = HEIGHT - 128 * SIZE;
					m_actionTimer = -1.0f;
					m_actionFlg = false;
					m_animNum = 0;
					m_actionNum = 0;
				}

			}


		}


	}

}

//プレイヤーの方向に歩く
void BossTmp::Move()
{
	D3DXVECTOR2 playerpos;
	float playersize;
	GetApp()->GetPlayerPos(playerpos);
	GetApp()->GetPlayerSize(playersize);
	if (!m_actionFlg)
	{
		float speed = 1.0f;
		float dx = (playerpos.x + playersize / 2) - (m_posX + m_w / 2);
		float dy = (playerpos.y + playersize / 2) - (m_posY + m_w / 2);
		float  distance = sqrt(dx * dx + dy * dy);
		//高さ0まで上昇後x軸移動
					//x軸上で重なっていない
		if (distance > 0)
		{

			float vx = dx / distance;
			float vy = dy / distance;

			m_posX += vx * speed;

			int time = (int)(m_timer / 2);			// 経過時間を10フレーム単位に換算.
			int anim = time % 2 + 1;					// テクスチャアニメーションを8パターンで.
			m_animNum = anim;
		}
	}
}

/********************
*
* 前川祐毅　追加
* 
* ボスに攻撃を当てたときの処理
*
********************/
// 現在のXY座標値を得る.
D3DXVECTOR2 BossTmp::GetXY()
{
	// 練習２
	// TODO メンバ変数の値を取得する処理を記述してください.
	return D3DXVECTOR2(m_posX, m_posY);
}

// 現在のボスのサイズを得る.
float BossTmp::GetSize() {
	return m_w;
}

// 最大HPを得る
int BossTmp::GetMaxHP() {
	return m_maxhp;
}

// 現在のHPを得る
int BossTmp::GetHP() {
	return m_hp;
}

// ダメージを受けた時の処理
void BossTmp::Damage(int damage)
{
	m_damagetimer = m_timer;
	m_hp -= damage;
}

// ダメージフラグを管理
void BossTmp::DamageFlg(unsigned char damage)
{
	//m_boss_damageflg = damage;
	m_damageflg |= damage;
}

// ダメージフラグをリセット
void BossTmp::DamageReset(unsigned char damage) {
	m_damageflg &= damage;
}

// 攻撃でダメージを負ったか？
unsigned char BossTmp::IsDamage()
{
	return m_damageflg;
}



// 次の空き位置を探す.
int BossManager::Next()
{
	// リングバッファを、現在位置(m_index)から一周探索.
	for (int i = 0; i < m_bossMax; i++) {
		// 現在位置(m_index)から空きを検索.
		int index = (m_index + i) % m_bossMax;
		// index がm_enemyMax以上にならないよう、剰余にすることで配列の.
		// 現在位置から m_index - 1 の位置までを検索.
		if (m_ppBuffer[index] == NULL) {
			m_index = (index + 1) % m_bossMax;
			return index;
		}
	}
	return -1;
}

// デストラクタ:配列を解放している.
// 配列の中身をまず解放してから全体を解放.
BossManager::~BossManager()
{
	for (int i = 0; i < m_bossMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			delete m_ppBuffer[i];
		}
	}
	delete[] m_ppBuffer;
	delete[] m_pTextBuff;
}

// メモリの確保等の初期化.
bool BossManager::Init()
{
	if (m_ppBuffer == NULL) {
		m_ppBuffer = new BossTmp * [m_bossMax];	// 配列の中身はEnemyBase*型.
		// ↑sizeof(EnemyBase*) を max 個分のメモリを確保しているだけで、コンストラクタは呼び出されない.
		// sizeof(EnemyBase*)はx64では１つあたり8バイト、x86では１つあたり4バイトのメモリサイズ.
		if (m_ppBuffer == NULL) {
			return false;
		}

		for (int i = 0; i < m_bossMax; i++) {
			m_ppBuffer[i] = NULL;		// ポインタをNULLで初期化し、未使用状態とする.
		}
	}

	if (m_pTextBuff == NULL) {
		m_pTextBuff = new char[m_bossMax + 1];	// デバッグ用文字列も確保する.
		// +1 は終端記号'\0'の分.
		if (m_pTextBuff == NULL) {
			return false;
		}
	}

	return true;
}

// 配列の全ての要素をリセットする.
void BossManager::ResetAll()
{
	for (int i = 0; i < m_bossMax; i++) {
		if(m_ppBuffer[i] != NULL)Damage(9999, i);
	}
}
// pEnemyを配列に追加.
// EnemyBase*型を引数にしているが、実際にはEnemyAAA*型やEnemyBBB*型が送られてくる.
// これらのクラスは、EnemyBaseの派生クラスなので、EnemyBase*型で受けることができる.
// 間違って引数を(EnemyBase pEnemy) と＊を付け忘れ、ポインタではなく実体で記述すると.
// データが一部欠落する現象が発生し深刻なバグの原因となる.
bool BossManager::Add(BossTmp* pEnemy)
{
	int index = Next();		// 配列から空きを探査.
	if (0 <= index && index < m_bossMax) {
		m_ppBuffer[index] = pEnemy;// 敵を管理用の配列に登録.
		return true;
	}
	return false;
}

// 配列の各要素すべてに、処理を進めるよう指示する.
void BossManager::Update(float time)
{
	for (int i = 0; i < m_bossMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			bool flag = m_ppBuffer[i]->Update(time);
			// FLASEが返るとこの要素は不要になる.
			if (!flag) {
				Damage(9999, i);
			}
		}
	}
}

// 敵にダメージを与える
// もし敵のHPが0になったら、敵配列のindex番目を無効にする.
// m_ppBuffer[index]は基本クラスのポインタだが、デストラクタをvirtual指定しているため.
// 適切な派生クラス(EnemyAAAやEnemyBBB)のデストラクタが呼び出される.
void BossManager::Damage(int damage, int index)
{
	if (0 <= index && index < m_bossMax) {
		m_ppBuffer[index]->Damage(damage);
		if (GetHP(&index) <= 0) {
			delete m_ppBuffer[index];
			m_ppBuffer[index] = NULL;// 配列の中身をNULLクリアして次の使用に備える.
		}
	}
}

// 配列の全ての敵に対して、表示を促す.
void BossManager::Show()
{
	for (int i = 0; i < m_bossMax; i++) {
		// m_ppBuffer[i]の型はEnemyBase*だが、Show は virtual関数なので.
		// 派生クラスのshowが適切に呼び出される.
		// つまりifやswitchを使って分岐させる必要がない。これがオブジェクト指向の利点の１つ.
		if (m_ppBuffer[i] != NULL) {
			m_ppBuffer[i]->Draw();
		}
	}
}

// 現在のXY座標値を得る.
D3DXVECTOR2 BossManager::GetXY(int* index) {
	return m_ppBuffer[*index]->GetXY();
}

// 現在のボスのサイズを得る.
float BossManager::GetSize(int* index) {
	return m_ppBuffer[*index]->GetSize();
}

// 最大HPを得る
int BossManager::GetMaxHP(int* index) {
	return m_ppBuffer[*index]->GetMaxHP();
}

// 現在のHPを得る
int BossManager::GetHP(int* index) {
	return m_ppBuffer[*index]->GetHP();
}

// ダメージフラグを管理
void BossManager::DamageFlg(unsigned char damage, int* index) {
	m_ppBuffer[*index]->DamageFlg(damage);
}

// ダメージフラグをリセット
void BossManager::DamageReset(unsigned char damage, int* index) {
	m_ppBuffer[*index]->DamageReset(damage);
}

unsigned char BossManager::IsDamage(int* index) {
	return m_ppBuffer[*index]->IsDamage();
}

// 配列の大きさを返す
int BossManager::BossMax()
{
	return m_bossMax;
}

// その配列が使用可能か
bool BossManager::PPBuffer(int index) {
	if (m_ppBuffer[index] != NULL) {
		return true;
	}
	return false;
}

// (cx, cy)の位置は敵と衝突距離だったか.
// 衝突した場合は引数pIndexが差す場所に配列のインデックスを入れてTRUEを返す.
// 衝突が無かった場合はFALSE返す.
bool BossManager::Collision(float cx, float cy, float limit2, int* pIndex)
{
	for (int i = 0; i < m_bossMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			if (m_ppBuffer[i]->Collision(cx, cy, limit2)) {
				*pIndex = i;
				return true;
			}
		}
	}
	return false;
}

//アクション決定
void BossManager::ChangeAction(int a, int index) {
	m_ppBuffer[index]->ChangeAction(a);
}

// デバッグ用にm_pBuffer配列の中身をテキスト化する.
// 弾丸が有効：*.
// 弾丸が無効：-.
char* BossManager::GetText()
{
	for (int i = 0; i < m_bossMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			m_pTextBuff[i] = m_ppBuffer[i]->Getch();
		}
		else {
			m_pTextBuff[i] = '-';
		}
	}
	// 文字列の最後は必ずNULL文字でターミネート.
	m_pTextBuff[m_bossMax] = NULL;
	return m_pTextBuff;
}