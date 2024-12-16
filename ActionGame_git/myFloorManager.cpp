/*
	myFloorManager.cpp

	床の基底クラスと管理クラス
*/
#include "stdafx.h"
#include "myFloorManager.h"
#include "myApp.h"


// 敵の移動処理：基底クラスでは時刻を進めるだけ.
bool FloorBase::Update(float time)
{
	if (m_timer >= 0.0f) {
		m_timer += time;		// 時刻を進める.
	}
	return true;
}

// 敵の初期化.
void FloorBase::Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h)
{
	// x (初期位置X), y (初期位置Y)
	m_pos = pos;
	m_vel = v_pos;
	m_w = w, m_h = h;				  // 床のサイズ
	m_timer = 0.0f;	// タイマーを0.0fにして、有効化する. 
}

void FloorBase::Show()
{
	assert(m_timer >= 0.0f);// タイマーが負の状態で呼ばれるべきではないのでアサートを仕掛けておく.
	if (m_timer < 0.0f) {
		return;
	}

	D3DXVECTOR3 pos(m_pos.x, m_pos.y, 0);
	MyApp* myapp = GetApp();
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	myapp->Draw(pSpr, m_pTex->GetTexture(), pos, { 0, 0,(long)m_w, (long)m_h }, m_w, m_h, false, false, false, 0.5f);
}

// 現在のXY座標値を得る.
D3DXVECTOR2 FloorBase::GetXY()
{
	// 練習２
	// TODO メンバ変数の値を取得する処理を記述してください.
	return m_pos;
}

// 現在の床のサイズを得る.
float FloorBase::GetSize()
{
	return m_w;
}

// 位置(cx, cy)と自分の位置(posX, posY)は距離limit2より近いかどうか.
// limit2は距離の二乗.
bool FloorBase::Collision(float cx, float cy, float limit2)
{
	return (GetDistance2(cx, cy, m_pos.x, m_pos.y) < limit2);
}

// 次の空き位置を探す.
int FloorManager::Next()
{
	// リングバッファを、現在位置(m_index)から一周探索.
	for (int i = 0; i < m_floorMax; i++) {
		// 現在位置(m_index)から空きを検索.
		int index = (m_index + i) % m_floorMax;
		// index がm_enemyMax以上にならないよう、剰余にすることで配列の.
		// 現在位置から m_index - 1 の位置までを検索.
		if (m_ppBuffer[index] == NULL) {
			m_index = (index + 1) % m_floorMax;
			return index;
		}
	}
	return -1;
}

// デストラクタ:配列を解放している.
// 配列の中身をまず解放してから全体を解放.
FloorManager::~FloorManager()
{
	for (int i = 0; i < m_floorMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			delete m_ppBuffer[i];
		}
	}
	delete[] m_ppBuffer;
	delete[] m_pTextBuff;
}

// メモリの確保等の初期化.
bool FloorManager::Init()
{
	if (m_ppBuffer == NULL) {
		m_ppBuffer = new FloorBase * [m_floorMax];	// 配列の中身はEnemyBase*型.
		// ↑sizeof(EnemyBase*) を max 個分のメモリを確保しているだけで、コンストラクタは呼び出されない.
		// sizeof(EnemyBase*)はx64では１つあたり8バイト、x86では１つあたり4バイトのメモリサイズ.
		if (m_ppBuffer == NULL) {
			return false;
		}

		for (int i = 0; i < m_floorMax; i++) {
			m_ppBuffer[i] = NULL;		// ポインタをNULLで初期化し、未使用状態とする.
		}
	}

	if (m_pTextBuff == NULL) {
		m_pTextBuff = new char[m_floorMax + 1];	// デバッグ用文字列も確保する.
		// +1 は終端記号'\0'の分.
		if (m_pTextBuff == NULL) {
			return false;
		}
	}

	return true;
}

// 配列の全ての要素をリセットする.
void FloorManager::ResetAll()
{
	for (int i = 0; i < m_floorMax; i++) {
		delete m_ppBuffer[i];
		m_ppBuffer[i] = NULL;// 配列の中身をNULLクリアして次の使用に備える.
	}
}
// pEnemyを配列に追加.
// EnemyBase*型を引数にしているが、実際にはEnemyAAA*型やEnemyBBB*型が送られてくる.
// これらのクラスは、EnemyBaseの派生クラスなので、EnemyBase*型で受けることができる.
// 間違って引数を(EnemyBase pEnemy) と＊を付け忘れ、ポインタではなく実体で記述すると.
// データが一部欠落する現象が発生し深刻なバグの原因となる.
bool FloorManager::Add(FloorBase* pFloor)
{
	int index = Next();		// 配列から空きを探査.
	if (0 <= index && index < m_floorMax) {
		m_ppBuffer[index] = pFloor;// 敵を管理用の配列に登録.
		return true;
	}
	return false;
}

// 配列の各要素すべてに、処理を進めるよう指示する.
void FloorManager::Update(float time)
{
	for (int i = 0; i < m_floorMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			bool flag = m_ppBuffer[i]->Update(time);
		}
	}
}

// 配列の全ての敵に対して、表示を促す.
void FloorManager::Show()
{
	for (int i = 0; i < m_floorMax; i++) {
		// m_ppBuffer[i]の型はEnemyBase*だが、Show は virtual関数なので.
		// 派生クラスのshowが適切に呼び出される.
		// つまりifやswitchを使って分岐させる必要がない。これがオブジェクト指向の利点の１つ.
		if (m_ppBuffer[i] != NULL) {
			m_ppBuffer[i]->Show();
		}
	}
}

// 現在のXY座標値を得る.
D3DXVECTOR2 FloorManager::GetXY(int* index) {
	return m_ppBuffer[*index]->GetXY();
}

// 現在の床のサイズを得る.
float FloorManager::GetSize(int* index)
{
	return m_ppBuffer[*index]->GetSize();
}

// 配列の大きさを返す
int FloorManager::FloorMax()
{
	return m_floorMax;
}

// その配列が使用可能か
bool FloorManager::PPBuffer(int index) {
	if (m_ppBuffer[index] != NULL) {
		return true;
	}
	return false;
}

// デバッグ用にm_pBuffer配列の中身をテキスト化する.
// 弾丸が有効：*.
// 弾丸が無効：-.
char* FloorManager::GetText()
{
	for (int i = 0; i < m_floorMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			m_pTextBuff[i] = m_ppBuffer[i]->Getch();
		}
		else {
			m_pTextBuff[i] = '-';
		}
	}
	// 文字列の最後は必ずNULL文字でターミネート.
	m_pTextBuff[m_floorMax] = NULL;
	return m_pTextBuff;
}

// (cx, cy)の位置は敵と衝突距離だったか.
// 衝突した場合は引数pIndexが差す場所に配列のインデックスを入れてTRUEを返す.
// 衝突が無かった場合はFALSE返す.
bool FloorManager::Collision(float cx, float cy, float limit2, int* pIndex)
{
	for (int i = 0; i < m_floorMax; i++) {
		if (m_ppBuffer[i] != NULL) {
			if (m_ppBuffer[i]->Collision(cx, cy, limit2)) {
				*pIndex = i;
				return true;
			}
		}
	}
	return false;
}