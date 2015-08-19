#ifndef __CAMERA_H__
#define __CAMERA_H__
#pragma once
// ---------- include file ----------
#include "Global.h"

class CCamera
{
private:
	static CCamera*			m_pInstance;			// カメラインスタンス
	LPDIRECT3DDEVICE9		m_pd3dDevice;			// 描画デバイス
	D3DXVECTOR3				m_vViewAngle;			// カメラアングル
	D3DXMATRIX				m_matView;				// ビュー行列
	D3DXMATRIX				m_matProjection;		// プロジェクション行列

public:
	CCamera(LPDIRECT3DDEVICE9);				// コンストラクタ
	~CCamera();								// デストラクタ

	static void Create(LPDIRECT3DDEVICE9);	// 生成
	static void Destroy();					// 破棄

	void SetCamera(D3DXVECTOR3 vPosition);	// カメラを指定
	// アクセッサ
	D3DXVECTOR3 GetViewAngle()			{return m_vViewAngle;}
	void SetViewAngle(D3DXVECTOR3 Value)
	{
		FLOAT MaxX = 1.0f;
		if ( Value.x <= 0.0f)
			Value.x = 0.0f;
		if ( MaxX <= Value.x)
			Value.x = MaxX;
		m_vViewAngle = Value;
	}
	D3DXMATRIX GetViewMatrix()						{return m_matView;					}
	void SetViewMatrix(D3DXMATRIX Value)			{		m_matView = Value;			}
	D3DXMATRIX GetViewMatrixProjection()			{return m_matProjection;			}
	void SetViewMatrixProjection(D3DXMATRIX Value)	{		m_matProjection = Value;	}
	// インスタンスの取得
	static CCamera* GetInstance()			{return m_pInstance;}

};
#endif