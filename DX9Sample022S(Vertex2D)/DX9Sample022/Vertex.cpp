#include "Vertex.h"

// コンストラクタ
CVertex::CVertex(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	m_pVertexDecl = nullptr;
	m_pEffect = nullptr;

}

// デストラクタ
CVertex::~CVertex()
{
	SAFE_RELEASE( m_pVertexDecl );
	SAFE_RELEASE( m_pEffect );
}


// 描画
VOID CVertex::Draw()
{
	if (m_pEffect)
	{
		// テクニックの指定
		m_pEffect->SetTechnique("ShaderTechnique");
		// パスを指定
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);		// パスの番号を指定


		// 頂点フォーマットの指定
		m_pd3dDevice->SetVertexDeclaration(m_pVertexDecl);

		// シェーダ設定をグラボに更新
		m_pEffect->CommitChanges();

		// ユーザメモリポインタでレンダリング
		m_pd3dDevice->DrawPrimitiveUP
			(
			D3DPT_TRIANGLELIST,	// [in] D3DPRIMITIVETYPE 列挙型のメンバ。レンダリングするプリミティブ タイプを記述します。 
			NumVertices / 3,	// レンダリングするプリミティブ数
			m_vertices,			// 頂点データに対するユーザー メモリ ポインタ
			sizeof(CUSTOMVERTEX)// 各頂点のデータのバイト数
			);

		// パスを終了
		m_pEffect->EndPass();
		m_pEffect->End();

	}
}

// 初期化
BOOL CVertex::Initialize()
{
	// シェーダの読み込み
	HRESULT hr;
	LPD3DXBUFFER pErrorMsgs;
	hr = D3DXCreateEffectFromFile(
		m_pd3dDevice,	// デバイス
		"Vertex.fx",		// ファイル名
		NULL,			// プリプロセッサ定義へのポインタ
		NULL,			// オプションのインターフェイスポインタ 
		0,				// コンパイルオプション
		NULL,			// エフェクトプールオブジェクトへのポインタ
		&m_pEffect,		// エフェクトオブジェクトへのポインタ
		&pErrorMsgs);	// エラーおよび警告のリストを含むバッファ
	if (hr != S_OK)
	{
		MessageBox(NULL, (LPCSTR)pErrorMsgs->
			GetBufferPointer(), "ERROR", MB_OK);
		SAFE_RELEASE( pErrorMsgs );
		return FALSE;
	}


	// 頂点の生成
	CUSTOMVERTEX vertices[] =
	{
		{ D3DXVECTOR4(  50.0f,  50.0f, 0.5f, 1.0f ), D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 1.0f )}, // x、y、z、色
		{ D3DXVECTOR4( 350.0f,  50.0f, 0.5f, 1.0f ), D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f )},
		{ D3DXVECTOR4( 200.0f, 300.0f, 0.5f, 1.0f ), D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 1.0f )},
		{ D3DXVECTOR4( 449.0f,  50.0f, 0.5f, 1.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f )},
		{ D3DXVECTOR4( 749.0f,  50.0f, 0.5f, 1.0f ), D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 1.0f )},
		{ D3DXVECTOR4( 599.0f, 300.0f, 0.5f, 1.0f ), D3DXVECTOR4( 0.5f, 0.5f, 0.5f, 1.0f )},
		{ D3DXVECTOR4( 400.0f, 300.0f, 0.5f, 1.0f ), D3DXVECTOR4( 0.0f, 1.0f, 1.0f, 1.0f )},
		{ D3DXVECTOR4( 550.0f, 550.0f, 0.5f, 1.0f ), D3DXVECTOR4( 1.0f, 1.0f, 0.0f, 1.0f )},
		{ D3DXVECTOR4( 250.0f, 550.0f, 0.5f, 1.0f ), D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f )},
	};

//	memcpy(m_vertices, vertices, sizeof(vertices));
	memcpy(m_vertices, vertices, sizeof(CUSTOMVERTEX) * NumVertices);

	// 頂点フォーマットの生成
	// 頂点のフォーマット宣言
	D3DVERTEXELEMENT9 VertexElements[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};
	m_pd3dDevice->CreateVertexDeclaration(VertexElements, &m_pVertexDecl);


	return TRUE;
}