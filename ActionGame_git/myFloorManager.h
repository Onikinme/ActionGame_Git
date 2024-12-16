/*
	myFloorManager.h

	床の基底クラスと管理クラス
*/
#pragma once

#include "myTexture.h"

//Floorの基底クラス
class FloorBase
{
protected:
	// 以下の変数は「派生クラス」から頻繁にアクセスされるため.
// privateではなくprotectedに指定しておく.
	MyTexture* m_pTex;		// 表示するテクスチャ.
	D3DXVECTOR2 m_pos;		// 床の座標
	D3DXVECTOR2 m_vel;		// 床の移動座標
	float m_w, m_h;				  // 床のサイズ
	float m_timer;			// 生成してからの時刻.
	bool floorflg; // プレイヤーがこの床の上にいる

	// このクラスは抽象クラスなので、派生クラス以外からはnewできないようにする.
	// そのため、コンストラクタをprotectedエリアに記述する.
	// コンストラクタ
	FloorBase(MyTexture* pTex)
		: m_pTex(pTex)
		, m_pos(0.0f, 0.0f)
		, m_vel(0.0f, 0.0f)
		, m_w(0.0f), m_h(0.0f)
		, m_timer(-1.0f)
		, floorflg(false)
	{
	}

public:

	// デストラクタは、子以外からもアクセスされる可能性があるため「public」.
// 派生元として設計しているのでデストラクタをvirtual指定しておく.
	// デストラクタ.
	virtual ~FloorBase() {}

	// 床キャラの動作.
	virtual bool Update(float time);

	// デバッグ用に文字を返す.
	virtual char Getch() { return '*'; }

	// 床の初期化.
	virtual void Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h);

	// キャラクタを表示する.
	virtual void Show();

	// 現在のXY座標値を得る.
	virtual D3DXVECTOR2 GetXY();

	// 現在の床のサイズを得る.
	virtual float GetSize();

	// 位置(cx,cy)との距離がlimit2以下かどうかを判定.
	virtual bool Collision(float cx, float cy, float limit2);
};

//////////////////////////////////
// FloorManager
// 床全体を管理する.
class FloorManager
{
private:
	FloorBase** m_ppBuffer;	// 床の配列、リングバッファとして使う.
	int m_floorMax;			// 配列の最大値.
	int m_index;			// 配列の使用現在位置.
	char* m_pTextBuff;		// デバッグ用に配列の状況を文字列にする.

	int Next();				// 次の位置を確保する（内部からしかコールしないのでprivate関数とする）.

public:
	FloorManager(int max)
		: m_ppBuffer(NULL)
		, m_floorMax(max)
		, m_index(0)
		, m_pTextBuff(NULL)
	{
	}

	~FloorManager();

	bool Init();			// メモリの確保等の初期化.
	void ResetAll();		// 配列の全ての要素をリセットする.
	bool Add(FloorBase*);	// 配列への敵キャラの追加.
	void Update(float t);	// 敵配列全体を更新.
	void Show();			// 敵キャラクタを表示する.

	// 現在のXY座標値を得る.
	D3DXVECTOR2 GetXY(int* index);

	// 現在の床のサイズを得る.
	float GetSize(int* index);

	// 配列の大きさを返す
	int FloorMax();

	// その配列が使用可能か
	bool PPBuffer(int index);

	char* GetText();				// デバッグ用に配列の状況を文字列化する.

	bool Collision(float cx, float cy, float limit2, int* pIndex);
	// ↑位置(cx, cy)からみて距離limit2以内の敵をpIndexに返却.
};