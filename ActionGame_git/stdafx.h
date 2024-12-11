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
#include <d3d9.h>			// Direct3D9 (DirectX9 Graphics)に必要.
#include <d3dx9core.h>		// DirectX スプライトに必要.
#include <dsound.h>			// DirectSoundに必要.

// dinput.hのインクルードの前にどのバージョンを使うかを定義する必要がある.
#define DIRECTINPUT_VERSION 0x0800// これがないとビルド時に警告がでる.
#include <dinput.h>

#define MEMORY_LEAK
#ifdef MEMORY_LEAK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// ウィンドウの大きさ設定.
#define WIDTH (960) // 幅.
#define HEIGHT (540) // 高さ.


// 距離の二乗を求める。「距離」を求めないのは、平方根の処理を避けるため.
inline float GetDistance2(float x1, float y1, float x2, float y2)
{
	return ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2));
}
