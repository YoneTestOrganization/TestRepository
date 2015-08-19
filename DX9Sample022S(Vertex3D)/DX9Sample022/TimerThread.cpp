// -------------------- IncludeFile -----------------
#include "TimerThread.h"
HANDLE CTimerThread::m_hTimerEvent = NULL;	// �n���h������
// �R���X�g���N�^
CTimerThread::CTimerThread()
{
	m_hTimerThread = NULL;
	m_bTimerThreadSleep = FALSE;
	m_pAppState = NULL;
	m_pDXDevice = NULL;
}
// �f�X�g���N�^
CTimerThread::~CTimerThread()
{
	ResetEvent(m_hTimerEvent);	// �C�x���g�I�u�W�F�N�g���V�O�i����Ԃɐݒ�
	while(WaitForSingleObject(m_hTimerThread,0) == WAIT_TIMEOUT)
		Sleep(1);
	m_hTimerThread = NULL;
}
// ------------------------------------------------------------------------
//�@�֐���	:Start						�^�C�}�[�X���b�h�̍쐬
//	����		:pDXDevice					�f�o�C�X
//			:pAppState					�A�v���P�[�V�������
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CTimerThread::Start(CDXDevice* pDXDevice,CAppState* pAppState)
{
	m_pAppState = pAppState;
	m_pDXDevice = pDXDevice;
	// �X���b�h�J�n
	DWORD threadID;							// �X���b�h�pID
	m_hTimerThread = CreateThread(	0,									// �Z�L�����e�B����(�Ȃ�)
									0,									// ���b�h�����X�^�b�N�̃T�C�Y���A�o�C�g�P�ʂŎw��(�f�t�H���g)
									(LPTHREAD_START_ROUTINE)Run,		// �X���b�h�̊J�n�A�h���X���w��
									(VOID *)this,						// �X���b�h�ւ̈����w��(�A�h���X)
									0,									// ����t���O���w��(�ݒ�Ȃ�)
									&threadID);							// �X���b�hID���i�[����Ă���|�C���^�w�� 
	// �C�x���g�I�u�W�F�N�g�쐬
	m_hTimerEvent = CreateEvent(0,		// �Z�L�����e�B����(�ݒ�Ȃ�)
								TRUE,	// �蓮/�������Z�b�g�I�u�W�F�N�g�쐬
								TRUE,	// �C�x���g�I�u�W�F�N�g�̏�����Ԏw��(�V�O�i�����)
								0);		// �C�x���g�I�u�W�F�N�g�̖��O
	SetEvent(m_hTimerEvent);			// �I�u�W�F�N�g���V�O�i����Ԃɐݒ�
}
// ------------------------------------------------------------------------
//�@�֐���	:Run						�A�v���P�[�V�����̎��s
//	����		:pTimerThread				�^�C�}�[�X���b�h
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CTimerThread::Run(CTimerThread* pTimerThread)
{
	static DWORD dwOldTime,dwNowTime;	// ���ԕۑ��p
	DWORD dwTimer = 20;					// �Q�[���X�s�[�h��0.02�b�Ԋu�Ɏw��

	while(TRUE) {
		if(WaitForSingleObject(m_hTimerEvent,0) == WAIT_TIMEOUT)
			break;
		if(pTimerThread->m_bTimerThreadSleep)
			continue;
		if(!pTimerThread->m_pDXDevice->getDeviceLost()) {
			dwNowTime = timeGetTime();				// ���݂̎��Ԃ��擾
			if(dwNowTime - dwOldTime < dwTimer){	// ���݂̎��� - �ߋ��̎��� < 0.03�b�̏ꍇ
				Sleep(dwTimer - (dwNowTime - dwOldTime));// ���ԑ҂�
				dwNowTime = timeGetTime();			// ���݂̎��Ԃ��擾
			}
			dwOldTime = dwNowTime;					// ���݂̎��Ԃ��ߋ��̎��Ԃɒu��������
			// �s���̍X�V

			// ��ʂ̍X�V
			if(pTimerThread->m_pDXDevice && pTimerThread->m_pDXDevice->getD3DSystem()) {
				HRESULT hr = S_OK;
				if((pTimerThread->m_pDXDevice->getWindowed() && !pTimerThread->m_pAppState->getIsIcon())
					|| !pTimerThread->m_pDXDevice->getWindowed())
					pTimerThread->m_pDXDevice->getDInput()->getKeyboardState(pTimerThread->m_pDXDevice->getKeyStateArray());	// �L�[���͏��擾
					LPBYTE p = pTimerThread->m_pDXDevice->getKeyStateArray();
					if(p[DIK_ESCAPE] & 0x80) {
						SendMessage(pTimerThread->m_pDXDevice->GetWindowHandle(),WM_CLOSE,0L,0L);
						break;
					}
					hr = pTimerThread->m_pDXDevice->Render();			// �����_�����O����
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
//�@�֐���	:setTimerThreadSleep		�^�C�}�[�X���b�h�ҋ@�̎w��
//	����		:bTimerThreadSleep			�ҋ@�̗L��
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CTimerThread::setTimerThreadSleep(BOOL bTimerThreadSleep)
{
	m_bTimerThreadSleep = bTimerThreadSleep;
}
// ------------------------------------------------------------------------
//�@�֐���	:getTimerThreadSleep		�^�C�}�[�X���b�h�ҋ@�̎擾
//	����		:							�Ȃ�
//	�߂�l	:							�^�C�}�[�X���b�h�ҋ@�̗L��
// ------------------------------------------------------------------------
BOOL CTimerThread::getTimerThreadSleep()
{
	return m_bTimerThreadSleep;
}