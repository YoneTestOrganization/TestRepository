#pragma once
// -------------------- IncludeFile -----------------
#include "Global.h"
#include "DInput.h"
#include "DXFont.h"
#include "Camera.h"
#include "Vertex.h"
// --------------------- 型宣言 ---------------------
enum METHOD 
{
	D3DNONINDEXED,	// 頂点ブレンディング			(最速)
	D3DINDEXED,		// インデックスブレンディング	(最速)
	SOFTWARE,		// ソフトウェアブレンディング	(低速)
};
// -------------------- クラス宣言 -------------------
class CDXDevice
{
private:
	LPDIRECT3D9				m_pD3D;					// Direct3Dシステム
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// 描画デバイス
	D3DPRESENT_PARAMETERS	m_d3dpp;				// プレゼントパラメータ情報
	D3DDISPLAYMODE			m_d3ddm;				// ディスプレイ情報
	D3DCAPS9				m_d3dCaps;				// DX環境
	BOOL					m_bDeviceLost;			// デバイス消失の有無
	CDInput*				m_pDInput;				// DirectInput
	CDXFont*				m_pDXFont;				// Font
	BOOL					m_CleanupObject;		// ObjetcCleanup実行有無
	BYTE					m_byAnimMethod;			// アニメーション環境
	BOOL					m_bWindowed;			// ウィンドウ/フルスクリーン
	BOOL					m_bChangeDisplayMode;	// ディスプレイモード変更実行の有無
	HWND					m_hWnd;					// ウィンドウハンドル
	D3DDISPLAYMODE			m_Mode;					// モード情報
	BYTE					m_byKeyState[BUFFER_SIZE];	// キー入力格納用配列

	CVertex*				m_pVertex;				// 頂点

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