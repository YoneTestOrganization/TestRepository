#ifndef __CAMERA_H__
#define __CAMERA_H__
#pragma once
// ---------- include file ----------
#include "Global.h"

class CCamera
{
private:
	static CCamera*			m_pInstance;			// �J�����C���X�^���X
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// �`��f�o�C�X
	D3DXVECTOR3				m_vViewAngle;			// �J�����A���O��
	D3DXMATRIX				m_matView;				// �r���[�s��
	D3DXMATRIX				m_matProjection;		// �v���W�F�N�V�����s��

public:
	CCamera(LPDIRECT3DDEVICE9);				// �R���X�g���N�^
	~CCamera();								// �f�X�g���N�^

	static void Create(LPDIRECT3DDEVICE9);	// ����
	static void Destroy();					// �j��

	void SetCamera(D3DXVECTOR3 vPosition);	// �J�������w��
	// �A�N�Z�b�T
	D3DXVECTOR3 GetViewAngle()			{return m_vViewAngle;}
	void SetViewAngle(D3DXVECTOR3 Value)
	{
		FLOAT MaxX = 1.0f;
		if ( Value.x <= 0.0f)
			Value.x = 0.0f;
		if ( MaxX <= Value.x)
			Value.x = MaxX;
		m_vViewAngle = Value;
	}
	D3DXMATRIX GetViewMatrix()						{return m_matView;					}
	void SetViewMatrix(D3DXMATRIX Value)			{		m_matView = Value;			}
	D3DXMATRIX GetViewMatrixProjection()			{return m_matProjection;			}
	void SetViewMatrixProjection(D3DXMATRIX Value)	{		m_matProjection = Value;	}
	// �C���X�^���X�̎擾
	static CCamera* GetInstance()			{return m_pInstance;}

};
#endif