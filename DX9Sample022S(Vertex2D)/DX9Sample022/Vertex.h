#ifndef __VERTEX_H__
#define __VERTEX_H__
#pragma once

#include "Global.h"
// ------ マクロ宣言 ------
#define NumVertices	3*3	// 頂点の数

// ------ 構造体宣言 ------
struct CUSTOMVERTEX 
{
	D3DXVECTOR4 vPosition;	// 描画座標
	D3DXVECTOR4 color;		// 頂点色
};

// ------ クラス宣言 ------
class CVertex
{
private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;				// 描画デバイス
	CUSTOMVERTEX		m_vertices[NumVertices];	// 頂点
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;		// 頂点宣言
	LPD3DXEFFECT		m_pEffect;					// シェーダ

public:
	CVertex(LPDIRECT3DDEVICE9);	// コンストラクタ
	~CVertex();					// デストラクタ
	void Draw();				// 描画
	BOOL Initialize();			// 初期化
	LPD3DXEFFECT GetEffect()	{ return m_pEffect;}
};
#endif