// -------------------- IncludeFile -----------------
#include "DInput.h"
#include "Global.h"
// �R���X�g���N�^
CDInput::CDInput(HWND hWnd)
{
	m_pDI = NULL;
	m_pDIKeyboard = NULL;
	InitDirectInput(hWnd);
}
// �f�X�g���N�^
CDInput::~CDInput()
{
	ReleaseDirectInput();
}
// ------------------------------------------------------------------------
//	�֐�	:InitDirectInput			DirectInput������
//	����	:hWnd						�E�B���h�E�n���h��
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDInput::InitDirectInput(HWND hWnd)
{
	// DirectInput�V�X�e���쐬
	::DirectInput8Create((HINSTANCE)((UINT64)::GetWindowLong(hWnd,GWL_HINSTANCE)),	// �v���O�����̃C���X�^���g�n���h�����w��
						DIRECTINPUT_VERSION,	// �o�[�W���������w��(�ʏ�w��)
						IID_IDirectInput8A,		// �ړI�̃C���^�[�t�F�C�X�̈�ӂȎ��ʎq(�ʏ�w��)
						(void **)&m_pDI,		// DirectInput�V�X�e���̃A�h���X���w��
						NULL);					// COM �W���̂ɑ΂��鐧��I�u�W�F�N�g��IUnknown�C���^�[�t�F�C�X�̃A�h���X�ւ̃|�C���^(�ʏ�w��)

	// DirectInputDevice�R�l�N�^�쐬
	// �L�[�{�[�h�I�u�W�F�N�g�쐬
	m_pDI->CreateDevice(	GUID_SysKeyboard,	// ���͋@���GUID���w��(�L�[�{�[�h)
							&m_pDIKeyboard,		// DirectInput�R�l�N�^�̃A�h���X���w��
							NULL);				// COM �W���̂ɑ΂��鐧��I�u�W�F�N�g��IUnknown�C���^�[�t�F�C�X�̃A�h���X

	// ���̓f�[�^�`���w��
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);	// �L�[�{�[�h���̓f�[�^�`���w��
	
	// �������x���ݒ�
	// �L�[�{�[�h�������x���ݒ�
	m_pDIKeyboard->SetCooperativeLevel(hWnd,		// �E�C���h�E�n���h��
										DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
													// �������x���ݒ�(�t�H�A�O���E���h���̂ݓ��͏��擾/��r���A�N�Z�X����v��)

	// ���͋@��̎擾
	m_pDIKeyboard->Acquire();						// �L�[�{�[�h������͎擾���J�n
}
// ------------------------------------------------------------------------
//	�֐�		:getKeyboardState			�L�[�{�[�h���͏��̎擾
//	����		:pKeyData					�L�[���z��̃A�h���X
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDInput::getKeyboardState(LPBYTE pKeyData)
{
	HRESULT hr;												// ���\�b�h���s���l���
	ZeroMemory(pKeyData,sizeof(BYTE) * BUFFER_SIZE);		// �z����O�ŏ�����

	// ���ړ��̓f�[�^�擾����
	hr = m_pDIKeyboard->GetDeviceState(	BUFFER_SIZE,		// �i�[�z��v�f�����w��
										pKeyData);			// �i�[�z��̃A�h���X���w��

	if((hr == DIERR_NOTACQUIRED) || (hr == DIERR_INPUTLOST))// ���͎擾�̎��s�����ꍇ
		m_pDIKeyboard->Acquire();							// ���͎擾�ĊJ
}
// ------------------------------------------------------------------------
//	�֐�		:ReleaseDirectInput			DirectInput�I�u�W�F�N�g�J��
//	����		:							�Ȃ�
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDInput::ReleaseDirectInput()
{
	// DirectInput�֘A�I�u�W�F�N�g�J��
	if(m_pDI != NULL) {								// DirectInput�V�X�e�������݂���ꍇ
		if(m_pDIKeyboard != NULL) {					// DirectInputDevice�L�[�{�[�h�R�l�N�V���������݂���ꍇ
			m_pDIKeyboard->Unacquire();				// �L�[�{�[�h�擾��~
			SAFE_RELEASE(m_pDIKeyboard);			// DirectInputDevice�L�[�{�[�h�R�l�N�V�����̊J��
		}
		SAFE_RELEASE(m_pDI);						// DirectInput�V�X�e���̊J��
	}
}
