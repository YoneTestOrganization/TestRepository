// -------------------- IncludeFile -----------------
#include "DXFont.h"
#include "Global.h"

// �B��̃C���X�^���X
CDXFont* CDXFont::m_pInstance = NULL;		// �t�H���g�C���X�^���X

// �R���X�g���N�^
CDXFont::CDXFont(LPDIRECT3DDEVICE9 pd3dDevice)
{
	CreateFont(pd3dDevice);
}
// �f�X�g���N�^
CDXFont::~CDXFont()
{

	//SAFE_RELEASE(m_pd3dFont);
}

// ����
void CDXFont::Create(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if ( ! m_pInstance)
		m_pInstance = new CDXFont(pd3dDevice);
}

// �j��
void CDXFont::Destroy()
{
	SAFE_DELETE( m_pInstance );
}


// ------------------------------------------------------------------------
//	�֐�	:CreateFont					�t�H���g�쐬
//	����	:pd3dDevice					�f�o�C�X
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDXFont::CreateFont(LPDIRECT3DDEVICE9 pd3dDevice)
{
	D3DXFONT_DESC Font;		// �t�H���g�X�^�C���\����

	ZeroMemory(&Font,sizeof(Font));				// �\���̂�������
	Font.Height = 16;							// �����̍���
	Font.Width = 12;							// �����̕�
	Font.Weight = FW_NORMAL;					// �E�F�C�g�l
	Font.MipLevels = D3DX_DEFAULT;				// �~�b�v�}�b�v���x��
	Font.Italic = FALSE;						// �Α�(���Ȃ�)
	Font.CharSet = SHIFTJIS_CHARSET;			// �����Z�b�g
	Font.OutputPrecision = OUT_DEFAULT_PRECIS;	// �o�͐��x
	Font.Quality = PROOF_QUALITY;				// �o�͕i��
	Font.PitchAndFamily = VARIABLE_PITCH;		// �s�b�`�ƃt�@�~��
	::lstrcpy(Font.FaceName,"�l�r ����");		// �t�H���g��

	D3DXCreateFontIndirect(	pd3dDevice,			// �`��f�o�C�X���w��
							&Font,				// �t�H���g�X�^�C�����L�q���Ă���\���̂ւ̃A�h���X���w��
							&m_pd3dFont);		// ���ۂɍ쐬����t�H���g�I�u�W�F�N�g�̃A�h���X���w��
}
// ------------------------------------------------------------------------
//	�֐�	:DrawText					�����`��
//	����	:x							X���W
//			:y							Y���W
//			:pText						������̐擪�A�h���X
//			:Color						�F
//	�߂�l	:							�Ȃ�
// ------------------------------------------------------------------------
void CDXFont::DrawText(int x,int y,char* pText,D3DCOLOR Color)
{
	RECT rc;	// ��`
	rc.left		= x;
	rc.top		= y;
	rc.right	= CW_USEDEFAULT;
	rc.bottom	= CW_USEDEFAULT;

	// �e�L�X�g����`�ɕ`��
	m_pd3dFont->DrawTextA(	NULL,				// �X�v���C�g�ւ̃A�h���X(�ݒ�Ȃ�)
							pText,				// ������̐擪�A�h���X
							-1,					// �`�悷�镶�����w��(�����v�Z������)
							&rc,				// �`��ʒu���̓������\���̂̃A�h���X
							DT_LEFT	| DT_NOCLIP,// �\���`�����w��(����,�N���b�s���O���Ȃ�)
							Color);				// �e�L�X�g�̐F���w��
}
