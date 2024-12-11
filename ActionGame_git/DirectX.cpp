/*
	DirectX.cpp

	ソースを整理して全体像を把握し易くた.
*/
#include "stdafx.h"
#include "myApp.h"

// イベント処理コールバック（ウィンドウプロシージャ）.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// イベントの種類に応じて、switch文にて処理を切り分ける.
	switch (uMsg) {
	case WM_KEYDOWN: {
		// ESCキーが押下されたら終了.
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	}

	case WM_CLOSE: {// 終了通知(CLOSEボタンが押された場合など)が届いた場合.
		// プログラムを終了させるため、イベントループに0を通知する.
		// この結果、GetMessageの戻り値は0になる.
		PostQuitMessage(0);
		break;
	}

	default: {
		break;
	}
	}

	// デフォルトのウィンドウイベント処理.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// エラー発生時のHRESULTを文字列に変換するための補助関数.
const TCHAR* D3DErrStr(HRESULT res)
{
	switch (res) {
	case D3D_OK: return _T("D3D_OK");
	case D3DERR_DEVICELOST: return _T("D3DERR_DEVICELOST");
	case D3DERR_DRIVERINTERNALERROR: return _T("D3DERR_DRIVERINTERNALERROR");
	case D3DERR_INVALIDCALL: return _T("D3DERR_INVALIDCALL");
	case D3DERR_OUTOFVIDEOMEMORY: return _T("D3DERR_OUTOFVIDEOMEMORY");
	case D3DERR_DEVICENOTRESET: return _T("D3DERR_DEVICENOTRESET");
	case D3DERR_NOTAVAILABLE: return _T("D3DERR_NOTAVAILABLE");
	case D3DXERR_INVALIDDATA: return _T("D3DXERR_INVALIDDATA");
	case MMSYSERR_NODRIVER: return _T("MMSYSERR_NODRIVER");
	case MMSYSERR_INVALPARAM: return _T("MMSYSERR_INVALPARAM");
	case MMSYSERR_BADDEVICEID: return _T("MMSYSERR_BADDEVICEID");
	case JOYERR_UNPLUGGED: return _T("JOYERR_UNPLUGGED");
	case JOYERR_PARMS: return _T("JOYERR_PARMS");
	case E_OUTOFMEMORY: return _T("E_OUTOFMEMORY");
	}
	return _T("unknown error");
}

// argc コマンドラインから渡された引数の数.
// argv 引数の実体へのポインタ配列.
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Japanese");		// 日本語にロケールを設定.

#ifdef MEMORY_LEAK
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
#endif

	MyApp* pApp = GetApp();
	if (pApp->InitApp()) {
		pApp->MainLoop();
	}
	pApp->ReleaseData();

#ifdef MEMORY_LEAK
	_CrtDumpMemoryLeaks();		// メモリリークを検出する.
#endif

	return 0;// 正常終了.
}
