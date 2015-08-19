#include "Vertex.h"
#include "Camera.h"

// コンストラクタ
CVertex::CVertex(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;

	m_pVertexDecl = nullptr;
	m_pTexture	= nullptr;
	m_pEffect = nullptr;

}

// デストラクタ
CVertex::~CVertex()
{
	SAFE_RELEASE( m_pVertexDecl );
	SAFE_RELEASE( m_pEffect );
	SAFE_RELEASE( m_pTexture );
}


// 描画
VOID CVertex::Draw()
{
	// ビュー行列
	D3DXMATRIX	matView = CCamera::GetInstance()->GetViewMatrix();
	// プロジェクション行列
	D3DXMATRIX	matProjection = CCamera::GetInstance()->GetViewMatrixProjection();

	if (m_pEffect)
	{
		// テクニックの指定
		m_pEffect->SetTechnique("ShaderTechnique");
		// パスを指定
		m_pEffect->Begin(0, 0);
		m_pEffect->BeginPass(0);		// パスの番号を指定


		// 頂点フォーマットの指定
		m_pd3dDevice->SetVertexDeclaration(m_pVertexDecl);

		// テクスチャを設定
		m_pEffect->SetTexture("Texture", m_pTexture);

		D3DXMATRIX matWorld;
		D3DXMatrixIdentity( &matWorld );

		// ワールド行列
		m_pEffect->SetMatrix("matWorld", &matWorld );
		// ビュー行列
		m_pEffect->SetMatrix("matView", &matView );
		// プロジェクション行列
		m_pEffect->SetMatrix("matProjection", &matProjection );


		// シェーダ設定をグラボに更新
		m_pEffect->CommitChanges();

		// ユーザメモリポインタでレンダリング
		m_pd3dDevice->DrawPrimitiveUP
			(
			D3DPT_TRIANGLEFAN,	// [in] D3DPRIMITIVETYPE 列挙型のメンバ。レンダリングするプリミティブ タイプを記述します。 
			2,	// レンダリングするプリミティブ数を指定
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
		{ D3DXVECTOR4(  -1.0f,   1.0f, 0.5f, 1.0f ), D3DXVECTOR2(0.0f, 0.0f )}, // x、y、z、色
		{ D3DXVECTOR4(   1.0f,   1.0f, 0.5f, 1.0f ), D3DXVECTOR2(1.0f, 0.0f )},
		{ D3DXVECTOR4(   1.0f,  -1.0f, 0.5f, 1.0f ), D3DXVECTOR2(1.0f, 1.0f )},
		{ D3DXVECTOR4(  -1.0f,  -1.0f, 0.5f, 1.0f ), D3DXVECTOR2(0.0f, 1.0f )},
	};

//	memcpy(m_vertices, vertices, sizeof(vertices));
	memcpy(m_vertices, vertices, sizeof(CUSTOMVERTEX) * NumVertices);

	// 頂点フォーマットの生成
	// 頂点のフォーマット宣言
	D3DVERTEXELEMENT9 VertexElements[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	m_pd3dDevice->CreateVertexDeclaration(VertexElements, &m_pVertexDecl);

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFileEx(
	m_pd3dDevice,			// デバイス
	"tex.jpg",				// ファイル名
	D3DX_DEFAULT,			// 幅
	D3DX_DEFAULT,			// 高さ
	1,						// ミップレベルの数
	0,						// 利用方法
	D3DFMT_A8R8G8B8,		// フォーマット
	D3DPOOL_MANAGED,		// メモリーの配置場所
	D3DX_FILTER_LINEAR,		// フィルタリングする方法
	D3DX_DEFAULT,			// フィルタリングする方法(ミップマップ)
	0xFF000000,				// 透明色を指定
	NULL,					// 画像情報へのアドレス
	NULL,					// パレット
	&m_pTexture				// テクスチャアドレス
	);

	return TRUE;
}