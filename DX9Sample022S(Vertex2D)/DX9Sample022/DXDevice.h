#pragma once
// -------------------- IncludeFile -----------------
#include "Global.h"
#include "DInput.h"
#include "DXFont.h"
#include "Camera.h"
#include "Vertex.h"
// --------------------- �^�錾 ---------------------
enum METHOD 
{
	D3DNONINDEXED,	// ���_�u�����f�B���O			(�ő�)
	D3DINDEXED,		// �C���f�b�N�X�u�����f�B���O	(�ő�)
	SOFTWARE,		// �\�t�g�E�F�A�u�����f�B���O	(�ᑬ)
};
// -------------------- �N���X�錾 -------------------
class CDXDevice
{
private:
	LPDIRECT3D9				m_pD3D;					// Direct3D�V�X�e��
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// �`��f�o�C�X
	D3DPRESENT_PARAMETERS	m_d3dpp;				// �v���[���g�p�����[�^���
	D3DDISPLAYMODE			m_d3ddm;				// �f�B�X�v���C���
	D3DCAPS9				m_d3dCaps;				// DX��
	BOOL					m_bDeviceLost;			// �f�o�C�X�����̗L��
	CDInput*				m_pDInput;				// DirectInput
	CDXFont*				m_pDXFont;				// Font
	BOOL					m_CleanupObject;		// ObjetcCleanup���s�L��
	BYTE					m_byAnimMethod;			// �A�j���[�V������
	BOOL					m_bWindowed;			// �E�B���h�E/�t���X�N���[��
	BOOL					m_bChangeDisplayMode;	// �f�B�X�v���C���[�h�ύX���s�̗L��
	HWND					m_hWnd;					// �E�B���h�E�n���h��
	D3DDISPLAYMODE			m_Mode;					// ���[�h���
	BYTE					m_byKeyState[BUFFER_SIZE];	// �L�[���͊i�[�p�z��

	CVertex*				m_pVertex;				// ���_

public:
	HRESULT	Initialize(HWND,BOOL);
	BOOL RestoreDevice();
	CDXDevice();
	virtual ~CDXDevice();
	void CleanupResource();
	void setDeviceLost(BOOL);
	BOOL getDeviceLost();
	LPDIRECT3D9 getD3DSystem();
	LPDIRECT3DDEVICE9 getDevice();
	CDInput* getDInput();
	void setWindowed(BOOL);
	BOOL getWindowed();
	void setChangeDisplayMode(BOOL);
	BOOL getChangeDisplayMode();
	LPBYTE getKeyStateArray();
	void ChangeDisplayMode();
	HRESULT Render();
	HWND GetWindowHandle()			{	return m_hWnd;	}
private:
	void InitPresentParameters(HWND,BOOL);
	D3DDISPLAYMODE CheckFullScreenAdapter(D3DDISPLAYMODE);
	HRESULT CreateDevice(HWND,D3DPRESENT_PARAMETERS);
	BOOL CreateResource();
	BOOL CreateObject();
	void CleanupObject();
};