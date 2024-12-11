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
void EnemyBase::Init(float x, float y, float vx, float vy, int maxhp)
{
	// x (初期位置X), y (初期位置Y)
	m_pos = D3DXVECTOR2(x, y);
	m_vel = D3DXVECTOR2(vx, vy);
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
	myapp->DrawChara(pSpr, m_pTex->GetTexture(), pos, { 0, 0, 48, 48 }, 48.0f, 48.0f, false, false);
	/*
	D3DXVECTOR3 matopos(pData->Mato->GetXY(i).x, pData->Mato->GetXY(i).y, 0);
	pSpr->DrawChara(pData->pDevice, pData->pSprite, pData->pMatoTex->GetTexture(), matopos, { 0, 0, 48, 48 }, 48.0f, 48.0f, false, false);
	*/

	/*
	D3DXVECTOR3 cnt(32.0f, 32.0f, 0.0f);	// 表示時の中心位置.
	D3DXVECTOR3 pos(m_posX, m_posY, 0.0f);	// 表示位置.
	int time = (int)(m_timer / 10);			// 経過時間を10フレーム単位に換算.
	int anim = time % 4;					// テクスチャアニメーションを4パターンで.
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	pSpr->Draw(m_pTex->GetTexture(), &(g_EnemyImageArea[anim]), &cnt, &pos, 0xFFFFFFFF);
	*/
}

// 現在のXY座標値を得る.
D3DXVECTOR2 EnemyBase::GetXY()
{
	// 練習２
	// TODO メンバ変数の値を取得する処理を記述してください.
	return m_pos;
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
	HP -= damage;
}

// ダメージフラグを管理
void EnemyBase::DamageFlg(bool damage)
{
	enemy_damageflg = damage;
}

// 攻撃でダメージを負ったか？
bool EnemyBase::IsDamage()
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
		Damage(i, 9999);
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
				Damage(i, 9999);
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
			if(GetHP(&index) == 0){
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

// 最大HPを得る
int EnemyManager::GetMaxHP(int* index){
	return m_ppBuffer[*index]->GetMaxHP();
}

// 現在のHPを得る
int EnemyManager::GetHP(int* index) {
	return m_ppBuffer[*index]->GetHP();
}

void EnemyManager::DamageFlg(bool damage, int index) {
	m_ppBuffer[index]->DamageFlg(damage);
}

bool EnemyManager::IsDamage(int index) {
	return m_ppBuffer[index]->IsDamage();
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