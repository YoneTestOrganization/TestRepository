#include "Vertex.h"
#include "Camera.h"

// �R���X�g���N�^
CVertex::CVertex(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	m_pVertexDecl = nullptr;
	m_pTexture	= nullptr;
	m_pEffect = nullptr;

}

// �f�X�g���N�^
CVertex::~CVertex()
{
	SAFE_RELEASE( m_pVertexDecl );
	SAFE_RELEASE( m_pEffect );
	SAFE_RELEASE( m_pTexture );
}


// �`��
VOID CVertex::Draw()
{
	// �r���[�s��
	D3DXMATRIX	matView = CCamera::GetInstance()->GetViewMatrix();
	// �v���W�F�N�V�����s��
	D3DXMATRIX	matProjection = CCamera::GetInstance()->GetViewMatrixProjection();

	if (m_pEffect)
	{
		// �e�N�j�b�N�̎w��
		m_pEffect->SetTechnique("ShaderTechnique");
		// �p�X���w��
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);		// �p�X�̔ԍ����w��


		// ���_�t�H�[�}�b�g�̎w��
		m_pd3dDevice->SetVertexDeclaration(m_pVertexDecl);

		// �e�N�X�`����ݒ�
		m_pEffect->SetTexture("Texture", m_pTexture);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity( &matWorld );

		// ���[���h�s��
		m_pEffect->SetMatrix("matWorld", &matWorld );
		// �r���[�s��
		m_pEffect->SetMatrix("matView", &matView );
		// �v���W�F�N�V�����s��
		m_pEffect->SetMatrix("matProjection", &matProjection );


		// �V�F�[�_�ݒ���O���{�ɍX�V
		m_pEffect->CommitChanges();

		// ���[�U�������|�C���^�Ń����_�����O
		m_pd3dDevice->DrawPrimitiveUP
			(
			D3DPT_TRIANGLEFAN,	// [in] D3DPRIMITIVETYPE �񋓌^�̃����o�B�����_�����O����v���~�e�B�u �^�C�v���L�q���܂��B 
			2,	// �����_�����O����v���~�e�B�u�����w��
			m_vertices,			// ���_�f�[�^�ɑ΂��郆�[�U�[ ������ �|�C���^
			sizeof(CUSTOMVERTEX)// �e���_�̃f�[�^�̃o�C�g��
			);

		// �p�X���I��
		m_pEffect->EndPass();
		m_pEffect->End();

	}
}

// ������
BOOL CVertex::Initialize()
{
	// �V�F�[�_�̓ǂݍ���
	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;
	hr = D3DXCreateEffectFromFile(
		m_pd3dDevice,	// �f�o�C�X
		"Vertex.fx",		// �t�@�C����
		NULL,			// �v���v���Z�b�T��`�ւ̃|�C���^
		NULL,			// �I�v�V�����̃C���^�[�t�F�C�X�|�C���^ 
		0,				// �R���p�C���I�v�V����
		NULL,			// �G�t�F�N�g�v�[���I�u�W�F�N�g�ւ̃|�C���^
		&m_pEffect,		// �G�t�F�N�g�I�u�W�F�N�g�ւ̃|�C���^
		&pErrorMsgs);	// �G���[����ьx���̃��X�g���܂ރo�b�t�@
	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE( pErrorMsgs );
		return FALSE;
	}


	// ���_�̐���
	CUSTOMVERTEX vertices[] =
	{
		{ D3DXVECTOR4(  -1.0f,   1.0f, 0.5f, 1.0f ), D3DXVECTOR2(0.0f, 0.0f )}, // x�Ay�Az�A�F
		{ D3DXVECTOR4(   1.0f,   1.0f, 0.5f, 1.0f ), D3DXVECTOR2(1.0f, 0.0f )},
		{ D3DXVECTOR4(   1.0f,  -1.0f, 0.5f, 1.0f ), D3DXVECTOR2(1.0f, 1.0f )},
		{ D3DXVECTOR4(  -1.0f,  -1.0f, 0.5f, 1.0f ), D3DXVECTOR2(0.0f, 1.0f )},
	};

//	memcpy(m_vertices, vertices, sizeof(vertices));
	memcpy(m_vertices, vertices, sizeof(CUSTOMVERTEX) * NumVertices);

	// ���_�t�H�[�}�b�g�̐���
	// ���_�̃t�H�[�}�b�g�錾
	D3DVERTEXELEMENT9 VertexElements[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pd3dDevice->CreateVertexDeclaration(VertexElements, &m_pVertexDecl);

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFileEx(
	m_pd3dDevice,			// �f�o�C�X
	"tex.jpg",				// �t�@�C����
	D3DX_DEFAULT,			// ��
	D3DX_DEFAULT,			// ����
	1,						// �~�b�v���x���̐�
	0,						// ���p���@
	D3DFMT_A8R8G8B8,		// �t�H�[�}�b�g
	D3DPOOL_MANAGED,		// �������[�̔z�u�ꏊ
	D3DX_FILTER_LINEAR,		// �t�B���^�����O������@
	D3DX_DEFAULT,			// �t�B���^�����O������@(�~�b�v�}�b�v)
	0xFF000000,				// �����F���w��
	NULL,					// �摜���ւ̃A�h���X
	NULL,					// �p���b�g
	&m_pTexture				// �e�N�X�`���A�h���X
	);

	return TRUE;
}