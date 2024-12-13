/*
	myEnemyManager.h

	敵の基底クラスと管理クラス
*/
#pragma once

#include "myTexture.h"

//////////////////////////////////
// EnemyBase
// 敵１体を管理する基本クラス.
class EnemyBase
{
protected:
	// 以下の変数は「派生クラス」から頻繁にアクセスされるため.
// privateではなくprotectedに指定しておく.
	MyTexture* m_pTex;		// 表示するテクスチャ.
	D3DXVECTOR2 m_pos;		// 敵の座標
	D3DXVECTOR2 m_vel;		// 敵の移動座標
	float m_w, m_h;				  // 敵のサイズ
	int MaxHP;				// 敵の最大体力
	int HP;					// 敵の現在体力
	float m_timer;			// 生成してからの時刻.
	bool enemy_damageflg;	// 敵がダメージを受けたか（同じ攻撃で複数回ダメージを受けないようにする）

	// このクラスは抽象クラスなので、派生クラス以外からはnewできないようにする.
	// そのため、コンストラクタをprotectedエリアに記述する.
	// コンストラクタ
	EnemyBase(MyTexture* pTex)
		: m_pTex(pTex)
		, m_pos(0.0f, 0.0f)
		, m_vel(0.0f, 0.0f)
		, MaxHP(0)
		, HP(0)
		, m_timer(-1.0f)
		, enemy_damageflg(false)
	{
	}

public:

	// デストラクタは、子以外からもアクセスされる可能性があるため「public」.
// 派生元として設計しているのでデストラクタをvirtual指定しておく.
	// デストラクタ.
	virtual ~EnemyBase() {}

	// 敵キャラの動作.
	virtual bool Update(float time);

	// デバッグ用に文字を返す.
	virtual char Getch() { return '*'; }

	// 敵の初期化.
	virtual void Init(D3DXVECTOR2 pos, D3DXVECTOR2 v_pos, float w, float h, int maxhp);

	// キャラクタを表示する.
	virtual void Show();

	// 現在のXY座標値を得る.
	//void GetXY(float& x, float& y);
	virtual D3DXVECTOR2 GetXY();

	// 現在のプレイヤーのサイズを得る.
	virtual float GetSize();

	// 最大HPを得る
	virtual int GetMaxHP();

	// 現在のHPを得る
	virtual int GetHP();

	// ダメージを受けた時の処理
	virtual void Damage(int damage);

	// ダメージフラグを管理
	virtual void DamageFlg(bool damage);

	// 攻撃でダメージを負ったか？
	virtual bool IsDamage();

	// タイマーが0以上なら有効（インライン関数）.
	//virtual bool IsActive();

	// 位置(cx,cy)との距離がlimit2以下かどうかを判定.
	virtual bool Collision(float cx, float cy, float limit2);
};

//////////////////////////////////
// EnemyManager
// 敵全体を管理する.
class EnemyManager
{
private:
	EnemyBase** m_ppBuffer;	// 敵の配列、リングバッファとして使う.
	int m_enemyMax;			// 配列の最大値.
	int m_index;			// 配列の使用現在位置.
	char* m_pTextBuff;		// デバッグ用に配列の状況を文字列にする.

	int Next();				// 次の位置を確保する（内部からしかコールしないのでprivate関数とする）.

public:
	EnemyManager(int max)
		: m_ppBuffer(NULL)
		, m_enemyMax(max)
		, m_index(0)
		, m_pTextBuff(NULL)
	{}

	~EnemyManager();
	//EnemyBase* pNext(); // 次の空き位置を返す.
	/*
	EnemyBase* pGet(int ix)
	{
		return m_pBuffer + ix; // &(m_pBuffer[ix]) と同じ意味.
	}
	*/

	bool Init();			// メモリの確保等の初期化.
	void ResetAll();		// 配列の全ての要素をリセットする.
	bool Add(EnemyBase*);	// 配列への敵キャラの追加.
	void Update(float t);	// 敵配列全体を更新.
	// void Purge(int index);	// 配列から位置indexの敵を取り除く.
	void Show();			// 敵キャラクタを表示する.

	// 現在のXY座標値を得る.
	D3DXVECTOR2 GetXY(int* index);

	// 現在のプレイヤーのサイズを得る.
	float GetSize();

	// 最大HPを得る
	int GetMaxHP(int* index);

	// 現在のHPを得る
	int GetHP(int* index);

	// ダメージを受けた時の処理
	void Damage(int damage, int index);

	// ダメージフラグを管理
	void DamageFlg(bool damage, int index);

	// 攻撃でダメージを負ったか？
	bool IsDamage(int index);

	// 配列の大きさを返す
	int EnemyMax();

	// その配列が使用可能か
	bool PPBuffer(int index);

	char* GetText();				// デバッグ用に配列の状況を文字列化する.

	bool Collision(float cx, float cy, float limit2, int* pIndex);
	// ↑位置(cx, cy)からみて距離limit2以内の敵をpIndexに返却.
};