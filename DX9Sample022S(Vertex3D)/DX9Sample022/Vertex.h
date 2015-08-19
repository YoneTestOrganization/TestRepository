#ifndef __VERTEX_H__
#define __VERTEX_H__
#pragma once

#include "Global.h"
// ------ マクロ宣言 ------
#define NumVertices	4	// 頂点の数

// ------ 構造体宣言 ------
struct CUSTOMVERTEX 
{
	D3DXVECTOR4 vPosition;	// 描画座標
	D3DXVECTOR2 TextureUV;		// UV値
};

// ------ クラス宣言 ------
class CVertex
{
private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;				// 描画デバイス
	CUSTOMVERTEX		m_vertices[NumVertices];	// 頂点
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDecl;		// 頂点宣言
	LPDIRECT3DTEXTURE9	m_pTexture;					// テクスチャ
	LPD3DXEFFECT		m_pEffect;					// シェーダ

public:
	CVertex(LPDIRECT3DDEVICE9);	// コンストラクタ
	~CVertex();					// デストラクタ
	void Draw();				// 描画
	BOOL Initialize();			// 初期化
	LPD3DXEFFECT GetEffect()	{ return m_pEffect;}
};
#endif