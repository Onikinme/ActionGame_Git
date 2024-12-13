#pragma once

#include "myTexture.h"

//Bossの基底クラス
class BossTmp
{
protected:
	MyTexture* m_pTex;
	float m_posX, m_posY;	// 現在位置.
	float m_velX, m_velY;	// XY方向への速度成分.
	float m_timer;			// 生成してからの時刻.
	int   m_animNum;        //ボスのアニメーションの添え字

	// 前川祐毅　追加
	int   m_maxhp;          //  最大耐久値
	bool  m_boss_damageflg;	// ボスがダメージを受けたか（同じ攻撃で複数回ダメージを受けないようにする）

	int   m_hp;             //  耐久値
	float m_actionTimer;    //攻撃中の時間	
	bool  m_actionFlg;      //行動をしているかどうかのフラグ
	int   m_actionNum;      //行動パターンの添え字
	bool  m_leftFlg;         //True:プレイヤーはボスの左側 False:プレイヤーはボスの右側

	// このクラスは抽象クラスなので、派生クラス以外からはnewできないようにする.
	// そのため、コンストラクタをprotectedエリアに記述する.
	// コンストラクタ
	BossTmp(MyTexture* pTex)
		: m_pTex(pTex)
		, m_posX(0.0f)
		, m_posY(0.0f)
		, m_velX(0.0f)
		, m_velY(0.0f)
		, m_timer(0.0f)
		, m_animNum(0)
		, m_hp(0)
		, m_actionTimer(-1.0f)
		, m_actionFlg(false)
		, m_actionNum(0)
		, m_leftFlg(true)

		// 前川祐毅　追加
		, m_maxhp(0)
		, m_boss_damageflg(false)
	{
	}

public:

	// デストラクタは、子以外からもアクセスされる可能性があるため「public」.
	// 派生元として設計しているのでデストラクタをvirtual指定しておく.
	// デストラクタ.
	virtual ~BossTmp()
	{
	}

	//ボスの動作
	virtual bool Update(float time);
	//ボスの位置初期化
	virtual void Init(float posX, float posY, float vx, float vy, int hp);
	//ボス描画
	virtual void Draw();

	/********************
	*
	* 前川祐毅　削除
	*
	* MyAppのDraw関数で画像反転処理を行うので、SpriteFlipとSpriteResetは一旦削除
	*
	********************/
	/*
	//画像反転
	virtual void SpriteFlip();
	//画像の行列のリセット
	virtual void SpriteReset();
	*/

	//当たり判定
	virtual bool Collision(float cx, float cy, float limit2);
	//アクション決定
	virtual void ChangeAction(int a);
	//攻撃
	virtual void Attack();
	//ジャンプ(空中でPlayerをooフレーム追従後着地)
	virtual void Jump();
	//プレイヤーの方向に歩く
	virtual void Move();

	/********************
	* 
	* 前川祐毅　追加
	* 
	* ボスに攻撃を当てたときの処理
	* 
	********************/
	// 現在のXY座標値を得る.
	virtual D3DXVECTOR2 GetXY();

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
};

//////////////////////////////////
// BossManager
// ボス全体を管理する.
class BossManager
{
private:
	BossTmp** m_ppBuffer;	// ボスの配列、リングバッファとして使う.
	int m_bossMax;			// 配列の最大値.
	int m_index;			// 配列の使用現在位置.
	char* m_pTextBuff;		// デバッグ用に配列の状況を文字列にする.

	int Next();				// 次の位置を確保する（内部からしかコールしないのでprivate関数とする）.

public:
	BossManager(int max)
		: m_ppBuffer(NULL)
		, m_bossMax(max)
		, m_index(0)
		, m_pTextBuff(NULL)
	{
	}

	~BossManager();

	bool Init();			// メモリの確保等の初期化.
	void ResetAll();		// 配列の全ての要素をリセットする.
	bool Add(BossTmp*);		// 配列へのボスキャラの追加.
	void Update(float t);	// ボス配列全体を更新.
	void Show();			// ボスキャラクタを表示する.

	// 現在のXY座標値を得る.
	D3DXVECTOR2 GetXY(int* index);

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
	int BossMax();

	// その配列が使用可能か
	bool PPBuffer(int index);

	bool Collision(float cx, float cy, float limit2, int* pIndex);
	// ↑位置(cx, cy)からみて距離limit2以内の敵をpIndexに返却.

	//アクション決定
	void ChangeAction(int a, int index);
};