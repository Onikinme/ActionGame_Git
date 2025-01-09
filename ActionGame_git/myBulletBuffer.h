/*
	myBulletBuffer.h

	�e�ۊǗ��N���X
*/
#pragma once

#include "myBulletOne.h"
#include "myTexture.h"


//////////////////////////
// BulletBuffer
// �e�ۃo�b�t�@���Ǘ�����N���X�BBulletOne�N���X�͒P��̒e�ۂ��Ǘ����Ă���.
// �����̒e�ۂ̊Ǘ��͂�����̃N���X�ōs���B�N���X���ɕ����邱�Ƃ�.
// ���݂��̒S���͈́i�ӔC�͈́j�𕪂��邱�Ƃ��ł��A�v���O�����̌��ʂ���ǂ�����.
class BulletBuffer
{
private:
	BulletOne* m_pBuffer;	// �e�ۂ̔z��.
	int m_index;			// �z���̌��݈ʒu������.
	int m_bulletMax;		// �z��̑傫��.
	char* m_pTextBuff;		// �f�o�b�O�p�̃e�L�X�g�o�b�t�@.

public:
	BulletBuffer(int max)
		: m_pBuffer(NULL)
		, m_index(0)
		, m_bulletMax(max)
		, m_pTextBuff(NULL)
	{}
	virtual ~BulletBuffer();

	bool Init();			// �������̊m�ۓ��̏�����.
	void ResetAll();

	BulletOne* Next();		// ���̋󂫈ʒu��Ԃ�.
	BulletOne* GetBullet(int index)
	{
		return (m_pBuffer + index);// &m_pBuffer[index] �Ɠ���.
	}

	void Update(float time);	// �z��̓��e�ł���e�ۑS�̂��X�V����.
 	void Draw(MyTexture*);		// �e�ۃe�N�X�`�����w�肵�ĕ`��.
	char* GetText();
	void Collision();			// �v���[���[�̒e�ۂƓG�L�����Ƃ̏Փ˔���.
	bool CollisionPlayer(float x, float y, float limit2);
								// �G�̒e�ۂƃv���C���[�@�Ƃ̓����蔻��.
#if defined (Shiken3)
	void Explosion(MyTexture* pTex, int index); // �e�N�X�`���w��t���ŁA������`�悷��.
#endif
};

