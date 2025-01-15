/*
	myBulletBuffer.cpp

	�e�ۊǗ��N���X
*/
#include "stdafx.h"
#include "myBulletBuffer.h"
#include "myEnemyManager.h"
#include "MyExplosionBuffer.h"
#include "mySound.h"
#include "myApp.h"

// �f�X�g���N�^:�z���������Ă���.
BulletBuffer::~BulletBuffer()
{
	if (m_pBuffer) {
		delete[] m_pBuffer;			// delete m_pBuffer�łȂ����Ƃɒ��ӁB�z��ł�[]��t����.
	}
	if (m_pTextBuff) {
		delete[] m_pTextBuff;
	}
}

// �������̊m�ۓ��̏�����.
bool BulletBuffer::Init()
{
	if (m_pBuffer == NULL) {
		m_pBuffer = new BulletOne[m_bulletMax];
		if (m_pBuffer == NULL) {
			return false;
		}
	}
	if (m_pTextBuff == NULL) {
		m_pTextBuff = new char[m_bulletMax + 1];
		if (m_pTextBuff == NULL) {
			return false;
		}
	}
	return true;
}

void BulletBuffer::ResetAll()
{
	for (int i = 0; i < m_bulletMax; i++) {
		m_pBuffer[i].Deactive();
	}
}

// �����O�o�b�t�@��T�����āA�󂫂�������.
// return NULL		�󂫂Ȃ�.
// return NULL�ȊO	�󂫈ʒu.
BulletOne* BulletBuffer::Next()
{
	// �����O�o�b�t�@�����݈ʒu(m_index)����T��.
	for (int i = 0; i < m_bulletMax; i++) {
		// ���݈ʒu(m_index)����󂫂�����.
		int index = (m_index + i) % m_bulletMax;
		// index ��m_bulletMax�ȏ�ɂȂ�Ȃ��悤��]�ɂ�.
		// �z��̍Ō�܂Ń`�F�b�N������A���x�͐擪����(index - 1)�̈ʒu�܂ł��`�F�b�N����.
		if (!m_pBuffer[index].IsActive()) {
			// �󂫂�����������...
			// �Ԃ��O�Ɍ����J�n�ʒu���P�i�߂�i�Ō�܂ōs������擪�ɖ߂�j.
			m_index = (m_index + 1) % m_bulletMax;

			// �󂢂Ă�ꏊ�̃|�C���^��Ԃ�.
			return &m_pBuffer[index];
		}
	}
	// �󂫂Ȃ�.
	return NULL;
}

// ���݂�XY���W�l�𓾂�.
void BulletBuffer::GetXY(int* index, float& x, float& y) {
	m_pBuffer[*index].GetXY(x, y);
}

// �e�ۂ̃T�C�Y�𓾂�
void BulletBuffer::GetSize(int* index, float& size)
{
	m_pBuffer[*index].GetSize(size);
}

// �����O�o�b�t�@���̃f�[�^�������X�V����.
void BulletBuffer::Update(float time)
{
	for (int i = 0; i < m_bulletMax; i++) {
		m_pBuffer[i].Update(time);
	}
}

// �f�o�b�O�p��m_pBuffer�z��̒��g���e�L�X�g������.
// �e�ۂ��L���Ȃ�*�A�����Ȃ�-�ƈꕶ���ɂ���.
char* BulletBuffer::GetText()
{
	for (int i = 0; i < m_bulletMax; i++) {
		if (m_pBuffer[i].IsActive()) {
			m_pTextBuff[i] = '*';
		}
		else {
			m_pTextBuff[i] = '-';
		}
	}
	// ������̍Ō�͕K�� NULL���� �Ń^�[�~�l�[�g.
	m_pTextBuff[m_bulletMax] = NULL;
	return m_pTextBuff;
}

// �v���[���[�̒e�ۂƓG�L�����Ƃ̏Փ˔���.
void BulletBuffer::Collision()
{
	/*
	EnemyManager* pEnemyMgr = GetApp()->GetEnemyMgr();
	ExplosionBuffer* pExplBuffer = GetApp()->GetExplMgr();

	// �e�e�ۂ̈ʒu(posX, posY)�ƁA�G�̋��������ȉ����ǂ�������.
	for (int i = 0; i < m_bulletMax; i++) {
		BulletOne& bullet = m_pBuffer[i];		// bullet�Œu������.
		if (!bullet.IsActive()) {
			continue;
		}

		// �e�e�ۂ̌��݈ʒu���擾�A�G�̂��Âꂩ�ƏՓ˂��������ׂ�.
		float posX, posY;
		bullet.GetXY(posX, posY);
		int index;		// �Փ˂����G�B�z���̈ʒu.
		if (pEnemyMgr->Collision(posX, posY, 24 * 24, &index)) {
			if (!pEnemyMgr->PowerDown(&index)) {
				// �Փ˂����̂ŁA�e�ۂ𖳌���.
				bullet.Deactive();
				// ��e����炷.
				GetSoundInst()->PlaySE(SE_HIDAN);// ��e���Đ�.
			}
			else {
				// �Փ˂����̂ŁA�e�ۂ𖳌���.
				bullet.Deactive();

				// ���_�����Z.
				GetApp()->AddScore(pEnemyMgr->GetScore(index));

				// �G��������.
				pEnemyMgr->Purge(index);

				// �����A�j���[�V�����𔭐�.
				pExplBuffer->Explose(posX, posY);

				// ���ĉ��Đ�.
				GetSoundInst()->PlaySE(SE_TEKI_DEAD);
			}
				// �Փ˂����̂ŁA�e�ۂ𖳌���.
				bullet.Deactive();

				// ���_�����Z.
				//GetApp()->AddScore(pEnemyMgr->GetScore(index));

				// �G��������.
				//pEnemyMgr->Purge(index);

				// ���ĉ��Đ�.
				GetSoundInst()->PlaySE(SE_TEKI_DEAD);
			}
		}
	}
	*/
}

// �e�N�X�`���w��t���ŁA�e�ۂ�`�悷��.
void BulletBuffer::Draw(MyTexture* pTex)
{
	for (int i = 0; i < m_bulletMax; ++i)
	{
		// �L���Ȃ�\��.
		if (m_pBuffer[i].IsActive()) {
			m_pBuffer[i].Draw(pTex);
		}
	}
}

// �v���C���[�ɓG�̒e�ۂ��Փ˂��Ă��邩�̔���.
// �߂�l�́Atrue=�Փ�  false=�������ĂȂ�.
bool BulletBuffer::CollisionPlayer(float x, float y, float limit2)
{
	// �e�e�ۂ̈ʒu(xx, yy)�ƁA�v���C���[�̋��������ȉ����ǂ�������.
	for (int i = 0; i < m_bulletMax; i++) {
		BulletOne& bullet = m_pBuffer[i];		// bullet�Œu������.
		if (!bullet.IsActive()) {
			continue;
		}

		// �A�N�e�B�u�Ȃ�ȍ~�̏������s��.
		// �e�e�ۂ̌��݈ʒu���擾�A�v���C���[�ƏՓ˂��������ׂ�.
		float bx, by;
		bullet.GetXY(bx, by);
		if (GetDistance2(bx, by, x, y) < limit2) {
			// �Փ˂����̂ŁA�e�ۂ𖳌���.
			bullet.Deactive();
			return true;
		}
	}
	return false;
}

// �z��̑傫����Ԃ�
int BulletBuffer::BulletMax()
{
	return m_bulletMax;
}

// �z�񂪈�ł��g�p����Ă��邩
bool BulletBuffer::PPBuffer(int index) {
		if (m_pBuffer[index].IsActive()) {
			return true;
		}
	return false;
}

#if defined (Shiken3)
/*
// �e�N�X�`���w��t���ŁA������`�悷��.
void BulletBuffer::Explosion(MyTexture* pTex, int index)
{
	ID3DXSprite* pSpr = GetApp()->GetSprite();
	D3DXVECTOR3 cnt(8, 8, 0);
	D3DXVECTOR3 pos(0, 0, 0);

	if (0 <= index && index < m_enemyMax) {
		m_ppBuffer[index]->Isdead = true;
		m_ppBuffer[index] = NULL;// �z��̒��g��NULL�N���A���Ď��̎g�p�ɔ�����.
	}

	for (int i = 0; i < m_enemyMax; i++) {
		// �擾�����e�ۂ��L���Ȃ�\��.
		if (m_ppBuffer[i]->IsActive()) {
			// ���݈ʒu��pos�\���̌^�̃f�[�^�Ɋi�[���ĕ\��.
			m_ppBuffer[i].GetXY(pos.x, pos.y);
			pSpr->Draw(pTex->GetTexture(), NULL, &cnt, &pos, 0xFFFFFFFF);
		}
	}
}
*/
#endif
