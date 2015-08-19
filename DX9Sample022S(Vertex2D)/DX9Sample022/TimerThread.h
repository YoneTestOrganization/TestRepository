#pragma once
// -------------------- IncludeFile -----------------
#include "AppState.h"
#include "DXDevice.h"
#include "Global.h"
// -------------------- クラス宣言 -------------------
class CTimerThread  
{
private:
	HANDLE					m_hTimerThread;			// ハンドル
	static HANDLE			m_hTimerEvent;			// スレッド管理用イベント
	BOOL					m_bTimerThreadSleep;	// タイマースレッド待機の有無
public:
	CAppState*				m_pAppState;
	CDXDevice*				m_pDXDevice;
private:
	static void Run(CTimerThread*);
public:
	CTimerThread();
	virtual ~CTimerThread();
	void Start(CDXDevice*,CAppState*);
	void setTimerThreadSleep(BOOL);
	BOOL getTimerThreadSleep();
};
