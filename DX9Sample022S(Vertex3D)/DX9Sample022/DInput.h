#pragma once
// -------------------- Define�錾 ------------------
#define DIRECTINPUT_VERSION 0x0800			// DirectInputVersion�ݒ�(DX8.0�ȍ~)
#define BUFFER_SIZE		256					// �o�b�t�@�T�C�Y
// -------------------- IncludeFile -----------------
#include <dinput.h>
#include "Global.h"
// -------------------- �N���X�錾 -------------------
class CDInput
{
private:
	LPDIRECTINPUT8			m_pDI;					// DirectInput�V�X�e��
	LPDIRECTINPUTDEVICE8	m_pDIKeyboard;			// DirectInputDevice�L�[�{�[�h�R�l�N�^
public:
	CDInput(HWND);
	virtual ~CDInput();
	void getKeyboardState(LPBYTE);
private:
	void InitDirectInput(HWND);
	void ReleaseDirectInput();
};