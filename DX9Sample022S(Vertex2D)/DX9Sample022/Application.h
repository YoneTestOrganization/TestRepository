#pragma once
// -------------------- IncludeFile -----------------
#include "Global.h"
#include "AppState.h"
#include "TimerThread.h"
#include "DXDevice.h"
// -------------------- �N���X�錾 -------------------
class CApplication  
{
private:
	static CApplication		m_pAppInstance;			// �C���X�^���X
	BOOL					m_bWindowActive;		// �ŏ����̗L��
	POINT					m_ptWindowShowPosition;	// �\�����W
	SIZE					m_WindowSize;			// �T�C�Y
	CAppState*				m_pAppState;			// �A�v���P�[�V�������
	CDXDevice*				m_pDXDevice;			// �Q�[���f�o�C�X
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
