#ifndef __VERTEX_H__
#define __VERTEX_H__
#pragma once

#include "Global.h"
// ------ �}�N���錾 ------
#define NumVertices	3*3	// ���_�̐�

// ------ �\���̐錾 ------
struct CUSTOMVERTEX 
{
	D3DXVECTOR4 vPosition;	// �`����W
	D3DXVECTOR4 color;		// ���_�F
};

// ------ �N���X�錾 ------
class CVertex
{
private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;				// �`��f�o�C�X
	CUSTOMVERTEX		m_vertices[NumVertices];	// ���_
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;		// ���_�錾
	LPD3DXEFFECT		m_pEffect;					// �V�F�[�_

public:
	CVertex(LPDIRECT3DDEVICE9);	// �R���X�g���N�^
	~CVertex();					// �f�X�g���N�^
	void Draw();				// �`��
	BOOL Initialize();			// ������
	LPD3DXEFFECT GetEffect()	{ return m_pEffect;}
};
#endif