#pragma once
// -------------------- IncludeFile -----------------
#include "AppState.h"
#include "DXDevice.h"
#include "Global.h"
// -------------------- �N���X�錾 -------------------
class CTimerThread  
{
private:
	HANDLE					m_hTimerThread;			// �n���h��
	static HANDLE			m_hTimerEvent;			// �X���b�h�Ǘ��p�C�x���g
	BOOL					m_bTimerThreadSleep;	// �^�C�}�[�X���b�h�ҋ@�̗L��
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
