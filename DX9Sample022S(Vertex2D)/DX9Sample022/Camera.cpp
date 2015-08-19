#include "Camera.h"

// �B��̃C���X�^���X
CCamera* CCamera::m_pInstance = NULL;

// �R���X�g���N�^
CCamera::CCamera(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// �f�X�g���N�^
CCamera::~CCamera()
{

}

// ����
void CCamera::Create(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if ( ! m_pInstance)
		m_pInstance = new CCamera(pd3dDevice);
}

// �j��
void CCamera::Destroy()
{
	SAFE_DELETE( m_pInstance );
}

// �j��

// �J�������w��						// �����_
void CCamera::SetCamera(D3DXVECTOR3 vPosition)
{
	// -------- �r���[�ϊ��@-------
	D3DXVECTOR3 vBaseEyePt( 0.0f,  0.0f, -10.0f );	// �J�����̊�ʒu
	D3DXVECTOR3 vEyePt;	// �J�����̈ʒu
	D3DXVECTOR3 vLookatPt ( 0.0f, 0.0f,  0.0f );	// �J�����̒����_
	D3DXVECTOR3 vUpVec    ( 0.0f, 1.0f,  0.0f );	// �J�����̏��
	D3DXMATRIX matRotation;							// ��]

	// --- ��]�ړ� ---
	D3DXMatrixRotationYawPitchRoll(
		&matRotation,		// ���Z���ʂł���s��ւ̃|�C���^
		m_vViewAngle.y,		// Y���𒆐S�Ƃ��郈�[(���W�A���P��)
		m_vViewAngle.x,		// X���𒆐S�Ƃ���s�b�`(���W�A���P��)
		m_vViewAngle.z);	// Z���𒆐S�Ƃ��郍�[��(���W�A���P��)
	D3DXVec3TransformCoord(
		&vEyePt,				// ���Z���ʂł���x�N�g���ւ̃|�C���^
		&vBaseEyePt,			// �R�s�[���ɂȂ�x�N�g���ւ̃|�C���^
		&matRotation			// ���Z���̍s��ւ̃|�C���^
		);

	// �ړ�
	vEyePt += vPosition;
	vLookatPt = vPosition;


	D3DXMATRIX matView;					// �r���[�s��

	// ������W�n�r���[�s����쐬
	D3DXMatrixLookAtLH(
		&matView,	// ���Z���ʂł���s��ւ̃A�h���X���w��
		&vEyePt,	// ���_���`����x�N�g���ւ̃A�h���X���w��
		&vLookatPt,	// �����_���`����x�N�g���ւ̃A�h���X���w��
		&vUpVec );	// ������`����x�N�g���ւ̃A�h���X���w��
	m_matView = matView;
	// ----------------------------

	// --------- �ˉe�ϊ� ---------
	D3DXMATRIX matProjection;	// �ˉe�s��
	// ������W�n���ߎˉe�s����쐬
	D3DXMatrixPerspectiveFovLH(
		&matProjection,	// ���Z���ʂł���s��ւ̃A�h���X���w��
		D3DX_PI / 4,	// Y�����̎���(���W�A���P��:45��) 
		800.0f / 600.0f,// �A�X�y�N�g��(��/����)���w��		// ���f�o�C�X����擾���悤
		1.0f,			// �߂��r���[���ʂ�Z�l
		2000.0f );		// �����r���[���ʂ�Z�l

	m_matProjection = matProjection;
}