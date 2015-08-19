#pragma once
// -------------------- IncludeFile -----------------
#include "Global.h"
#include "AppState.h"
#include "TimerThread.h"
#include "DXDevice.h"
// -------------------- クラス宣言 -------------------
class CApplication  
{
private:
	static CApplication		m_pAppInstance;			// インスタンス
	BOOL					m_bWindowActive;		// 最小化の有無
	POINT					m_ptWindowShowPosition;	// 表示座標
	SIZE					m_WindowSize;			// サイズ
	CAppState*				m_pAppState;			// アプリケーション状態
	CDXDevice*				m_pDXDevice;			// ゲームデバイス
public:
	static CApplication* getInstance();
	HWND Initialize(HINSTANCE,LPCTSTR,SIZE,BOOL);
	int	Run(HWND);
	virtual ~CApplication();
private:
	CApplication();
	static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
	HWND InitWindow(HINSTANCE,LPCTSTR,SIZE,BOOL);
	SIZE getClientSize(HWND);
	SIZE getWindowSize(HWND);
	void WindowMoveToCenter(HWND);
	void setClientSize(HWND,SIZE);
};
