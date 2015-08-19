#include "Camera.h"

// 唯一のインスタンス
CCamera* CCamera::m_pInstance = NULL;

// コンストラクタ
CCamera::CCamera(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_vViewAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// デストラクタ
CCamera::~CCamera()
{

}

// 生成
void CCamera::Create(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if ( ! m_pInstance)
		m_pInstance = new CCamera(pd3dDevice);
}

// 破棄
void CCamera::Destroy()
{
	SAFE_DELETE( m_pInstance );
}

// 破棄

// カメラを指定						// 注視点
void CCamera::SetCamera(D3DXVECTOR3 vPosition)
{
	// -------- ビュー変換　-------
	D3DXVECTOR3 vBaseEyePt( 0.0f,  0.0f, -10.0f );	// カメラの基準位置
	D3DXVECTOR3 vEyePt;	// カメラの位置
	D3DXVECTOR3 vLookatPt ( 0.0f, 0.0f,  0.0f );	// カメラの注視点
	D3DXVECTOR3 vUpVec    ( 0.0f, 1.0f,  0.0f );	// カメラの上方
	D3DXMATRIX matRotation;							// 回転

	// --- 回転移動 ---
	D3DXMatrixRotationYawPitchRoll(
		&matRotation,		// 演算結果である行列へのポインタ
		m_vViewAngle.y,		// Y軸を中心とするヨー(ラジアン単位)
		m_vViewAngle.x,		// X軸を中心とするピッチ(ラジアン単位)
		m_vViewAngle.z);	// Z軸を中心とするロール(ラジアン単位)
	D3DXVec3TransformCoord(
		&vEyePt,				// 演算結果であるベクトルへのポインタ
		&vBaseEyePt,			// コピー元になるベクトルへのポインタ
		&matRotation			// 演算元の行列へのポインタ
		);

	// 移動
	vEyePt += vPosition;
	vLookatPt = vPosition;


	D3DXMATRIX matView;					// ビュー行列

	// 左手座標系ビュー行列を作成
	D3DXMatrixLookAtLH(
		&matView,	// 演算結果である行列へのアドレスを指定
		&vEyePt,	// 視点を定義するベクトルへのアドレスを指定
		&vLookatPt,	// 注視点を定義するベクトルへのアドレスを指定
		&vUpVec );	// 上方を定義するベクトルへのアドレスを指定
	m_matView = matView;
	// ----------------------------

	// --------- 射影変換 ---------
	D3DXMATRIX matProjection;	// 射影行列
	// 左手座標系遠近射影行列を作成
	D3DXMatrixPerspectiveFovLH(
		&matProjection,	// 演算結果である行列へのアドレスを指定
		D3DX_PI / 4,	// Y方向の視野(ラジアン単位:45°) 
		800.0f / 600.0f,// アスペクト比(幅/高さ)を指定		// ※デバイスから取得しよう
		1.0f,			// 近いビュー平面のZ値
		2000.0f );		// 遠いビュー平面のZ値

	m_matProjection = matProjection;
}