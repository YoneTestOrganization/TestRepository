#pragma once
// -------------------- Define�錾 ------------------
// -------------------- IncludeFile -----------------
#include "Global.h"
// -------------------- �N���X�錾 -------------------
class CDXFont
{
private:
	static CDXFont* m_pInstance;		// �t�H���g�C���X�^���X
	LPD3DXFONT		m_pd3dFont;		// �t�H���g

public:
	CDXFont(LPDIRECT3DDEVICE9);
	~CDXFont();
	static void Create(LPDIRECT3DDEVICE9);	// ����
	static void Destroy();					// �j��
	void CreateFont(LPDIRECT3DDEVICE9);
	void DrawText(int,int,char*,D3DCOLOR);
	LPD3DXFONT GetFont()					{return m_pd3dFont;}
	// �C���X�^���X�̎擾
	static CDXFont* GetInstance()			{return m_pInstance;}
};