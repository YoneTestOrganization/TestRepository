#pragma once
// -------------------- Define宣言 ------------------
// -------------------- IncludeFile -----------------
#include "Global.h"
// -------------------- クラス宣言 -------------------
class CDXFont
{
private:
	static CDXFont* m_pInstance;		// フォントインスタンス
	LPD3DXFONT		m_pd3dFont;		// フォント

public:
	CDXFont(LPDIRECT3DDEVICE9);
	~CDXFont();
	static void Create(LPDIRECT3DDEVICE9);	// 生成
	static void Destroy();					// 破棄
	void CreateFont(LPDIRECT3DDEVICE9);
	void DrawText(int,int,char*,D3DCOLOR);
	LPD3DXFONT GetFont()					{return m_pd3dFont;}
	// インスタンスの取得
	static CDXFont* GetInstance()			{return m_pInstance;}
};