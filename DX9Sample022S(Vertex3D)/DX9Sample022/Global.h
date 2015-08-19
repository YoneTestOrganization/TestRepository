#pragma once
// -------------------- Define�錾 ------------------
#define UM_SETCURSOR		WM_USER + 101	// �J�[�\���̐ݒ�
#define SAFE_DELETE(p)		{ if(p) { delete (p);		(p)=NULL; } }	// �I�u�W�F�N�g�J���}�N��
#define SAFE_DELETE_ARRAY(p){ if(p) { delete [](p);		(p)=NULL; } }	// �I�u�W�F�N�g�J���}�N��
#define SAFE_RELEASE(p)		{ if(p) { (p)->Release();	(p)=NULL; } }	// �I�u�W�F�N�g�J���}�N��
#define ZERO_CHECK (1e-5f)
// -------------------- IncludeFile -----------------
#include <windows.h>
#include <Mmsystem.h>
#include <commctrl.h>
#include <d3d9.h>
#include <d3dx9.h>
// -------------------- LibraryFile -----------------
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")

// -------------------- GrobalProgram --------------------
inline BOOL NearZero(FLOAT Value)
{
	if (-ZERO_CHECK < Value && Value < ZERO_CHECK)
	{
		return TRUE;
	}

	return FALSE;
}

// ------------ �^�錾 -----------
typedef struct _tagColorVertex_
{
	D3DXVECTOR3	vPosition;		// ���W
	D3DXVECTOR3	vNormal;		// �@��
	DWORD		Color;			// �F
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE};

}COLORVERTEX;

typedef struct _tagHitCheckInfo_
{
	FLOAT				Range;		// ���a
	DWORD				NumPoint;	// �����蔻��p�̒��_�̐�
	COLORVERTEX*		pPoint;		// �����蔻��p�̒��_
	COLORVERTEX*		pEdge;		// �����蔻��p�̕�
	struct _tagHitCheckInfo_()
	{
		pPoint = pEdge = NULL;
	}
}HITCHECKINFO;