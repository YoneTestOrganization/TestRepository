#pragma once
// -------------------- IncludeFile -----------------
#include "Global.h"
// -------------------- クラス宣言 -------------------
class CAppState  
{
private:
	BOOL					m_bIsIcon;				// アイコン化の有無
public:
	CAppState();
	~CAppState();
	void setIsIcon(BOOL);
	BOOL getIsIcon();
};
