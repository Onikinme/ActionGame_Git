#include <windows.h>
#include <d3d9.h>			// Direct3D9 (DirectX9 Graphics)�ɕK�v.
#include <d3dx9core.h>		// DirectX �X�v���C�g�ɕK�v.
#include <tchar.h>
#include <stdio.h>

const int MAX_TEXTURE = 256;

////////////////////////// MyTexture //////////////////////////
// �e�N�X�`�����Ǘ����邽�߂̃N���X���쐬����.
// �e�N�X�`�����ݒ肳��Ă���΁A�f�X�g���N�^�ɂĉ������.
// ���̌��ʁA����Y���h�����Ƃ��ł���.
// �������A�I�u�W�F�N�g���̂�delete���Ȃ���΂Ȃ�Ȃ��̂�.
// ���ς�炸����Y��ɂ͒��ӂ��Ȃ���΂Ȃ�Ȃ�.
class MyTexture
{
private:
	static MyTexture* sm_pStack[MAX_TEXTURE];
	static int sm_maxTexture;

protected:
	IDirect3DTexture9* m_pTex;			// �Ǘ�����e�N�X�`��.

private:
	// �R���X�g���N�^���N���X��`���ŋL�q�B�����o�ϐ�pTex�� NULL �ŏ���������.
	MyTexture() : m_pTex(NULL)
	{
	}

	// �f�X�g���N�^�ł́ApTex���������B�����I�ɍs���邽�߁A����Y���h����.
	// �f�X�g���N�^��private�G���A�ɔz�u���O������̊J�����֎~.
	virtual ~MyTexture()
	{
		if (m_pTex)
		{
			m_pTex->Release();
		}
	}

public:
	// �A�N�Z�T�BpTex���擾���邽�߂̊֐�.
	IDirect3DTexture9* GetTexture()
	{
		return m_pTex;
	}

	// MyTexture�I�u�W�F�N�g���쐬���e�N�X�`����ǂݍ���.
	static MyTexture* LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname);

	// �ǂݍ��񂾃e�N�X�`�������ׂĔj������.
	static void ReleaseAll();
};

HRESULT LoadTexture(IDirect3DDevice9* pDev, const TCHAR* pFname, IDirect3DTexture9** ppTex);
