/*
	stdafx.h
*/
#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include <locale.h>
#include <assert.h>
#include <d3d9.h>			// Direct3D9 (DirectX9 Graphics)�ɕK�v.
#include <d3dx9core.h>		// DirectX �X�v���C�g�ɕK�v.
#include <dsound.h>			// DirectSound�ɕK�v.

// dinput.h�̃C���N���[�h�̑O�ɂǂ̃o�[�W�������g�������`����K�v������.
#define DIRECTINPUT_VERSION 0x0800// ���ꂪ�Ȃ��ƃr���h���Ɍx�����ł�.
#include <dinput.h>

#define MEMORY_LEAK
#ifdef MEMORY_LEAK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// �E�B���h�E�̑傫���ݒ�.
#define WIDTH (960) // ��.
#define HEIGHT (540) // ����.


// �����̓������߂�B�u�����v�����߂Ȃ��̂́A�������̏���������邽��.
inline float GetDistance2(float x1, float y1, float x2, float y2)
{
	return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
}
