// -------------------- IncludeFile -----------------
#include "DInput.h"
#include "Global.h"
// コンストラクタ
CDInput::CDInput(HWND hWnd)
{
	m_pDI = NULL;
	m_pDIKeyboard = NULL;
	InitDirectInput(hWnd);
}
// デストラクタ
CDInput::~CDInput()
{
	ReleaseDirectInput();
}
// ------------------------------------------------------------------------
//	関数	:InitDirectInput			DirectInput初期化
//	引数	:hWnd						ウィンドウハンドル
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDInput::InitDirectInput(HWND hWnd)
{
	// DirectInputシステム作成
	::DirectInput8Create((HINSTANCE)((UINT64)::GetWindowLong(hWnd,GWL_HINSTANCE)),	// プログラムのインスタントハンドルを指定
						DIRECTINPUT_VERSION,	// バージョン情報を指定(通常指定)
						IID_IDirectInput8A,		// 目的のインターフェイスの一意な識別子(通常指定)
						(void **)&m_pDI,		// DirectInputシステムのアドレスを指定
						NULL);					// COM 集成体に対する制御オブジェクトのIUnknownインターフェイスのアドレスへのポインタ(通常指定)

	// DirectInputDeviceコネクタ作成
	// キーボードオブジェクト作成
	m_pDI->CreateDevice(	GUID_SysKeyboard,	// 入力機器のGUIDを指定(キーボード)
							&m_pDIKeyboard,		// DirectInputコネクタのアドレスを指定
							NULL);				// COM 集成体に対する制御オブジェクトのIUnknownインターフェイスのアドレス

	// 入力データ形式指定
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);	// キーボード入力データ形式指定
	
	// 協調レベル設定
	// キーボード協調レベル設定
	m_pDIKeyboard->SetCooperativeLevel(hWnd,		// ウインドウハンドル
										DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
													// 協調レベル設定(フォアグラウンド時のみ入力情報取得/非排他アクセス権を要求)

	// 入力機器の取得
	m_pDIKeyboard->Acquire();						// キーボードから入力取得を開始
}
// ------------------------------------------------------------------------
//	関数		:getKeyboardState			キーボード入力情報の取得
//	引数		:pKeyData					キー情報配列のアドレス
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDInput::getKeyboardState(LPBYTE pKeyData)
{
	HRESULT hr;												// メソッド失敗時値代入
	ZeroMemory(pKeyData,sizeof(BYTE) * BUFFER_SIZE);		// 配列を０で初期化

	// 直接入力データ取得処理
	hr = m_pDIKeyboard->GetDeviceState(	BUFFER_SIZE,		// 格納配列要素数を指定
										pKeyData);			// 格納配列のアドレスを指定

	if((hr == DIERR_NOTACQUIRED) || (hr == DIERR_INPUTLOST))// 入力取得の失敗した場合
		m_pDIKeyboard->Acquire();							// 入力取得再開
}
// ------------------------------------------------------------------------
//	関数		:ReleaseDirectInput			DirectInputオブジェクト開放
//	引数		:							なし
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDInput::ReleaseDirectInput()
{
	// DirectInput関連オブジェクト開放
	if(m_pDI != NULL) {								// DirectInputシステムが存在する場合
		if(m_pDIKeyboard != NULL) {					// DirectInputDeviceキーボードコネクションが存在する場合
			m_pDIKeyboard->Unacquire();				// キーボード取得停止
			SAFE_RELEASE(m_pDIKeyboard);			// DirectInputDeviceキーボードコネクションの開放
		}
		SAFE_RELEASE(m_pDI);						// DirectInputシステムの開放
	}
}
