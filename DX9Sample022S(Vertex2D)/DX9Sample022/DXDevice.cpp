// -------------------- IncludeFile -----------------
#include "DXDevice.h"
// �R���X�g���N�^
CDXDevice::CDXDevice()
{
	m_pD3D = nullptr;
	m_pd3dDevice = nullptr;
	m_bDeviceLost = FALSE;
	m_bChangeDisplayMode = FALSE;
	m_pDInput = nullptr;
	m_pDXFont = nullptr;
	m_CleanupObject = FALSE;
	ZeroMemory(m_byKeyState,sizeof(BYTE) * BUFFER_SIZE);	// �������̃[���N���A

	m_pVertex = nullptr;

}
// �f�X�g���N�^
CDXDevice::~CDXDevice()
{
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	SAFE_DELETE(m_pDInput);

}
// ------------------------------------------------------------------------
//	�֐�	:CreateResource				���\�[�X�쐬
//			:							�f�o�C�X�����ɉe������Ȃ����\�[�X
//	����	:							�Ȃ�
//	�߂�l	:							TRUE:�����AFALSE:��������̖�蔭��
// ------------------------------------------------------------------------
BOOL CDXDevice::CreateResource()
{
	srand(timeGetTime());

	CCamera::Create(m_pd3dDevice);
	CDXFont::Create(m_pd3dDevice);

	m_pVertex = new CVertex(m_pd3dDevice);
	if (m_pVertex->Initialize() != TRUE)
	{
		return FALSE;
	}

	return TRUE;
}
// ------------------------------------------------------------------------
//	�֐�	:CleanupResource			���\�[�X�J��
//	����	:							�Ȃ�
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDXDevice::CleanupResource()
{
	CCamera::Destroy();
	CDXFont::Destroy();

	SAFE_DELETE( m_pVertex );

	CleanupObject();					// �I�u�W�F�N�g�J��
}
// ------------------------------------------------------------------------
//	�֐�	:CreateObject				�I�u�W�F�N�g�쐬
//			:							�f�o�C�X�����ɉe������郊�\�[�X
//	����	:							�Ȃ�
//	�߂�l	:							TRUE:�����AFALSE:��������̖�蔭��
// ------------------------------------------------------------------------
BOOL CDXDevice::CreateObject()
{
	if (CDXFont::GetInstance())
		CDXFont::GetInstance()->GetFont()->OnResetDevice();

	if (m_pVertex)
		m_pVertex->GetEffect()->OnResetDevice();

	return TRUE;
}
// ------------------------------------------------------------------------
//	�֐�	:CleanupObject				�I�u�W�F�N�g�J��
//	����	:							�Ȃ�
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDXDevice::CleanupObject()
{
	//delete m_pDXFont;
	if (CDXFont::GetInstance())
		CDXFont::GetInstance()->GetFont()->OnLostDevice();

	if (m_pVertex)
		m_pVertex->GetEffect()->OnLostDevice();

}
// ------------------------------------------------------------------------
//�@�֐���	:setDeviceLost				�f�o�C�X�����̎w��
//	����	:bIsIcon					�f�o�C�X�����̗L��
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDXDevice::setDeviceLost(BOOL bDeviceLost)
{
	m_bDeviceLost = bDeviceLost;
}
// ------------------------------------------------------------------------
//�@�֐���	:getDeviceLost				�f�o�C�X�����̎擾
//	����		:							�Ȃ�
//	�߂�l	:							�f�o�C�X�����̗L��
// ------------------------------------------------------------------------
BOOL CDXDevice::getDeviceLost()
{
	return m_bDeviceLost;
}
// ------------------------------------------------------------------------
//�@�֐���	:getD3DSystem				Direct3D�V�X�e���̎擾
//	����		:							�Ȃ�
//	�߂�l	:							Direct3D�V�X�e��
// ------------------------------------------------------------------------
LPDIRECT3D9 CDXDevice::getD3DSystem()
{
	return m_pD3D;
}
// ------------------------------------------------------------------------
//�@�֐���	:getDevice					�`��f�o�C�X�̎擾
//	����		:							�Ȃ�
//	�߂�l	:							�`��f�o�C�X
// ------------------------------------------------------------------------
LPDIRECT3DDEVICE9 CDXDevice::getDevice()
{
	return m_pd3dDevice;
}

// ------------------------------------------------------------------------
//�@�֐���	:getDInput					DirectInput�̎擾
//	����		:							�Ȃ�
//	�߂�l	:							DirectInput
// ------------------------------------------------------------------------
CDInput* CDXDevice::getDInput()
{
	return m_pDInput;
}
// ------------------------------------------------------------------------
//�@�֐���	:setDeviceLost				�E�B���h�E���[�h�̎w��
//	����		:bWindowed					�E�B���h�E���[�h/�t���X�N���[�����[�h
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDXDevice::setWindowed(BOOL bWindowed)
{
	m_bWindowed = bWindowed;
}
// ------------------------------------------------------------------------
//�@�֐���	:getWindowed				�E�B���h�E���[�h�̎擾
//	����		:							�Ȃ�
//	�߂�l	:							�E�B���h�E���[�h/�t���X�N���[�����[�h
// ------------------------------------------------------------------------
BOOL CDXDevice::getWindowed()
{
	return m_bWindowed;
}
// ------------------------------------------------------------------------
//�@�֐���	:setChangeDisplayMode		�f�B�X�v���C���[�h�ύX�̎w��
//	����		:bChangeDisplayMode			�f�B�X�v���C���[�h�ύX���s�̗L��
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDXDevice::setChangeDisplayMode(BOOL bChangeDisplayMode)
{
	m_bChangeDisplayMode = bChangeDisplayMode;
}
// ------------------------------------------------------------------------
//�@�֐���	:getChangeDisplayMode		�f�B�X�v���C���[�h�ύX�̎擾
//	����		:							�Ȃ�
//	�߂�l	:							�f�B�X�v���C���[�h�ύX���s�̗L��
// ------------------------------------------------------------------------
BOOL CDXDevice::getChangeDisplayMode()
{
	return m_bChangeDisplayMode;
}
// ------------------------------------------------------------------------
//�@�֐���	:getKeyStateArray			�L�[���͏��i�[�z��A�h���X�̎擾
//	����		:							�Ȃ�
//	�߂�l	:							�f�B�X�v���C���[�h�ύX���s�̗L��
// ------------------------------------------------------------------------
LPBYTE CDXDevice::getKeyStateArray()
{
	return m_byKeyState;
}
// ------------------------------------------------------------------------
//�@�֐���	:Initialize					�f�o�C�X�̏�����
//	����		:hWnd						�E�B���h�E�Y�n���h��
//			:bWindowed					TRUE:�E�B���h�E���[�h�AFALSE:�t���X�N���[�����[�h
//	�߂�l	:							S_OK:�����AE_FAIL:��������̖�蔭��
// ------------------------------------------------------------------------
HRESULT CDXDevice::Initialize(HWND hWnd,BOOL bWindowed)
{
	// Direct3D�V�X�e���̍쐬
	m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL) 
		return E_FAIL;

	// �v���[���g�p�����[�^�̏�����
	InitPresentParameters(hWnd,bWindowed);

	// Direct3DDevice�̍쐬
	HRESULT hResult = CreateDevice(hWnd,m_d3dpp);
	if(FAILED(hResult))
		return E_FAIL;
	// DirectInput�̍쐬
	m_pDInput = new CDInput(hWnd);

	if(!CreateResource()) {
		CleanupResource();
		return E_FAIL;
	}
	if(!CreateObject()) {
		CleanupObject();
		return E_FAIL;
	}

	return S_OK;
}
// ------------------------------------------------------------------------
//�@�֐���	:InitPresentParameters		�v���[���g�p�����[�^�̏�����
//	����		:hWnd						�E�B���h�E�Y�n���h��
//			:bWindowed					TRUE:�E�B���h�E���[�h�AFALSE:�t���X�N���[�����[�h
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDXDevice::InitPresentParameters(HWND hWnd,BOOL bWindowed)
{
	ZeroMemory(&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS));	// �������̃[���N���A
	m_d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;// �t���b�s���O���@:IDirect3DDevice9::Present�������o�b�N�o�b�t�@�̓��e�Ɉ�؉e�����Ȃ�
	m_d3dpp.BackBufferCount		= 1;					// �o�b�N�o�b�t�@�����Q�ɐݒ�
	m_d3dpp.EnableAutoDepthStencil= TRUE;				// �[�x�o�b�t�@���Ǘ���L��
	m_d3dpp.AutoDepthStencilFormat= D3DFMT_D24S8;		// 8�r�b�g�̃X�e���V���`�����l�����w��
	m_d3dpp.hDeviceWindow		= hWnd;					// �E�B���h�E�n���h��
	D3DDISPLAYMODE BaseFullScreenMode = {800,600,60,D3DFMT_R5G6B5};	// ��{���[�h
	D3DDISPLAYMODE FullScreenMode = {800,600,75,D3DFMT_X8R8G8B8};	// �w�胂�[�h
	m_bWindowed = bWindowed;
	m_hWnd = hWnd;

	m_Mode = CheckFullScreenAdapter(FullScreenMode);
	if(m_Mode.Format == D3DFMT_UNKNOWN)
		m_Mode = BaseFullScreenMode;

	//�f�B�X�v���C���[�h���擾
	m_pD3D->GetAdapterDisplayMode(	D3DADAPTER_DEFAULT	// �f�B�X�v���C�A�_�v�^�[�w��(�W���w��) 
									,&m_d3ddm);			// �f�B�X�v���C���[�h��ۑ�����\���̂��w��

	if(bWindowed) {		// �E�C���h�E���[�h�ɐݒ�
		m_d3dpp.Windowed = TRUE;					// �E�C���h�E���[�h�ɐݒ�
		RECT rcClient;
		::GetClientRect(hWnd,&rcClient);
		m_d3dpp.BackBufferWidth		= rcClient.right - rcClient.left;
		m_d3dpp.BackBufferHeight	= rcClient.bottom - rcClient.top;
		if(m_d3ddm.Format == D3DFMT_X8R8G8B8)
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;	// �F��:ARGB8bit
		else if(m_d3ddm.Format == D3DFMT_R5G6B5)
			m_d3dpp.BackBufferFormat = D3DFMT_A1R5G5B5;	// �F��:A1bitRGB5bit
		else
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;	// �F��:GetAdapterDisplayMode�Ŏ擾����Format���w��
		m_d3dpp.FullScreen_RefreshRateInHz = 0;			// ���t���b�V�����[�g���w��
	}
	else {
		m_d3dpp.Windowed = FALSE;						// �t���X�N���[�����[�h�ɐݒ�
		m_d3dpp.BackBufferWidth = m_Mode.Width;	// �o�b�N�o�b�t�@�����𑜓x
		m_d3dpp.BackBufferHeight = m_Mode.Height;// �o�b�N�o�b�t�@�����𑜓x
		m_d3dpp.FullScreen_RefreshRateInHz = m_Mode.RefreshRate;	// ���t���b�V�����[�g���w��
		m_d3dpp.BackBufferFormat = m_Mode.Format;	// �F�����w��
	}
}
// ------------------------------------------------------------------------
//�@�֐���	:CheckFullScreenAdapter		�O���t�B�b�N�A�_�v�^���̎擾
//	����		:FullScreenMode				�t���X�N���[�����[�h�̏��
//	�߂�l	:							���s�ł���t���X�N���[�����[�h
// ------------------------------------------------------------------------
D3DDISPLAYMODE CDXDevice::CheckFullScreenAdapter(D3DDISPLAYMODE FullScreenMode)
{
	D3DADAPTER_IDENTIFIER9 Adapter;
	D3DDISPLAYMODE ResultMode = {0,0,0,D3DFMT_UNKNOWN};
	D3DDISPLAYMODE Mode;
	int iLevel = 1000000;
	int iNumber,iCount,i,j;
	UINT uMaxRefreshRate = 0;
	int iSelectNumber = -1;
	D3DFORMAT AdapterFormat[6] = {D3DFMT_A2R10G10B10,D3DFMT_X8R8G8B8,D3DFMT_A8R8G8B8,D3DFMT_X1R5G5B5,D3DFMT_A1R5G5B5,D3DFMT_R5G6B5};

	iNumber = m_pD3D->GetAdapterCount();		// �V�X�e���ɂ���A�_�v�^�̐����擾
	if(iNumber != -1){							// �V�X�e���ɃA�_�v�^������ꍇ
		// �V�X�e���ɑ��݂��Ă��������I�ȃf�B�X�v���C�A�_�v�^���L�q
		m_pD3D->GetAdapterIdentifier(	0,			// �f�B�X�v���C�A�_�v�^�������������w��(DEFAULT���w��)
										0,			// (�ʏ�̓[���ɐݒ肷��p�����[�^)
										&Adapter);	// �w�肵���A�_�v�^���L�q�����񂪊i�[�����\���̂�
													// �|�C���^���w��
		for(j = 0;j < 6;j++) {
			iCount = 0;
			// �A�_�v�^�ŗ��p�\�ȃf�B�X�v���C���[�h�̐����擾
			iCount = m_pD3D->GetAdapterModeCount(	D3DADAPTER_DEFAULT,	// �f�B�X�v���C�A�_�v�^����������(DEFAULT���w��)
													AdapterFormat[j]);	// �f�B�X�v���C�t�H�[�}�b�g(16Bit)
			for(i = 0; i < iCount;i++) {
				// �A�_�v�^�̃f�B�X�v���C���[�h���
				if(m_pD3D->EnumAdapterModes(0,					// �Ɖ��f�B�X�v���C�A�_�v�^�������������w��
																// (DEFAULT���w��)
											AdapterFormat[j],	// �f�B�X�v���C�t�H�[�}�b�g
											i,					// �񋓂��郂�[�h������������
											&Mode) == D3D_OK){	// D3DDISPLAYMODE�\���̂ւ̃|�C���^���w��
					if(Mode.Format == FullScreenMode.Format) {
						int l = abs((int)(Mode.Width - FullScreenMode.Width)) + abs((int)(Mode.Height - FullScreenMode.Height)) + abs((int)(Mode.RefreshRate - FullScreenMode.RefreshRate));
						if(l < iLevel) {
							memcpy(&ResultMode,&Mode,sizeof(D3DDISPLAYMODE));
							iLevel = l;
						}
					}
				}
			}
		}
	}

	return ResultMode;
}
// ------------------------------------------------------------------------
//	�֐�1	:CreateDevic				�`��f�o�C�X�̍쐬
//	����		:hWnd						�E�B���h�E�n���h��
//			:d3dpp						�v���[���g�p�����[�^
//	�߂�l	:							S_OK:�o�^�ɐ���,E_FAIL:�o�^�Ɏ��s
// ------------------------------------------------------------------------
HRESULT CDXDevice::CreateDevice(HWND hWnd,D3DPRESENT_PARAMETERS d3dpp)
{
	// �f�o�C�X�̍쐬 �`��:HAL & 3D�v�Z:HARDWARE �Ŏ��s
	if(FAILED(m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,						// �f�B�X�v���C�A�_�v�^�[�w��(�W���w��)
									D3DDEVTYPE_HAL,							// �`��f�o�C�X�w��(�n�[�h�E�F�A(HAL)�w��)
									hWnd,									// �E�C���h�E�n���h��
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// �R�c�v�Z����(�n�[�h�E�F�A�Ŏ��{���w��)
									&m_d3dpp,								// �v���[���e�[�V�����p�����[�^�\���̊i�[�A�h���X�w��
									&m_pd3dDevice))) {							// �`��f�o�C�X�i�[�A�h���X�w��
		// ���s�����̂� �`��:HAL & 3D�v�Z:SOFTWARE �Ŏ��s
		if(FAILED(m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^�[�w��(�W���w��)
										D3DDEVTYPE_HAL,						// �`��f�o�C�X�w��(�n�[�h�E�F�A(HAL)�w��)
										hWnd,								// �E�C���h�E�n���h��
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,// �R�c�v�Z����(�\�t�g�E�F�A�Ŏ��{���w��)
										&m_d3dpp,							// �v���[���e�[�V�����p�����[�^�\���̊i�[�A�h���X�w��
										&m_pd3dDevice))) {						// �`��f�o�C�X�i�[�A�h���X�w��
			MessageBox(hWnd,TEXT("Direct3D�̏������Ɏ��s���܂����BTrue Color(24�ޯ�)����High Color(16�ޯ�/32�ޯ�)�ɕύX���āA���s���Ă��������B"),TEXT("�m�F"),MB_OK | MB_ICONSTOP);
			SAFE_RELEASE(m_pD3D);
			return E_FAIL;
		}
	}

	m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);
	if(m_d3dCaps.MaxVertexBlendMatrixIndex > 1)
		m_byAnimMethod = D3DINDEXED;
	else {
		if(m_d3dCaps.MaxVertexBlendMatrices > 1)
			m_byAnimMethod = D3DNONINDEXED;
		else {
			m_byAnimMethod = SOFTWARE;
			SAFE_RELEASE(m_pd3dDevice);
			// �`��:HAL & 3D�v�Z:SOFTWARE �Ŏ��s
			if(FAILED(m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,					// �f�B�X�v���C�A�_�v�^�[�w��(�W���w��)
											D3DDEVTYPE_HAL,						// �`��f�o�C�X�w��(�n�[�h�E�F�A(HAL)�w��)
											hWnd,								// �E�C���h�E�n���h��
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,// �R�c�v�Z����(�\�t�g�E�F�A�Ŏ��{���w��)
											&m_d3dpp,							// �v���[���e�[�V�����p�����[�^�\���̊i�[�A�h���X�w��
											&m_pd3dDevice))) {						// �`��f�o�C�X�i�[�A�h���X�w��
				MessageBox(hWnd,TEXT("Direct3D�̏������Ɏ��s���܂����B"),TEXT("�m�F"),MB_OK | MB_ICONSTOP);
				SAFE_RELEASE(m_pD3D);
				return E_FAIL;
			}
		}
	}

	return S_OK;
}
// ------------------------------------------------------------------------
//�@�֐���	:RestoreDevice				�f�o�C�X�̕���
//	����		:							�Ȃ�			
//	�߂�l	:							TRUE:�����AFALSE:���s					
// ------------------------------------------------------------------------
BOOL CDXDevice::RestoreDevice()
{
	HRESULT hr;

	if(!m_pD3D || !m_pd3dDevice)
		return FALSE;

	// ���������f�o�C�X�̕�������
	if(m_bDeviceLost) {
		// �f�o�C�X��Ԃ̃`�F�b�N
		hr  = m_pd3dDevice->TestCooperativeLevel();
		if(FAILED(hr)) {
			// �f�o�C�X�́A�������Ă��邪���݃��Z�b�g�ł��Ȃ�
			if(hr == D3DERR_DEVICELOST)
				return FALSE;
			 // ���݃��Z�b�g�ł��Ȃ���
			if(hr != D3DERR_DEVICENOTRESET)
				return FALSE;
			if(!m_CleanupObject) {
				CleanupObject();	// �I�u�W�F�N�g�J��
				m_CleanupObject = TRUE;
			}
			hr = m_pd3dDevice->Reset(&m_d3dpp); // �f�o�C�X�̕���
			if(FAILED(hr))
				// �f�o�C�X�́A�������Ă��邪���݃��Z�b�g�ł��Ȃ�
				if(hr == D3DERR_DEVICELOST)
					return FALSE;
			CreateObject();		// �I�u�W�F�N�g�č쐬
			m_CleanupObject = FALSE;
		}
	}

	return TRUE;
}
// ------------------------------------------------------------------------
//�@�֐���	:ChangeDisplayMode			�\�����[�h�؂�ւ�
//	����		:							�Ȃ�			
//	�߂�l	:							�Ȃ�		
// ------------------------------------------------------------------------
void CDXDevice::ChangeDisplayMode()
{
	CleanupObject();			// �I�u�W�F�N�g�J��

	// �v���[���e�[�V�����p�����[�^�ݒ�
	ZeroMemory(&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS));	// ���������[���N���A
	m_d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;// �t���b�s���O���@:IDirect3DDevice9::Present�������o�b�N�o�b�t�@�̓��e�Ɉ�؉e�����Ȃ�
	m_d3dpp.BackBufferCount		= 1;					// �o�b�N�o�b�t�@�����Q�ɐݒ�
	m_d3dpp.EnableAutoDepthStencil= TRUE;				// �[�x�o�b�t�@���Ǘ���L��
	m_d3dpp.AutoDepthStencilFormat= D3DFMT_D24S8;		// 8�r�b�g�̃X�e���V���`�����l�����w��
	m_d3dpp.hDeviceWindow		= m_hWnd;				// �E�B���h�E�n���h��

	// �\�����[�h�̐؂�ւ�
	if(m_bWindowed) {								// �E�B���h�E���[�h�̏ꍇ
		m_d3dpp.Windowed = TRUE;					// �E�B���h�E���[�h�ɐݒ�
		RECT rcClient;
		::GetClientRect(m_hWnd,&rcClient);
		m_d3dpp.BackBufferWidth		= rcClient.right - rcClient.left;	// �o�b�N�o�b�t�@�̕�
		m_d3dpp.BackBufferHeight	= rcClient.bottom - rcClient.top;	// �o�b�N�o�b�t�@�̍���
		if(m_d3ddm.Format == D3DFMT_X8R8G8B8)
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;	// �F��:ARGB8bit
		else if(m_d3ddm.Format == D3DFMT_R5G6B5)
			m_d3dpp.BackBufferFormat = D3DFMT_A1R5G5B5;	// �F��:A1bitRGB5bit
		else
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;	// �F��:GetAdapterDisplayMode�Ŏ擾����Format���w��

		m_d3dpp.FullScreen_RefreshRateInHz = 0;		// ���t���b�V�����[�g���w��
	}
	else {											// �t���X�N���[�����[�h�̏ꍇ
		m_d3dpp.Windowed = FALSE;						// �t���X�N���[�����[�h�ɐݒ�
		m_d3dpp.BackBufferWidth = m_Mode.Width;			// �o�b�N�o�b�t�@�����𑜓x
		m_d3dpp.BackBufferHeight = m_Mode.Height;		// �o�b�N�o�b�t�@�����𑜓x
		m_d3dpp.FullScreen_RefreshRateInHz = m_Mode.RefreshRate;	// ���t���b�V�����[�g���w��
		m_d3dpp.BackBufferFormat = m_Mode.Format;		// �F�����w��
	}

	m_pd3dDevice->Reset(&m_d3dpp);				// ���Z�b�g
	m_bDeviceLost = TRUE;
	PostMessage(m_hWnd,UM_SETCURSOR,0L,0L);		// �J�[�\���ݒ�
	CreateObject();							// �I�u�W�F�N�g�č쐬
}
// ------------------------------------------------------------------------
//�@�֐���	:Render						�����_�����O����
//	����	:							�Ȃ�			
//	�߂�l	:							�]������
// ------------------------------------------------------------------------
HRESULT CDXDevice::Render()
{
	if(SUCCEEDED(m_pd3dDevice->BeginScene())) {			// �V�[���̊J�n
		//�r���[�|�[�g�܂��̓r���[�|�[�g���̋�`�Z�b�g���w�肵�� RGBA�F�ɃN���A���A�[�x�o�b�t�@���N���A���A�X�e���V�� �o�b�t�@���폜
		m_pd3dDevice->Clear(
			0,												// ��������Z�`���w��(�o�b�N�o�b�t�@�S�̏��������w��)
			NULL,											// ��`�̍��W�w��(�摜�S�̏��������w��)
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	// �����_�����O�^�[�Q�b�g/�[�x�o�b�t�@/�X�e���V���o�b�t�@�̃N���A
			D3DCOLOR_XRGB( 0, 50, 0),						// �����_�����O�^�[�Q�b�g�T�[�t�F�X��h��Ԃ�32�r�b�gRGBA�J���[�l���w��(���F)
			1.0f,											// ���̃��\�b�h���[�x�o�b�t�@�ɕۑ�����V����Z�l
			0);												// �e�X�e���V���o�b�t�@�̃G���g���ɕۑ�����l

		// �R���g���[���L�[�v���X�v�L�[�������ꂽ�ꍇ
		if(((m_byKeyState[DIK_LCONTROL] & 0x80) || (m_byKeyState[DIK_RCONTROL] & 0x80))
			&& (m_byKeyState[DIK_W] & 0x80)) {
			m_bChangeDisplayMode = TRUE;
		}

		// ��������L�q����

		CCamera* pCamera = CCamera::GetInstance();


		if ((m_byKeyState[DIK_LSHIFT] & 0x80) || (m_byKeyState[DIK_RSHIFT] & 0x80))
		{
			D3DXVECTOR3 vViewAngle = pCamera->GetViewAngle();
			if (m_byKeyState[DIK_UP] & 0x80)
			{
				vViewAngle.x += 0.05f;
			}
			if (m_byKeyState[DIK_DOWN] & 0x80)
			{
				vViewAngle.x -= 0.05f;
			}
			if (m_byKeyState[DIK_RIGHT] & 0x80)
			{
				vViewAngle.y -= 0.05f;
			}
			if (m_byKeyState[DIK_LEFT] & 0x80)
			{
				vViewAngle.y += 0.05f;
			}

			pCamera->SetViewAngle(vViewAngle);
		}

		pCamera->SetCamera(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		m_pVertex->Draw();

		m_pd3dDevice->EndScene();							// �V�[�����I������
	}
	// �f�o�C�X�����L����o�b�N�o�b�t�@�̃V�[�P���X�̒��Ŏ��̃R���e���c��񎦂���
	// �o�b�N�o�b�t�@�̓��e���E�C���h�E�ɓ]��
	return m_pd3dDevice->Present(	NULL,						// �o�b�N�o�b�t�@�Z�`�̈���w��(�o�b�N�o�b�t�@�S�̂��t�����g�o�b�t�@�ɓ]���Ŏw��Ȃ̂�NULL)
									NULL,						// �t�����g�o�b�t�@�Z�`�̈���w��(�o�b�N�o�b�t�@�S�̂��t�����g�o�b�t�@�ɓ]���Ŏw��Ȃ̂�NULL)
									NULL,						// �E�B���h�E�n���h��(�E�B���h�E���P��ʂ����g���Ă��Ȃ��̂�NULL���w��)
									NULL);						// DirectX9�ł́A�g�p����Ă��Ȃ��̂�NULL���w��
}