#pragma once
// -------------------- IncludeFile -----------------
#include "Global.h"
// -------------------- �N���X�錾 -------------------
class CAppState  
{
private:
	BOOL					m_bIsIcon;				// �A�C�R�����̗L��
public:
	CAppState();
	~CAppState();
	void setIsIcon(BOOL);
	BOOL getIsIcon();
};
