// -------------------- IncludeFile -----------------
#include "TimerThread.h"
HANDLE CTimerThread::m_hTimerEvent = NULL;	// ハンドル実態
// コンストラクタ
CTimerThread::CTimerThread()
{
	m_hTimerThread = NULL;
	m_bTimerThreadSleep = FALSE;
	m_pAppState = NULL;
	m_pDXDevice = NULL;
}
// デストラクタ
CTimerThread::~CTimerThread()
{
	ResetEvent(m_hTimerEvent);	// イベントオブジェクトを非シグナル状態に設定
	while(WaitForSingleObject(m_hTimerThread,0) == WAIT_TIMEOUT)
		Sleep(1);
	m_hTimerThread = NULL;
}
// ------------------------------------------------------------------------
//　関数名	:Start						タイマースレッドの作成
//	引数		:pDXDevice					デバイス
//			:pAppState					アプリケーション状態
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CTimerThread::Start(CDXDevice* pDXDevice,CAppState* pAppState)
{
	m_pAppState = pAppState;
	m_pDXDevice = pDXDevice;
	// スレッド開始
	DWORD threadID;							// スレッド用ID
	m_hTimerThread = CreateThread(	0,									// セキュリティ属性(なし)
									0,									// レッドが持つスタックのサイズを、バイト単位で指定(デフォルト)
									(LPTHREAD_START_ROUTINE)Run,		// スレッドの開始アドレスを指定
									(VOID *)this,						// スレッドへの引数指定(アドレス)
									0,									// 制御フラグを指定(設定なし)
									&threadID);							// スレッドIDが格納されているポインタ指定 
	// イベントオブジェクト作成
	m_hTimerEvent = CreateEvent(0,		// セキュリティ属性(設定なし)
								TRUE,	// 手動/自動リセットオブジェクト作成
								TRUE,	// イベントオブジェクトの初期状態指定(シグナル状態)
								0);		// イベントオブジェクトの名前
	SetEvent(m_hTimerEvent);			// オブジェクトをシグナル状態に設定
}
// ------------------------------------------------------------------------
//　関数名	:Run						アプリケーションの実行
//	引数		:pTimerThread				タイマースレッド
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CTimerThread::Run(CTimerThread* pTimerThread)
{
	static DWORD dwOldTime,dwNowTime;	// 時間保存用
	DWORD dwTimer = 20;					// ゲームスピードを0.02秒間隔に指定

	while(TRUE) {
		if(WaitForSingleObject(m_hTimerEvent,0) == WAIT_TIMEOUT)
			break;
		if(pTimerThread->m_bTimerThreadSleep)
			continue;
		if(!pTimerThread->m_pDXDevice->getDeviceLost()) {
			dwNowTime = timeGetTime();				// 現在の時間を取得
			if(dwNowTime - dwOldTime < dwTimer){	// 現在の時間 - 過去の時間 < 0.03秒の場合
				Sleep(dwTimer - (dwNowTime - dwOldTime));// 時間待ち
				dwNowTime = timeGetTime();			// 現在の時間を取得
			}
			dwOldTime = dwNowTime;					// 現在の時間を過去の時間に置き換える
			// 行動の更新

			// 画面の更新
			if(pTimerThread->m_pDXDevice && pTimerThread->m_pDXDevice->getD3DSystem()) {
				HRESULT hr = S_OK;
				if((pTimerThread->m_pDXDevice->getWindowed() && !pTimerThread->m_pAppState->getIsIcon())
					|| !pTimerThread->m_pDXDevice->getWindowed())
					pTimerThread->m_pDXDevice->getDInput()->getKeyboardState(pTimerThread->m_pDXDevice->getKeyStateArray());	// キー入力情報取得
					LPBYTE p = pTimerThread->m_pDXDevice->getKeyStateArray();
					if(p[DIK_ESCAPE] & 0x80) {
						SendMessage(pTimerThread->m_pDXDevice->GetWindowHandle(),WM_CLOSE,0L,0L);
						break;
					}
					hr = pTimerThread->m_pDXDevice->Render();			// レンダリング処理
					if(hr == D3DERR_DEVICELOST) {
						pTimerThread->m_pDXDevice->setDeviceLost(TRUE);
						pTimerThread->setTimerThreadSleep(TRUE);
					}
					if(hr == D3DERR_DEVICENOTRESET) {
						pTimerThread->m_pDXDevice->setDeviceLost(TRUE);
						pTimerThread->setTimerThreadSleep(TRUE);
					}
					if(pTimerThread->m_pDXDevice->getChangeDisplayMode())
						pTimerThread->setTimerThreadSleep(TRUE);
			}
		}
		else {
			if(pTimerThread->m_pDXDevice->RestoreDevice()) {
				pTimerThread->m_pDXDevice->setDeviceLost(FALSE);
				pTimerThread->m_pDXDevice->setChangeDisplayMode(FALSE);
			}
		}
		Sleep(1);
	}
}
// ------------------------------------------------------------------------
//　関数名	:setTimerThreadSleep		タイマースレッド待機の指定
//	引数		:bTimerThreadSleep			待機の有無
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CTimerThread::setTimerThreadSleep(BOOL bTimerThreadSleep)
{
	m_bTimerThreadSleep = bTimerThreadSleep;
}
// ------------------------------------------------------------------------
//　関数名	:getTimerThreadSleep		タイマースレッド待機の取得
//	引数		:							なし
//	戻り値	:							タイマースレッド待機の有無
// ------------------------------------------------------------------------
BOOL CTimerThread::getTimerThreadSleep()
{
	return m_bTimerThreadSleep;
}