#include "stdafx.h"
#include "myExplosionOne.h"
#include "myApp.h"

// クラスのデストラクタ.
ExplosionOne::~ExplosionOne()
{}

void ExplosionOne::Start(float x, float y)
{
	m_posX = x;
	m_posY = y;
	m_ExplTimer = 0;
}

void ExplosionOne::Update(int time)
{
	assert(0 <= m_ExplTimer);

	// ８パターンのアニメーションを５フレーム毎に更新するので４０フレームで爆発アニメを終了させる.
	m_ExplTimer += time;
	if (m_ExplTimer > 20)
	{
		m_ExplTimer = -1;// 負の値で、活動停止状態とする.
	}
}

// 爆発アニメの描画.
void ExplosionOne::Draw(MyTexture* pTex)
{

	// 爆発テクスチャの表示領域のための配列
	const static RECT rcExplImageUV[] =
	{
	{ 0, 0, 64, 64 },
	{ 0, 64, 64, 128 },
	};

	//assert(0 < m_ExplTimer && m_ExplTimer <= 40);
	int nTime = m_ExplTimer - 1;// Drawよりも先にUpdateが呼び出されるため、前フレームの値で計算.

	D3DXVECTOR3 cnt(32.f, 32.f, 0.f);		// 画像のセンター位置を指定.
	//D3DXVECTOR3 cnt(48.f, 48.f, 0.f);		// 画像のセンター位置を指定.
	D3DXVECTOR3 pos(m_posX, m_posY, 0.f);	// 表示位置を指定.

	int animTime = nTime / 10;	// ５フレーム毎に更新.
	int anim = animTime % 2;	// ８パターンのアニメーション.　配列のオーバーアクセス回避のため剰余算にしておく.
	//int anim = animTime % 9;	// ９パターンのアニメーション.　配列のオーバーアクセス回避のため剰余算にしておく.
	//pSpr->Draw(pTex->GetTexture(), &(rcExplImageUV[anim]), &cnt, &pos, 0xFFFFFFFF);

	MyApp* myapp = GetApp();
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	myapp->Draw(pSpr, pTex->GetTexture(), pos, rcExplImageUV[anim], 64.0f, 64.0f, false, false);
}
