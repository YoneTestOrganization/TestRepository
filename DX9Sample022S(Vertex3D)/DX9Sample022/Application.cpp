// -------------------- IncludeFile -----------------
#include "Application.h"
CApplication CApplication::m_pAppInstance;	// インスタンスの実態
// コンストラクタ
CApplication::CApplication()
{
	m_pAppState = new CAppState();
	m_ptWindowShowPosition.x = -1;
}
// デストラクタ
CApplication::~CApplication()
{
	delete m_pAppState;
}
// ------------------------------------------------------------------------
//　関数名	:Initialize					アプリケーションの初期化
//	引数	:hInstance					インスタンスハンドル
//			:WindowSize					ウィンドウサイズ
//			:bWindowed					TRUE:ウィンドウモード、FALSE:フルスクリーンモード
//	戻り値	:							ウィンドウハンドル
// ------------------------------------------------------------------------
HWND CApplication::Initialize(HINSTANCE hInstance,
							  LPCTSTR WindowTitle,
							  SIZE WindowSize,
							  BOOL bWindowed)
{
	// ウィンドウの初期化
	HWND hWnd = InitWindow(hInstance,WindowTitle,WindowSize,bWindowed);
	if(hWnd == NULL)
		return NULL;

	// ゲームデバイスを初期化する
	m_pDXDevice = new CDXDevice();
	HRESULT hResult = m_pDXDevice->Initialize(hWnd,bWindowed);
	if(FAILED(hResult)) {
		delete m_pDXDevice;
		return NULL;
	}

	// タイマーの分解度を1msに設定する
	timeBeginPeriod(1);

	// ウィンドウの表示
	::ShowWindow(hWnd,SW_SHOWNORMAL);

	// カーソル設定
	if(!bWindowed)
		PostMessage(hWnd,UM_SETCURSOR,0L,0L);

	return hWnd;
}
// ------------------------------------------------------------------------
//　関数名	:Run						アプリケーションの実行
//	引数	:hWnd						ウィンドウハンドル
//	戻り値	:							アプリケーション終了値
// ------------------------------------------------------------------------
int CApplication::Run(HWND hWnd)
{
	// ゲームスピード管理スレッド作成/実行
	CTimerThread* pTimerThread = new CTimerThread();
	pTimerThread->Start(m_pDXDevice,m_pAppState);

	// メッセージループ
	MSG msg;
	do {
		// メッセージ処理
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			DispatchMessage( &msg );
			TranslateMessage( &msg );
		}
		if(pTimerThread->getTimerThreadSleep()) {
			if(m_pDXDevice->getDeviceLost()) {
				if(m_pDXDevice->getDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
					if(pTimerThread->m_pDXDevice->RestoreDevice()) {
						pTimerThread->m_pDXDevice->setDeviceLost(FALSE);	// デバイスが復元した
						m_pDXDevice->setDeviceLost(TRUE);
					}
				}
			}
			else {
				BOOL bWindowed = m_pDXDevice->getWindowed();
				bWindowed ^= TRUE;
				m_pDXDevice->setWindowed(bWindowed);
				m_pDXDevice->ChangeDisplayMode();					// 表示モード切り替え
				// ウィンドウスタイルの指定
				DWORD WindowStyle;
				if(bWindowed) {
					// ウィンドウスタイルを指定
					WindowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
				}
				else {
					// ウィンドウスタイルをポップアップに指定
					WindowStyle = WS_POPUP | WS_VISIBLE;
				}
				::SetWindowLong(hWnd,GWL_STYLE,WindowStyle);		// ウィンドウスタイルの変更

				// ウィンドウの配置
				if(bWindowed) {										// ウィンドウモード時の処理
					if(m_ptWindowShowPosition.x == -1) {
						// クライアントサイズの変更
						setClientSize(hWnd,m_WindowSize);
						// ウィンドウ表示位置を画面の中心変更
						WindowMoveToCenter(hWnd);
					}
					::SetWindowPos(hWnd,HWND_TOP,m_ptWindowShowPosition.x,m_ptWindowShowPosition.y,m_WindowSize.cx,m_WindowSize.cy,SWP_SHOWWINDOW);
				}
			}
			pTimerThread->setTimerThreadSleep(FALSE);
		}
		Sleep(1);
	} while(msg.message != WM_QUIT);

	// ゲームスピード管理スレッド終了
	delete pTimerThread;

	// リソース開放
	m_pAppInstance.m_pDXDevice->CleanupResource();

	// ゲームスレッド終了
	delete m_pDXDevice;

	// タイマーの分解度を1msに設定する
	timeEndPeriod(1);

	// 戻り値(メッセージのwParamパラメータ)
	return (int)msg.wParam;
}
// ------------------------------------------------------------------------
//　関数名	:WndProc					ウィンドウのプロシージャ	
//	引数	:hWnd						ウィンドウのハンドル
//			:uMessage					メッセージの種類
//			:wParam						メッセージの補助情報
//			:lParam						メッセージの補助情報
//	戻り値	:							メッセージを戻す		
// ------------------------------------------------------------------------
LRESULT CALLBACK CApplication::WndProc(HWND hWnd,UINT uMessage,WPARAM wParam,LPARAM lParam)
{
	// メッセージ処理
	switch(uMessage) {
		case WM_ACTIVATE:
			if(HIWORD(wParam))
				m_pAppInstance.m_pAppState->setIsIcon(TRUE);
			else
				m_pAppInstance.m_pAppState->setIsIcon(FALSE);
			break;
		case WM_MOVE:
			{
				if(!m_pAppInstance.m_pDXDevice)
					break;
				if(m_pAppInstance.m_ptWindowShowPosition.x == -1)
					break;
				if(m_pAppInstance.m_pDXDevice->getWindowed()) {
					RECT rc;
					GetWindowRect(hWnd,&rc);
					m_pAppInstance.m_ptWindowShowPosition.x = rc.left;
					m_pAppInstance.m_ptWindowShowPosition.y = rc.top;
				}
			}
			break;
		// カーソルの設定処理
		case UM_SETCURSOR:
			if(m_pAppInstance.m_pDXDevice->getWindowed())
				ShowCursor(TRUE);				// カーソル表示
			else
				ShowCursor(FALSE);				// カーソル非表示
			break;
		// ウィンドウの破棄処理	
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);								// ウィンドウ終了通知
			break;
	}
	return DefWindowProc(hWnd,uMessage,wParam,lParam);
}
// ------------------------------------------------------------------------
//　関数名	:InitWindow					ウィンドウの初期化	
//	引数	:hInstance					インスタンスハンドル
//			:WindowTitle				タイトル名
//			:WindowSize					ウィンドウサイズ
//			:bWindowed					TRUE:ウィンドウモード、FALSE:フルスクリーンモード
//	戻り値	:							ウィンドウハンドル		
// ------------------------------------------------------------------------
HWND CApplication::InitWindow(	HINSTANCE hInstance,
								LPCSTR WindowTitle,
								SIZE WindowSize,
								BOOL bWindowed)
{
	// ウィンドウクラスの登録
	WNDCLASSEX wcex;	// ウィンドウクラスの情報格納用構造体変数
	ZeroMemory(&wcex,sizeof(wcex));
	wcex.cbSize			= sizeof(WNDCLASSEX);					// 構造体のサイズを設定
	wcex.style			= CS_HREDRAW | CS_VREDRAW;				// ウィンドウスタイル(幅/高さ変更許可)
	wcex.lpfnWndProc	= (WNDPROC)WndProc;						// ウィンドウプロシージャの指定
	wcex.cbClsExtra		= 0;									// 追加領域は使わない
	wcex.cbWndExtra		= 0;									// 追加領域は使わない
	wcex.hInstance		= hInstance;							// このインスタンスのハンドル
	wcex.hIcon			= LoadIcon(hInstance,IDI_APPLICATION);	// ラージアイコン
	wcex.hCursor		= LoadCursor(NULL,IDC_ARROW);			// カーソルスタイル
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	// 背景色 黒色
	wcex.lpszMenuName	= NULL;									// 表示メニューのセレクト(メニューなし)
	wcex.lpszClassName	= TEXT("MainWindowClass");				// ウィンドウクラス名
	wcex.hIconSm		= LoadIcon(wcex.hInstance,(LPCTSTR)IDI_APPLICATION);// スモールアイコン
	::RegisterClassEx(&wcex);									// ウィンドウクラスの登録

	// ウィンドウの作成
	HWND hWnd = ::CreateWindow(	"MainWindowClass",		// クラス名
								WindowTitle,			// タイトル
								WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,	// スタイル
								CW_USEDEFAULT,			// ｘ座標(Windowsに任せる)
								CW_USEDEFAULT,			// ｙ座標(Windowsに任せる)ｘｙで左上角の座標
								WindowSize.cx,			// 幅
								WindowSize.cy,			// 高さ
								HWND_DESKTOP,			// 無し
								(HMENU)NULL,			// メニュー無し
								hInstance,				// このプログラムのインスタンスのハンドル
								NULL);					// 追加引数無し
	if(hWnd == NULL)
		return NULL;

	// ウィンドウモードの場合
	if(bWindowed) {
		// クライアントサイズの変更
		setClientSize(hWnd,WindowSize);
		// ウィンドウ表示位置を画面の中心変更
		WindowMoveToCenter(hWnd);
	}
	else {
		DWORD WindowStyle;
		// ウィンドウスタイルをポップアップに指定
		WindowStyle = WS_POPUP | WS_VISIBLE;
		::SetWindowLong(hWnd,GWL_STYLE,WindowStyle);		// ウィンドウスタイルの変更
		memcpy(&m_WindowSize,&WindowSize,sizeof(SIZE));
	}

	return hWnd;
}
// ------------------------------------------------------------------------
//　関数名	:setClientSize				ライアント領域のサイズを指定
//	引数	:hWnd						ウィンドウハンドル
//			:Size						指定するサイズ
//	戻り値	:							なし	
// ------------------------------------------------------------------------
void CApplication::setClientSize(HWND hWnd,SIZE size)
{
	// クライアント領域サイズの取得
	SIZE ClientSize = getClientSize(hWnd);
	// クライアント領域と設定するサイズの差を取得
	SIZE CompensationSize;
	CompensationSize.cx = size.cx - ClientSize.cx;
	CompensationSize.cy = size.cy - ClientSize.cy;
	// ウィンドウ矩形の幅と高さを取得
	SIZE WindowSize = getWindowSize(hWnd);
	// ウィンドウの幅と高さを指定
	m_WindowSize;
	m_WindowSize.cx = WindowSize.cx + CompensationSize.cx;
	m_WindowSize.cy = WindowSize.cy + CompensationSize.cy;
	// サイズを指定
	::SetWindowPos(hWnd,NULL,0,0,m_WindowSize.cx,m_WindowSize.cy,SWP_HIDEWINDOW);
}
// ------------------------------------------------------------------------
//　関数名	:WindowMoveToCenter			ウィンドウを画面中心へ移動
//	引数	:hWnd						ウィンドウハンドル
//	戻り値	:							なし	
// ------------------------------------------------------------------------
void CApplication::WindowMoveToCenter(HWND hWnd)
{
	// デスクトップの矩形を取得
	RECT rcDesktop;
	::GetWindowRect(::GetDesktopWindow(),(LPRECT)&rcDesktop);
	RECT rcWindow;
	::GetWindowRect(hWnd,(LPRECT)&rcWindow);
	// ウィンドウの中心座標を求め最終的に位置を決定
	m_ptWindowShowPosition.x = (rcDesktop.right - (rcWindow.right - rcWindow.left)) / 2;
	m_ptWindowShowPosition.y = (rcDesktop.bottom - (rcWindow.bottom - rcWindow.top)) / 2;
	// ポジションを移動する
	::SetWindowPos(hWnd,HWND_TOP,m_ptWindowShowPosition.x,m_ptWindowShowPosition.y,(rcWindow.right - rcWindow.left),(rcWindow.bottom - rcWindow.top),SWP_HIDEWINDOW);
}
// ------------------------------------------------------------------------
//　関数名	:getWindowSize				ウィンドウサイズを取得
//	引数	:hWnd						ウィンドウハンドル
//	戻り値	:							ウィンドウサイズ	
// ------------------------------------------------------------------------
SIZE CApplication::getWindowSize(HWND hWnd)
{
	// ウィンドウの矩形を取得
	RECT rcWindow;
	::GetWindowRect(hWnd,&rcWindow);

	// サイズを求める
	SIZE Result;
	Result.cx = rcWindow.right - rcWindow.left;
	Result.cy = rcWindow.bottom - rcWindow.top;
	
	return Result;
}
// ------------------------------------------------------------------------
//　関数名	:getClientSize				クライアント領域サイズを取得
//	引数	:hWnd						ウィンドウハンドル
//	戻り値	:							クライアント領域サイズ	
// ------------------------------------------------------------------------
SIZE CApplication::getClientSize(HWND hWnd)
{
	// クライアント領域の矩形を取得
	RECT rcClient;
	::GetClientRect(hWnd,&rcClient);

	// サイズを求める
	SIZE Result;
	Result.cx = rcClient.right - rcClient.left;
	Result.cy = rcClient.bottom - rcClient.top;

	return Result;
}
// ------------------------------------------------------------------------
//　関数名	:getInstance				インスタンスの取得
//	引数	:							なし
//	戻り値	:							インスタンスハンドル	
// ------------------------------------------------------------------------
CApplication* CApplication::getInstance()
{
	return &m_pAppInstance;
}
