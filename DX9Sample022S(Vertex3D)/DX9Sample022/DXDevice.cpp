// -------------------- IncludeFile -----------------
#include "DXDevice.h"
// コンストラクタ
CDXDevice::CDXDevice()
{
	m_pD3D = nullptr;
	m_pd3dDevice = nullptr;
	m_bDeviceLost = FALSE;
	m_bChangeDisplayMode = FALSE;
	m_pDInput = nullptr;
	m_pDXFont = nullptr;
	m_CleanupObject = FALSE;
	ZeroMemory(m_byKeyState,sizeof(BYTE) * BUFFER_SIZE);	// メモリのゼロクリア

	m_pVertex = nullptr;

}
// デストラクタ
CDXDevice::~CDXDevice()
{
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	SAFE_DELETE(m_pDInput);

}
// ------------------------------------------------------------------------
//	関数	:CreateResource				リソース作成
//			:							デバイス消失に影響されないリソース
//	引数	:							なし
//	戻り値	:							TRUE:成功、FALSE:何かしらの問題発生
// ------------------------------------------------------------------------
BOOL CDXDevice::CreateResource()
{
	srand(timeGetTime());

	CCamera::Create(m_pd3dDevice);
	CDXFont::Create(m_pd3dDevice);

	m_pVertex = new CVertex(m_pd3dDevice);
	if (m_pVertex->Initialize() != TRUE)
	{
		return FALSE;
	}

	return TRUE;
}
// ------------------------------------------------------------------------
//	関数	:CleanupResource			リソース開放
//	引数	:							なし
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDXDevice::CleanupResource()
{
	CCamera::Destroy();
	CDXFont::Destroy();

	SAFE_DELETE( m_pVertex );

	CleanupObject();					// オブジェクト開放
}
// ------------------------------------------------------------------------
//	関数	:CreateObject				オブジェクト作成
//			:							デバイス消失に影響されるリソース
//	引数	:							なし
//	戻り値	:							TRUE:成功、FALSE:何かしらの問題発生
// ------------------------------------------------------------------------
BOOL CDXDevice::CreateObject()
{
	if (CDXFont::GetInstance())
		CDXFont::GetInstance()->GetFont()->OnResetDevice();

	if (m_pVertex)
		m_pVertex->GetEffect()->OnResetDevice();

	return TRUE;
}
// ------------------------------------------------------------------------
//	関数	:CleanupObject				オブジェクト開放
//	引数	:							なし
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDXDevice::CleanupObject()
{
	//delete m_pDXFont;
	if (CDXFont::GetInstance())
		CDXFont::GetInstance()->GetFont()->OnLostDevice();

	if (m_pVertex)
		m_pVertex->GetEffect()->OnLostDevice();

}
// ------------------------------------------------------------------------
//　関数名	:setDeviceLost				デバイス消失の指定
//	引数	:bIsIcon					デバイス消失の有無
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDXDevice::setDeviceLost(BOOL bDeviceLost)
{
	m_bDeviceLost = bDeviceLost;
}
// ------------------------------------------------------------------------
//　関数名	:getDeviceLost				デバイス消失の取得
//	引数		:							なし
//	戻り値	:							デバイス消失の有無
// ------------------------------------------------------------------------
BOOL CDXDevice::getDeviceLost()
{
	return m_bDeviceLost;
}
// ------------------------------------------------------------------------
//　関数名	:getD3DSystem				Direct3Dシステムの取得
//	引数		:							なし
//	戻り値	:							Direct3Dシステム
// ------------------------------------------------------------------------
LPDIRECT3D9 CDXDevice::getD3DSystem()
{
	return m_pD3D;
}
// ------------------------------------------------------------------------
//　関数名	:getDevice					描画デバイスの取得
//	引数		:							なし
//	戻り値	:							描画デバイス
// ------------------------------------------------------------------------
LPDIRECT3DDEVICE9 CDXDevice::getDevice()
{
	return m_pd3dDevice;
}

// ------------------------------------------------------------------------
//　関数名	:getDInput					DirectInputの取得
//	引数		:							なし
//	戻り値	:							DirectInput
// ------------------------------------------------------------------------
CDInput* CDXDevice::getDInput()
{
	return m_pDInput;
}
// ------------------------------------------------------------------------
//　関数名	:setDeviceLost				ウィンドウモードの指定
//	引数		:bWindowed					ウィンドウモード/フルスクリーンモード
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDXDevice::setWindowed(BOOL bWindowed)
{
	m_bWindowed = bWindowed;
}
// ------------------------------------------------------------------------
//　関数名	:getWindowed				ウィンドウモードの取得
//	引数		:							なし
//	戻り値	:							ウィンドウモード/フルスクリーンモード
// ------------------------------------------------------------------------
BOOL CDXDevice::getWindowed()
{
	return m_bWindowed;
}
// ------------------------------------------------------------------------
//　関数名	:setChangeDisplayMode		ディスプレイモード変更の指定
//	引数		:bChangeDisplayMode			ディスプレイモード変更実行の有無
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDXDevice::setChangeDisplayMode(BOOL bChangeDisplayMode)
{
	m_bChangeDisplayMode = bChangeDisplayMode;
}
// ------------------------------------------------------------------------
//　関数名	:getChangeDisplayMode		ディスプレイモード変更の取得
//	引数		:							なし
//	戻り値	:							ディスプレイモード変更実行の有無
// ------------------------------------------------------------------------
BOOL CDXDevice::getChangeDisplayMode()
{
	return m_bChangeDisplayMode;
}
// ------------------------------------------------------------------------
//　関数名	:getKeyStateArray			キー入力情報格納配列アドレスの取得
//	引数		:							なし
//	戻り値	:							ディスプレイモード変更実行の有無
// ------------------------------------------------------------------------
LPBYTE CDXDevice::getKeyStateArray()
{
	return m_byKeyState;
}
// ------------------------------------------------------------------------
//　関数名	:Initialize					デバイスの初期化
//	引数		:hWnd						ウィンドウズハンドル
//			:bWindowed					TRUE:ウィンドウモード、FALSE:フルスクリーンモード
//	戻り値	:							S_OK:成功、E_FAIL:何かしらの問題発生
// ------------------------------------------------------------------------
HRESULT CDXDevice::Initialize(HWND hWnd,BOOL bWindowed)
{
	// Direct3Dシステムの作成
	m_pD3D = ::Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL) 
		return E_FAIL;

	// プレゼントパラメータの初期化
	InitPresentParameters(hWnd,bWindowed);

	// Direct3DDeviceの作成
	HRESULT hResult = CreateDevice(hWnd,m_d3dpp);
	if(FAILED(hResult))
		return E_FAIL;
	// DirectInputの作成
	m_pDInput = new CDInput(hWnd);

	if(!CreateResource()) {
		CleanupResource();
		return E_FAIL;
	}
	if(!CreateObject()) {
		CleanupObject();
		return E_FAIL;
	}

	return S_OK;
}
// ------------------------------------------------------------------------
//　関数名	:InitPresentParameters		プレゼントパラメータの初期化
//	引数		:hWnd						ウィンドウズハンドル
//			:bWindowed					TRUE:ウィンドウモード、FALSE:フルスクリーンモード
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDXDevice::InitPresentParameters(HWND hWnd,BOOL bWindowed)
{
	ZeroMemory(&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS));	// メモリのゼロクリア
	m_d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;// フリッピング方法:IDirect3DDevice9::Present処理がバックバッファの内容に一切影響しない
	m_d3dpp.BackBufferCount		= 1;					// バックバッファ数を２に設定
	m_d3dpp.EnableAutoDepthStencil= TRUE;				// 深度バッファを管理を有効
	m_d3dpp.AutoDepthStencilFormat= D3DFMT_D24S8;		// 8ビットのステンシルチャンネルを指定
	m_d3dpp.hDeviceWindow		= hWnd;					// ウィンドウハンドル
	D3DDISPLAYMODE BaseFullScreenMode = {800,600,60,D3DFMT_R5G6B5};	// 基本モード
	D3DDISPLAYMODE FullScreenMode = {800,600,75,D3DFMT_X8R8G8B8};	// 指定モード
	m_bWindowed = bWindowed;
	m_hWnd = hWnd;

	m_Mode = CheckFullScreenAdapter(FullScreenMode);
	if(m_Mode.Format == D3DFMT_UNKNOWN)
		m_Mode = BaseFullScreenMode;

	//ディスプレイモードを取得
	m_pD3D->GetAdapterDisplayMode(	D3DADAPTER_DEFAULT	// ディスプレイアダプター指定(標準指定) 
									,&m_d3ddm);			// ディスプレイモードを保存する構造体を指定

	if(bWindowed) {		// ウインドウモードに設定
		m_d3dpp.Windowed = TRUE;					// ウインドウモードに設定
		RECT rcClient;
		::GetClientRect(hWnd,&rcClient);
		m_d3dpp.BackBufferWidth		= rcClient.right - rcClient.left;
		m_d3dpp.BackBufferHeight	= rcClient.bottom - rcClient.top;
		if(m_d3ddm.Format == D3DFMT_X8R8G8B8)
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;	// 色数:ARGB8bit
		else if(m_d3ddm.Format == D3DFMT_R5G6B5)
			m_d3dpp.BackBufferFormat = D3DFMT_A1R5G5B5;	// 色数:A1bitRGB5bit
		else
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;	// 色数:GetAdapterDisplayModeで取得したFormatを指定
		m_d3dpp.FullScreen_RefreshRateInHz = 0;			// リフレッシュレートを指定
	}
	else {
		m_d3dpp.Windowed = FALSE;						// フルスクリーンモードに設定
		m_d3dpp.BackBufferWidth = m_Mode.Width;	// バックバッファ水平解像度
		m_d3dpp.BackBufferHeight = m_Mode.Height;// バックバッファ垂直解像度
		m_d3dpp.FullScreen_RefreshRateInHz = m_Mode.RefreshRate;	// リフレッシュレートを指定
		m_d3dpp.BackBufferFormat = m_Mode.Format;	// 色数を指定
	}
}
// ------------------------------------------------------------------------
//　関数名	:CheckFullScreenAdapter		グラフィックアダプタ情報の取得
//	引数		:FullScreenMode				フルスクリーンモードの情報
//	戻り値	:							実行できるフルスクリーンモード
// ------------------------------------------------------------------------
D3DDISPLAYMODE CDXDevice::CheckFullScreenAdapter(D3DDISPLAYMODE FullScreenMode)
{
	D3DADAPTER_IDENTIFIER9 Adapter;
	D3DDISPLAYMODE ResultMode = {0,0,0,D3DFMT_UNKNOWN};
	D3DDISPLAYMODE Mode;
	int iLevel = 1000000;
	int iNumber,iCount,i,j;
	UINT uMaxRefreshRate = 0;
	int iSelectNumber = -1;
	D3DFORMAT AdapterFormat[6] = {D3DFMT_A2R10G10B10,D3DFMT_X8R8G8B8,D3DFMT_A8R8G8B8,D3DFMT_X1R5G5B5,D3DFMT_A1R5G5B5,D3DFMT_R5G6B5};

	iNumber = m_pD3D->GetAdapterCount();		// システムにあるアダプタの数を取得
	if(iNumber != -1){							// システムにアダプタがある場合
		// システムに存在していた物理的なディスプレイアダプタを記述
		m_pD3D->GetAdapterIdentifier(	0,			// ディスプレイアダプタを示す序数を指定(DEFAULTを指定)
										0,			// (通常はゼロに設定するパラメータ)
										&Adapter);	// 指定したアダプタを記述する情報が格納される構造体の
													// ポインタを指定
		for(j = 0;j < 6;j++) {
			iCount = 0;
			// アダプタで利用可能なディスプレイモードの数を取得
			iCount = m_pD3D->GetAdapterModeCount(	D3DADAPTER_DEFAULT,	// ディスプレイアダプタを示す序数(DEFAULTを指定)
													AdapterFormat[j]);	// ディスプレイフォーマット(16Bit)
			for(i = 0; i < iCount;i++) {
				// アダプタのディスプレイモードを列挙
				if(m_pD3D->EnumAdapterModes(0,					// 照会するディスプレイアダプタを示す序数を指定
																// (DEFAULTを指定)
											AdapterFormat[j],	// ディスプレイフォーマット
											i,					// 列挙するモードを示す順序数
											&Mode) == D3D_OK){	// D3DDISPLAYMODE構造体へのポインタを指定
					if(Mode.Format == FullScreenMode.Format) {
						int l = abs((int)(Mode.Width - FullScreenMode.Width)) + abs((int)(Mode.Height - FullScreenMode.Height)) + abs((int)(Mode.RefreshRate - FullScreenMode.RefreshRate));
						if(l < iLevel) {
							memcpy(&ResultMode,&Mode,sizeof(D3DDISPLAYMODE));
							iLevel = l;
						}
					}
				}
			}
		}
	}

	return ResultMode;
}
// ------------------------------------------------------------------------
//	関数1	:CreateDevic				描画デバイスの作成
//	引数		:hWnd						ウィンドウハンドル
//			:d3dpp						プレゼントパラメータ
//	戻り値	:							S_OK:登録に成功,E_FAIL:登録に失敗
// ------------------------------------------------------------------------
HRESULT CDXDevice::CreateDevice(HWND hWnd,D3DPRESENT_PARAMETERS d3dpp)
{
	// デバイスの作成 描画:HAL & 3D計算:HARDWARE で試行
	if(FAILED(m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,						// ディスプレイアダプター指定(標準指定)
									D3DDEVTYPE_HAL,							// 描画デバイス指定(ハードウェア(HAL)指定)
									hWnd,									// ウインドウハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// ３Ｄ計算処理(ハードウェアで実施を指定)
									&m_d3dpp,								// プレゼンテーションパラメータ構造体格納アドレス指定
									&m_pd3dDevice))) {							// 描画デバイス格納アドレス指定
		// 失敗したので 描画:HAL & 3D計算:SOFTWARE で試行
		if(FAILED(m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,					// ディスプレイアダプター指定(標準指定)
										D3DDEVTYPE_HAL,						// 描画デバイス指定(ハードウェア(HAL)指定)
										hWnd,								// ウインドウハンドル
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,// ３Ｄ計算処理(ソフトウェアで実施を指定)
										&m_d3dpp,							// プレゼンテーションパラメータ構造体格納アドレス指定
										&m_pd3dDevice))) {						// 描画デバイス格納アドレス指定
			MessageBox(hWnd,TEXT("Direct3Dの初期化に失敗しました。True Color(24ﾋﾞｯﾄ)からHigh Color(16ﾋﾞｯﾄ/32ﾋﾞｯﾄ)に変更して、実行してください。"),TEXT("確認"),MB_OK | MB_ICONSTOP);
			SAFE_RELEASE(m_pD3D);
			return E_FAIL;
		}
	}

	m_pd3dDevice->GetDeviceCaps(&m_d3dCaps);
	if(m_d3dCaps.MaxVertexBlendMatrixIndex > 1)
		m_byAnimMethod = D3DINDEXED;
	else {
		if(m_d3dCaps.MaxVertexBlendMatrices > 1)
			m_byAnimMethod = D3DNONINDEXED;
		else {
			m_byAnimMethod = SOFTWARE;
			SAFE_RELEASE(m_pd3dDevice);
			// 描画:HAL & 3D計算:SOFTWARE で試行
			if(FAILED(m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT,					// ディスプレイアダプター指定(標準指定)
											D3DDEVTYPE_HAL,						// 描画デバイス指定(ハードウェア(HAL)指定)
											hWnd,								// ウインドウハンドル
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,// ３Ｄ計算処理(ソフトウェアで実施を指定)
											&m_d3dpp,							// プレゼンテーションパラメータ構造体格納アドレス指定
											&m_pd3dDevice))) {						// 描画デバイス格納アドレス指定
				MessageBox(hWnd,TEXT("Direct3Dの初期化に失敗しました。"),TEXT("確認"),MB_OK | MB_ICONSTOP);
				SAFE_RELEASE(m_pD3D);
				return E_FAIL;
			}
		}
	}

	return S_OK;
}
// ------------------------------------------------------------------------
//　関数名	:RestoreDevice				デバイスの復元
//	引数		:							なし			
//	戻り値	:							TRUE:成功、FALSE:失敗					
// ------------------------------------------------------------------------
BOOL CDXDevice::RestoreDevice()
{
	HRESULT hr;

	if(!m_pD3D || !m_pd3dDevice)
		return FALSE;

	// 消失したデバイスの復元処理
	if(m_bDeviceLost) {
		// デバイス状態のチェック
		hr  = m_pd3dDevice->TestCooperativeLevel();
		if(FAILED(hr)) {
			// デバイスは、消失しているが現在リセットできない
			if(hr == D3DERR_DEVICELOST)
				return FALSE;
			 // 現在リセットできないか
			if(hr != D3DERR_DEVICENOTRESET)
				return FALSE;
			if(!m_CleanupObject) {
				CleanupObject();	// オブジェクト開放
				m_CleanupObject = TRUE;
			}
			hr = m_pd3dDevice->Reset(&m_d3dpp); // デバイスの復元
			if(FAILED(hr))
				// デバイスは、消失しているが現在リセットできない
				if(hr == D3DERR_DEVICELOST)
					return FALSE;
			CreateObject();		// オブジェクト再作成
			m_CleanupObject = FALSE;
		}
	}

	return TRUE;
}
// ------------------------------------------------------------------------
//　関数名	:ChangeDisplayMode			表示モード切り替え
//	引数		:							なし			
//	戻り値	:							なし		
// ------------------------------------------------------------------------
void CDXDevice::ChangeDisplayMode()
{
	CleanupObject();			// オブジェクト開放

	// プレゼンテーションパラメータ設定
	ZeroMemory(&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS));	// メモリをゼロクリア
	m_d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;// フリッピング方法:IDirect3DDevice9::Present処理がバックバッファの内容に一切影響しない
	m_d3dpp.BackBufferCount		= 1;					// バックバッファ数を２に設定
	m_d3dpp.EnableAutoDepthStencil= TRUE;				// 深度バッファを管理を有効
	m_d3dpp.AutoDepthStencilFormat= D3DFMT_D24S8;		// 8ビットのステンシルチャンネルを指定
	m_d3dpp.hDeviceWindow		= m_hWnd;				// ウィンドウハンドル

	// 表示モードの切り替え
	if(m_bWindowed) {								// ウィンドウモードの場合
		m_d3dpp.Windowed = TRUE;					// ウィンドウモードに設定
		RECT rcClient;
		::GetClientRect(m_hWnd,&rcClient);
		m_d3dpp.BackBufferWidth		= rcClient.right - rcClient.left;	// バックバッファの幅
		m_d3dpp.BackBufferHeight	= rcClient.bottom - rcClient.top;	// バックバッファの高さ
		if(m_d3ddm.Format == D3DFMT_X8R8G8B8)
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;	// 色数:ARGB8bit
		else if(m_d3ddm.Format == D3DFMT_R5G6B5)
			m_d3dpp.BackBufferFormat = D3DFMT_A1R5G5B5;	// 色数:A1bitRGB5bit
		else
			m_d3dpp.BackBufferFormat = m_d3ddm.Format;	// 色数:GetAdapterDisplayModeで取得したFormatを指定

		m_d3dpp.FullScreen_RefreshRateInHz = 0;		// リフレッシュレートを指定
	}
	else {											// フルスクリーンモードの場合
		m_d3dpp.Windowed = FALSE;						// フルスクリーンモードに設定
		m_d3dpp.BackBufferWidth = m_Mode.Width;			// バックバッファ水平解像度
		m_d3dpp.BackBufferHeight = m_Mode.Height;		// バックバッファ垂直解像度
		m_d3dpp.FullScreen_RefreshRateInHz = m_Mode.RefreshRate;	// リフレッシュレートを指定
		m_d3dpp.BackBufferFormat = m_Mode.Format;		// 色数を指定
	}

	m_pd3dDevice->Reset(&m_d3dpp);				// リセット
	m_bDeviceLost = TRUE;
	PostMessage(m_hWnd,UM_SETCURSOR,0L,0L);		// カーソル設定
	CreateObject();							// オブジェクト再作成
}
// ------------------------------------------------------------------------
//　関数名	:Render						レンダリング処理
//	引数	:							なし			
//	戻り値	:							転送結果
// ------------------------------------------------------------------------
HRESULT CDXDevice::Render()
{
	if(SUCCEEDED(m_pd3dDevice->BeginScene())) {			// シーンの開始
		//ビューポートまたはビューポート内の矩形セットを指定した RGBA色にクリアし、深度バッファをクリアし、ステンシル バッファを削除
		m_pd3dDevice->Clear(
			0,												// 消去する短形数指定(バックバッファ全体書き換え指定)
			NULL,											// 矩形の座標指定(画像全体書き換え指定)
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	// レンダリングターゲット/深度バッファ/ステンシルバッファのクリア
			D3DCOLOR_XRGB( 0, 50, 0),						// レンダリングターゲットサーフェスを塗りつぶす32ビットRGBAカラー値を指定(黒色)
			1.0f,											// このメソッドが深度バッファに保存する新しいZ値
			0);												// 各ステンシルバッファのエントリに保存する値

		// コントロールキープラスＷキーが押された場合
		if(((m_byKeyState[DIK_LCONTROL] & 0x80) || (m_byKeyState[DIK_RCONTROL] & 0x80))
			&& (m_byKeyState[DIK_W] & 0x80)) {
			m_bChangeDisplayMode = TRUE;
		}

		// ここから記述して

		CCamera* pCamera = CCamera::GetInstance();


		if ((m_byKeyState[DIK_LSHIFT] & 0x80) || (m_byKeyState[DIK_RSHIFT] & 0x80))
		{
			D3DXVECTOR3 vViewAngle = pCamera->GetViewAngle();
			if (m_byKeyState[DIK_UP] & 0x80)
			{
				vViewAngle.x += 0.05f;
			}
			if (m_byKeyState[DIK_DOWN] & 0x80)
			{
				vViewAngle.x -= 0.05f;
			}
			if (m_byKeyState[DIK_RIGHT] & 0x80)
			{
				vViewAngle.y -= 0.05f;
			}
			if (m_byKeyState[DIK_LEFT] & 0x80)
			{
				vViewAngle.y += 0.05f;
			}

			pCamera->SetViewAngle(vViewAngle);
		}

		pCamera->SetCamera(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		m_pVertex->Draw();

		m_pd3dDevice->EndScene();							// シーンを終了する
	}
	// デバイスが所有するバックバッファのシーケンスの中で次のコンテンツを提示する
	// バックバッファの内容をウインドウに転送
	return m_pd3dDevice->Present(	NULL,						// バックバッファ短形領域を指定(バックバッファ全体をフロントバッファに転送で指定なのでNULL)
									NULL,						// フロントバッファ短形領域を指定(バックバッファ全体をフロントバッファに転送で指定なのでNULL)
									NULL,						// ウィンドウハンドル(ウィンドウを１画面しか使っていないのでNULLを指定)
									NULL);						// DirectX9では、使用されていないのでNULLを指定
}