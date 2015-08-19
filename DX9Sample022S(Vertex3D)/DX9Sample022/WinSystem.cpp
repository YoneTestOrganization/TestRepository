// -------------------- IncludeFile -----------------
#include <crtdbg.h>
#include "Application.h"
// ------------------------------------------------------------------------
//　関数名	:WinMain				アプリケーションエントリーポイント	
//	引数	:hInstance				インスタンスハンドル(Windows環境で実行されているプログラムを一意に識別する)	
//			:hPrevInstance			Windows95以降はNULL
//			:pCmdLine				プログラムに渡されたコマンドライン引数が格納された文字列のポインタ
//			:CmdShow				ウィンドウズの描画方法
//	戻り値	:						Windowsへの終了コード		
// ------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR pCmdLine,int CmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CApplication* pApp = CApplication::getInstance();

	// アプリケーションの初期化
	LPCSTR WindowTitle = "DX9Sample022";		// アプリケーション名
	SIZE WindowSize = {800,600};				// スクリーンサイズ
	BOOL bWindowed = TRUE;						// モード選定(TRUE:ウィンドウ、FALSE:フルスクリーン)
	HWND hWnd = pApp->Initialize(hInstance,WindowTitle,WindowSize,bWindowed);
	if(hWnd == NULL)
		return 0;

	// アプリケーション実行
	return pApp->Run(hWnd);
}