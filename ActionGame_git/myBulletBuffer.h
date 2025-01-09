/*
	myBulletBuffer.h

	弾丸管理クラス
*/
#pragma once

#include "myBulletOne.h"
#include "myTexture.h"


//////////////////////////
// BulletBuffer
// 弾丸バッファを管理するクラス。BulletOneクラスは単一の弾丸を管理している.
// 複数の弾丸の管理はこちらのクラスで行う。クラスを二つに分けることで.
// お互いの担当範囲（責任範囲）を分けることができ、プログラムの見通しを良くする.
class BulletBuffer
{
private:
	BulletOne* m_pBuffer;	// 弾丸の配列.
	int m_index;			// 配列上の現在位置を示す.
	int m_bulletMax;		// 配列の大きさ.
	char* m_pTextBuff;		// デバッグ用のテキストバッファ.

public:
	BulletBuffer(int max)
		: m_pBuffer(NULL)
		, m_index(0)
		, m_bulletMax(max)
		, m_pTextBuff(NULL)
	{}
	virtual ~BulletBuffer();

	bool Init();			// メモリの確保等の初期化.
	void ResetAll();

	BulletOne* Next();		// 次の空き位置を返す.
	BulletOne* GetBullet(int index)
	{
		return (m_pBuffer + index);// &m_pBuffer[index] と同じ.
	}

	void Update(float time);	// 配列の内容である弾丸全体を更新する.
 	void Draw(MyTexture*);		// 弾丸テクスチャを指定して描画.
	char* GetText();
	void Collision();			// プレーヤーの弾丸と敵キャラとの衝突判定.
	bool CollisionPlayer(float x, float y, float limit2);
								// 敵の弾丸とプレイヤー機との当たり判定.
#if defined (Shiken3)
	void Explosion(MyTexture* pTex, int index); // テクスチャ指定付きで、爆発を描画する.
#endif
};

