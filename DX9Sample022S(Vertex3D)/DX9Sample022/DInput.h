#pragma once
// -------------------- Define宣言 ------------------
#define DIRECTINPUT_VERSION 0x0800			// DirectInputVersion設定(DX8.0以降)
#define BUFFER_SIZE		256					// バッファサイズ
// -------------------- IncludeFile -----------------
#include <dinput.h>
#include "Global.h"
// -------------------- クラス宣言 -------------------
class CDInput
{
private:
	LPDIRECTINPUT8			m_pDI;					// DirectInputシステム
	LPDIRECTINPUTDEVICE8	m_pDIKeyboard;			// DirectInputDeviceキーボードコネクタ
public:
	CDInput(HWND);
	virtual ~CDInput();
	void getKeyboardState(LPBYTE);
private:
	void InitDirectInput(HWND);
	void ReleaseDirectInput();
};