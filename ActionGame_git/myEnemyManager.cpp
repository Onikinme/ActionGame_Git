/*
	myEnemyManager.cpp

	敵の基底クラスと管理クラス
*/
#include "stdafx.h"
#include "myEnemyManager.h"
#include "myApp.h"

// 敵の移動処理：基底クラスでは時刻を進めるだけ.
bool EnemyBase::Update(float time)
{
	if (m_timer >= 0.0f) {
		m_timer += time;		// 時刻を進める.
	}
	return true;
}

// 敵の初期化.
void EnemyBase::Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, int maxhp)
{
	// x (初期位置X), y (初期位置Y)
	m_pos = pos;
	m_vel = v_pos;
	m_w = w, m_h = h;				  // 敵のサイズ
	MaxHP = maxhp;
	HP = MaxHP;
	enemy_damageflg = false;
	m_timer = 0.0f;	// タイマーを0.0fにして、有効化する. 
}

void EnemyBase::Show()
{
	assert(m_timer >= 0.0f);// タイマーが負の状態で呼ばれるべきではないのでアサートを仕掛けておく.
	if (m_timer < 0.0f) {
		return;
	}

	D3DXVECTOR3 pos(m_pos.x, m_pos.y, 0);
	MyApp* myapp = GetApp();
	ID3DXSprite* pSpr = GetApp()->GetSprite();

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
		myapp->Draw(pSpr, m_pTex->GetTexture(), pos, { 0, 0, (long)m_w, (long)m_h }, m_w, m_h, false, false, damageflg, 1);
	}
}

// 現在のXY座標値を得る.
D3DXVECTOR2 EnemyBase::GetXY()
{
	// 練習２
	// TODO メンバ変数の値を取得する処理を記述してください.
	return m_pos;
}

// 現在の敵のサイズを得る.
float EnemyBase::GetSize()
{
	return m_w;
}

// 最大HPを得る
int EnemyBase::GetMaxHP() {
	return MaxHP;
}

// 現在のHPを得る
int EnemyBase::GetHP() {
	return HP;
}

// ダメージを受けた時の処理
void EnemyBase::Damage(int damage)
{
	m_damagetimer = m_timer;
	HP -= damage;
}

// ダメージフラグを管理
void EnemyBase::DamageFlg(unsigned char damage)
{
	enemy_damageflg |= damage;
}

// ダメージフラグをリセット
void EnemyBase::DamageReset(unsigned char damage) {
	enemy_damageflg &= damage;
}

// 攻撃でダメージを負ったか？
unsigned char EnemyBase::IsDamage()
{
	return enemy_damageflg;
}

/*
// タイマーが0以上なら有効（インライン関数）.
bool EnemyBase::IsActive()
{
	// 練習２
	// TODO
	return (m_timer >= 0.0f);
}
*/

// 位置(cx, cy)と自分の位置(posX, posY)は距離limit2より近いかどうか.
// limit2は距離の二乗.
bool EnemyBase::Collision(float cx, float cy, float limit2)
{
	return (GetDistance2(cx, cy, m_pos.x, m_pos.y) < limit2);
}

// 次の空き位置を探す.
int EnemyManager::Next()
{
	// リングバッファを、現在位置(m_index)から一周探索.
	for (int i = 0; i < m_enemyMax; i++) {
		// 現在位置(m_index)から空きを検索.
		int index = (m_index + i) % m_enemyMax;
		// index がm_enemyMax以上にならないよう、剰余にすることで配列の.
		// 現在位置から m_index - 1 の位置までを検索.
		if (m_ppBuffer[index] == NULL) {
			m_index = (index + 1) % m_enemyMax;
			return index;
		}
	}
	return -1;
}

// デストラクタ:配列を解放している.
// 配列の中身をまず解放してから全体を解放.
EnemyManager::~EnemyManager()
{
	for (int i = 0; i < m_enemyMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			delete m_ppBuffer[i];
		}
	}
	delete[] m_ppBuffer;
	delete[] m_pTextBuff;
}

// メモリの確保等の初期化.
bool EnemyManager::Init()
{
	if (m_ppBuffer == NULL) {
		m_ppBuffer = new EnemyBase * [m_enemyMax];	// 配列の中身はEnemyBase*型.
		// ↑sizeof(EnemyBase*) を max 個分のメモリを確保しているだけで、コンストラクタは呼び出されない.
		// sizeof(EnemyBase*)はx64では１つあたり8バイト、x86では１つあたり4バイトのメモリサイズ.
		if (m_ppBuffer == NULL) {
			return false;
		}

		for (int i = 0; i < m_enemyMax; i++) {
			m_ppBuffer[i] = NULL;		// ポインタをNULLで初期化し、未使用状態とする.
		}
	}

	if (m_pTextBuff == NULL) {
		m_pTextBuff = new char[m_enemyMax + 1];	// デバッグ用文字列も確保する.
		// +1 は終端記号'\0'の分.
		if (m_pTextBuff == NULL) {
			return false;
		}
	}

	return true;
}

// 配列の全ての要素をリセットする.
void EnemyManager::ResetAll()
{
	for (int i = 0; i < m_enemyMax; i++) {
		delete m_ppBuffer[i];
		m_ppBuffer[i] = NULL;// 配列の中身をNULLクリアして次の使用に備える.
	}
}
// pEnemyを配列に追加.
// EnemyBase*型を引数にしているが、実際にはEnemyAAA*型やEnemyBBB*型が送られてくる.
// これらのクラスは、EnemyBaseの派生クラスなので、EnemyBase*型で受けることができる.
// 間違って引数を(EnemyBase pEnemy) と＊を付け忘れ、ポインタではなく実体で記述すると.
// データが一部欠落する現象が発生し深刻なバグの原因となる.
bool EnemyManager::Add(EnemyBase* pEnemy)
{
	int index = Next();		// 配列から空きを探査.
	if (0 <= index && index < m_enemyMax) {
		m_ppBuffer[index] = pEnemy;// 敵を管理用の配列に登録.
		return true;
	}
	return false;
}

// 配列の各要素すべてに、処理を進めるよう指示する.
void EnemyManager::Update(float time)
{
	for (int i = 0; i < m_enemyMax; i++) {
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
void EnemyManager::Damage(int damage, int index)
{
	if (0 <= index && index < m_enemyMax) {
		m_ppBuffer[index]->Damage(damage);
		if (GetHP(&index) <= 0) {
			delete m_ppBuffer[index];
			m_ppBuffer[index] = NULL;// 配列の中身をNULLクリアして次の使用に備える.
		}
	}
}

// 配列の全ての敵に対して、表示を促す.
void EnemyManager::Show()
{
	for (int i = 0; i < m_enemyMax; i++) {
		// m_ppBuffer[i]の型はEnemyBase*だが、Show は virtual関数なので.
		// 派生クラスのshowが適切に呼び出される.
		// つまりifやswitchを使って分岐させる必要がない。これがオブジェクト指向の利点の１つ.
		if (m_ppBuffer[i] != NULL) {
			m_ppBuffer[i]->Show();
		}
	}
}

// 現在のXY座標値を得る.
D3DXVECTOR2 EnemyManager::GetXY(int* index) {
	return m_ppBuffer[*index]->GetXY();
}

// 現在の敵のサイズを得る.
float EnemyManager::GetSize(int* index)
{
	return m_ppBuffer[*index]->GetSize();
}

// 最大HPを得る
int EnemyManager::GetMaxHP(int* index){
	return m_ppBuffer[*index]->GetMaxHP();
}

// 現在のHPを得る
int EnemyManager::GetHP(int* index) {
	return m_ppBuffer[*index]->GetHP();
}

void EnemyManager::DamageFlg(unsigned char damage, int *index) {
	m_ppBuffer[*index]->DamageFlg(damage);
}

// ダメージフラグをリセット
void EnemyManager::DamageReset(unsigned char damage, int* index) {
	m_ppBuffer[*index]->DamageReset(damage);
}

unsigned char EnemyManager::IsDamage(int* index) {
	return m_ppBuffer[*index]->IsDamage();
}

// 配列の大きさを返す
int EnemyManager::EnemyMax()
{
	return m_enemyMax;
}

// その配列が使用可能か
bool EnemyManager::PPBuffer(int index) {
	if (m_ppBuffer[index] != NULL) {
		return true;
	}
	return false;
}

// デバッグ用にm_pBuffer配列の中身をテキスト化する.
// 弾丸が有効：*.
// 弾丸が無効：-.
char* EnemyManager::GetText()
{
	for (int i = 0; i < m_enemyMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			m_pTextBuff[i] = m_ppBuffer[i]->Getch();
		}
		else {
			m_pTextBuff[i] = '-';
		}
	}
	// 文字列の最後は必ずNULL文字でターミネート.
	m_pTextBuff[m_enemyMax] = NULL;
	return m_pTextBuff;
}

// (cx, cy)の位置は敵と衝突距離だったか.
// 衝突した場合は引数pIndexが差す場所に配列のインデックスを入れてTRUEを返す.
// 衝突が無かった場合はFALSE返す.
bool EnemyManager::Collision(float cx, float cy, float limit2, int* pIndex)
{
	for (int i = 0; i < m_enemyMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			if (m_ppBuffer[i]->Collision(cx, cy, limit2)) {
				*pIndex = i;
				return true;
			}
		}
	}
	return false;
}