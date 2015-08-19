// -------------------- IncludeFile -----------------
#include "Application.h"
CApplication CApplication::m_pAppInstance;	// �C���X�^���X�̎���
// �R���X�g���N�^
CApplication::CApplication()
{
	m_pAppState = new CAppState();
	m_ptWindowShowPosition.x = -1;
}
// �f�X�g���N�^
CApplication::~CApplication()
{
	delete m_pAppState;
}
// ------------------------------------------------------------------------
//�@�֐���	:Initialize					�A�v���P�[�V�����̏�����
//	����	:hInstance					�C���X�^���X�n���h��
//			:WindowSize					�E�B���h�E�T�C�Y
//			:bWindowed					TRUE:�E�B���h�E���[�h�AFALSE:�t���X�N���[�����[�h
//	�߂�l	:							�E�B���h�E�n���h��
// ------------------------------------------------------------------------
HWND CApplication::Initialize(HINSTANCE hInstance,
							  LPCTSTR WindowTitle,
							  SIZE WindowSize,
							  BOOL bWindowed)
{
	// �E�B���h�E�̏�����
	HWND hWnd = InitWindow(hInstance,WindowTitle,WindowSize,bWindowed);
	if(hWnd == NULL)
		return NULL;

	// �Q�[���f�o�C�X������������
	m_pDXDevice = new CDXDevice();
	HRESULT hResult = m_pDXDevice->Initialize(hWnd,bWindowed);
	if(FAILED(hResult)) {
		delete m_pDXDevice;
		return NULL;
	}

	// �^�C�}�[�̕���x��1ms�ɐݒ肷��
	timeBeginPeriod(1);

	// �E�B���h�E�̕\��
	::ShowWindow(hWnd,SW_SHOWNORMAL);

	// �J�[�\���ݒ�
	if(!bWindowed)
		PostMessage(hWnd,UM_SETCURSOR,0L,0L);

	return hWnd;
}
// ------------------------------------------------------------------------
//�@�֐���	:Run						�A�v���P�[�V�����̎��s
//	����	:hWnd						�E�B���h�E�n���h��
//	�߂�l	:							�A�v���P�[�V�����I���l
// ------------------------------------------------------------------------
int CApplication::Run(HWND hWnd)
{
	// �Q�[���X�s�[�h�Ǘ��X���b�h�쐬/���s
	CTimerThread* pTimerThread = new CTimerThread();
	pTimerThread->Start(m_pDXDevice,m_pAppState);

	// ���b�Z�[�W���[�v
	MSG msg;
	do {
		// ���b�Z�[�W����
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			DispatchMessage( &msg );
			TranslateMessage( &msg );
		}
		if(pTimerThread->getTimerThreadSleep()) {
			if(m_pDXDevice->getDeviceLost()) {
				if(m_pDXDevice->getDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
					if(pTimerThread->m_pDXDevice->RestoreDevice()) {
						pTimerThread->m_pDXDevice->setDeviceLost(FALSE);	// �f�o�C�X����������
						m_pDXDevice->setDeviceLost(TRUE);
					}
				}
			}
			else {
				BOOL bWindowed = m_pDXDevice->getWindowed();
				bWindowed ^= TRUE;
				m_pDXDevice->setWindowed(bWindowed);
				m_pDXDevice->ChangeDisplayMode();					// �\�����[�h�؂�ւ�
				// �E�B���h�E�X�^�C���̎w��
				DWORD WindowStyle;
				if(bWindowed) {
					// �E�B���h�E�X�^�C�����w��
					WindowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
				}
				else {
					// �E�B���h�E�X�^�C�����|�b�v�A�b�v�Ɏw��
					WindowStyle = WS_POPUP | WS_VISIBLE;
				}
				::SetWindowLong(hWnd,GWL_STYLE,WindowStyle);		// �E�B���h�E�X�^�C���̕ύX

				// �E�B���h�E�̔z�u
				if(bWindowed) {										// �E�B���h�E���[�h���̏���
					if(m_ptWindowShowPosition.x == -1) {
						// �N���C�A���g�T�C�Y�̕ύX
						setClientSize(hWnd,m_WindowSize);
						// �E�B���h�E�\���ʒu����ʂ̒��S�ύX
						WindowMoveToCenter(hWnd);
					}
					::SetWindowPos(hWnd,HWND_TOP,m_ptWindowShowPosition.x,m_ptWindowShowPosition.y,m_WindowSize.cx,m_WindowSize.cy,SWP_SHOWWINDOW);
				}
			}
			pTimerThread->setTimerThreadSleep(FALSE);
		}
		Sleep(1);
	} while(msg.message != WM_QUIT);

	// �Q�[���X�s�[�h�Ǘ��X���b�h�I��
	delete pTimerThread;

	// ���\�[�X�J��
	m_pAppInstance.m_pDXDevice->CleanupResource();

	// �Q�[���X���b�h�I��
	delete m_pDXDevice;

	// �^�C�}�[�̕���x��1ms�ɐݒ肷��
	timeEndPeriod(1);

	// �߂�l(���b�Z�[�W��wParam�p�����[�^)
	return (int)msg.wParam;
}
// ------------------------------------------------------------------------
//�@�֐���	:WndProc					�E�B���h�E�̃v���V�[�W��	
//	����	:hWnd						�E�B���h�E�̃n���h��
//			:uMessage					���b�Z�[�W�̎��
//			:wParam						���b�Z�[�W�̕⏕���
//			:lParam						���b�Z�[�W�̕⏕���
//	�߂�l	:							���b�Z�[�W��߂�		
// ------------------------------------------------------------------------
LRESULT CALLBACK CApplication::WndProc(HWND hWnd,UINT uMessage,WPARAM wParam,LPARAM lParam)
{
	// ���b�Z�[�W����
	switch(uMessage) {
		case WM_ACTIVATE:
			if(HIWORD(wParam))
				m_pAppInstance.m_pAppState->setIsIcon(TRUE);
			else
				m_pAppInstance.m_pAppState->setIsIcon(FALSE);
			break;
		case WM_MOVE:
			{
				if(!m_pAppInstance.m_pDXDevice)
					break;
				if(m_pAppInstance.m_ptWindowShowPosition.x == -1)
					break;
				if(m_pAppInstance.m_pDXDevice->getWindowed()) {
					RECT rc;
					GetWindowRect(hWnd,&rc);
					m_pAppInstance.m_ptWindowShowPosition.x = rc.left;
					m_pAppInstance.m_ptWindowShowPosition.y = rc.top;
				}
			}
			break;
		// �J�[�\���̐ݒ菈��
		case UM_SETCURSOR:
			if(m_pAppInstance.m_pDXDevice->getWindowed())
				ShowCursor(TRUE);				// �J�[�\���\��
			else
				ShowCursor(FALSE);				// �J�[�\����\��
			break;
		// �E�B���h�E�̔j������	
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);								// �E�B���h�E�I���ʒm
			break;
	}
	return DefWindowProc(hWnd,uMessage,wParam,lParam);
}
// ------------------------------------------------------------------------
//�@�֐���	:InitWindow					�E�B���h�E�̏�����	
//	����	:hInstance					�C���X�^���X�n���h��
//			:WindowTitle				�^�C�g����
//			:WindowSize					�E�B���h�E�T�C�Y
//			:bWindowed					TRUE:�E�B���h�E���[�h�AFALSE:�t���X�N���[�����[�h
//	�߂�l	:							�E�B���h�E�n���h��		
// ------------------------------------------------------------------------
HWND CApplication::InitWindow(	HINSTANCE hInstance,
								LPCSTR WindowTitle,
								SIZE WindowSize,
								BOOL bWindowed)
{
	// �E�B���h�E�N���X�̓o�^
	WNDCLASSEX wcex;	// �E�B���h�E�N���X�̏��i�[�p�\���̕ϐ�
	ZeroMemory(&wcex,sizeof(wcex));
	wcex.cbSize			= sizeof(WNDCLASSEX);					// �\���̂̃T�C�Y��ݒ�
	wcex.style			= CS_HREDRAW | CS_VREDRAW;				// �E�B���h�E�X�^�C��(��/�����ύX����)
	wcex.lpfnWndProc	= (WNDPROC)WndProc;						// �E�B���h�E�v���V�[�W���̎w��
	wcex.cbClsExtra		= 0;									// �ǉ��̈�͎g��Ȃ�
	wcex.cbWndExtra		= 0;									// �ǉ��̈�͎g��Ȃ�
	wcex.hInstance		= hInstance;							// ���̃C���X�^���X�̃n���h��
	wcex.hIcon			= LoadIcon(hInstance,IDI_APPLICATION);	// ���[�W�A�C�R��
	wcex.hCursor		= LoadCursor(NULL,IDC_ARROW);			// �J�[�\���X�^�C��
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	// �w�i�F ���F
	wcex.lpszMenuName	= NULL;									// �\�����j���[�̃Z���N�g(���j���[�Ȃ�)
	wcex.lpszClassName	= TEXT("MainWindowClass");				// �E�B���h�E�N���X��
	wcex.hIconSm		= LoadIcon(wcex.hInstance,(LPCTSTR)IDI_APPLICATION);// �X���[���A�C�R��
	::RegisterClassEx(&wcex);									// �E�B���h�E�N���X�̓o�^

	// �E�B���h�E�̍쐬
	HWND hWnd = ::CreateWindow(	"MainWindowClass",		// �N���X��
								WindowTitle,			// �^�C�g��
								WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,	// �X�^�C��
								CW_USEDEFAULT,			// �����W(Windows�ɔC����)
								CW_USEDEFAULT,			// �����W(Windows�ɔC����)�����ō���p�̍��W
								WindowSize.cx,			// ��
								WindowSize.cy,			// ����
								HWND_DESKTOP,			// ����
								(HMENU)NULL,			// ���j���[����
								hInstance,				// ���̃v���O�����̃C���X�^���X�̃n���h��
								NULL);					// �ǉ���������
	if(hWnd == NULL)
		return NULL;

	// �E�B���h�E���[�h�̏ꍇ
	if(bWindowed) {
		// �N���C�A���g�T�C�Y�̕ύX
		setClientSize(hWnd,WindowSize);
		// �E�B���h�E�\���ʒu����ʂ̒��S�ύX
		WindowMoveToCenter(hWnd);
	}
	else {
		DWORD WindowStyle;
		// �E�B���h�E�X�^�C�����|�b�v�A�b�v�Ɏw��
		WindowStyle = WS_POPUP | WS_VISIBLE;
		::SetWindowLong(hWnd,GWL_STYLE,WindowStyle);		// �E�B���h�E�X�^�C���̕ύX
		memcpy(&m_WindowSize,&WindowSize,sizeof(SIZE));
	}

	return hWnd;
}
// ------------------------------------------------------------------------
//�@�֐���	:setClientSize				���C�A���g�̈�̃T�C�Y���w��
//	����	:hWnd						�E�B���h�E�n���h��
//			:Size						�w�肷��T�C�Y
//	�߂�l	:							�Ȃ�	
// ------------------------------------------------------------------------
void CApplication::setClientSize(HWND hWnd,SIZE size)
{
	// �N���C�A���g�̈�T�C�Y�̎擾
	SIZE ClientSize = getClientSize(hWnd);
	// �N���C�A���g�̈�Ɛݒ肷��T�C�Y�̍����擾
	SIZE CompensationSize;
	CompensationSize.cx = size.cx - ClientSize.cx;
	CompensationSize.cy = size.cy - ClientSize.cy;
	// �E�B���h�E��`�̕��ƍ������擾
	SIZE WindowSize = getWindowSize(hWnd);
	// �E�B���h�E�̕��ƍ������w��
	m_WindowSize;
	m_WindowSize.cx = WindowSize.cx + CompensationSize.cx;
	m_WindowSize.cy = WindowSize.cy + CompensationSize.cy;
	// �T�C�Y���w��
	::SetWindowPos(hWnd,NULL,0,0,m_WindowSize.cx,m_WindowSize.cy,SWP_HIDEWINDOW);
}
// ------------------------------------------------------------------------
//�@�֐���	:WindowMoveToCenter			�E�B���h�E����ʒ��S�ֈړ�
//	����	:hWnd						�E�B���h�E�n���h��
//	�߂�l	:							�Ȃ�	
// ------------------------------------------------------------------------
void CApplication::WindowMoveToCenter(HWND hWnd)
{
	// �f�X�N�g�b�v�̋�`���擾
	RECT rcDesktop;
	::GetWindowRect(::GetDesktopWindow(),(LPRECT)&rcDesktop);
	RECT rcWindow;
	::GetWindowRect(hWnd,(LPRECT)&rcWindow);
	// �E�B���h�E�̒��S���W�����ߍŏI�I�Ɉʒu������
	m_ptWindowShowPosition.x = (rcDesktop.right - (rcWindow.right - rcWindow.left)) / 2;
	m_ptWindowShowPosition.y = (rcDesktop.bottom - (rcWindow.bottom - rcWindow.top)) / 2;
	// �|�W�V�������ړ�����
	::SetWindowPos(hWnd,HWND_TOP,m_ptWindowShowPosition.x,m_ptWindowShowPosition.y,(rcWindow.right - rcWindow.left),(rcWindow.bottom - rcWindow.top),SWP_HIDEWINDOW);
}
// ------------------------------------------------------------------------
//�@�֐���	:getWindowSize				�E�B���h�E�T�C�Y���擾
//	����	:hWnd						�E�B���h�E�n���h��
//	�߂�l	:							�E�B���h�E�T�C�Y	
// ------------------------------------------------------------------------
SIZE CApplication::getWindowSize(HWND hWnd)
{
	// �E�B���h�E�̋�`���擾
	RECT rcWindow;
	::GetWindowRect(hWnd,&rcWindow);

	// �T�C�Y�����߂�
	SIZE Result;
	Result.cx = rcWindow.right - rcWindow.left;
	Result.cy = rcWindow.bottom - rcWindow.top;
	
	return Result;
}
// ------------------------------------------------------------------------
//�@�֐���	:getClientSize				�N���C�A���g�̈�T�C�Y���擾
//	����	:hWnd						�E�B���h�E�n���h��
//	�߂�l	:							�N���C�A���g�̈�T�C�Y	
// ------------------------------------------------------------------------
SIZE CApplication::getClientSize(HWND hWnd)
{
	// �N���C�A���g�̈�̋�`���擾
	RECT rcClient;
	::GetClientRect(hWnd,&rcClient);

	// �T�C�Y�����߂�
	SIZE Result;
	Result.cx = rcClient.right - rcClient.left;
	Result.cy = rcClient.bottom - rcClient.top;

	return Result;
}
// ------------------------------------------------------------------------
//�@�֐���	:getInstance				�C���X�^���X�̎擾
//	����	:							�Ȃ�
//	�߂�l	:							�C���X�^���X�n���h��	
// ------------------------------------------------------------------------
CApplication* CApplication::getInstance()
{
	return &m_pAppInstance;
}
