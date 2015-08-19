#pragma once
// -------------------- Define宣言 ------------------
#define UM_SETCURSOR		WM_USER + 101	// カーソルの設定
#define SAFE_DELETE(p)		{ if(p) { delete (p);		(p)=NULL; } }	// オブジェクト開放マクロ
#define SAFE_DELETE_ARRAY(p){ if(p) { delete [](p);		(p)=NULL; } }	// オブジェクト開放マクロ
#define SAFE_RELEASE(p)		{ if(p) { (p)->Release();	(p)=NULL; } }	// オブジェクト開放マクロ
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

// ------------ 型宣言 -----------
typedef struct _tagColorVertex_
{
	D3DXVECTOR3	vPosition;		// 座標
	D3DXVECTOR3	vNormal;		// 法線
	DWORD		Color;			// 色
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE};

}COLORVERTEX;

typedef struct _tagHitCheckInfo_
{
	FLOAT				Range;		// 半径
	DWORD				NumPoint;	// あたり判定用の頂点の数
	COLORVERTEX*		pPoint;		// あたり判定用の頂点
	COLORVERTEX*		pEdge;		// あたり判定用の辺
	struct _tagHitCheckInfo_()
	{
		pPoint = pEdge = NULL;
	}
}HITCHECKINFO;