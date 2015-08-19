// -------------------- IncludeFile -----------------
#include "DXFont.h"
#include "Global.h"

// 唯一のインスタンス
CDXFont* CDXFont::m_pInstance = NULL;		// フォントインスタンス

// コンストラクタ
CDXFont::CDXFont(LPDIRECT3DDEVICE9 pd3dDevice)
{
	CreateFont(pd3dDevice);
}
// デストラクタ
CDXFont::~CDXFont()
{

	//SAFE_RELEASE(m_pd3dFont);
}

// 生成
void CDXFont::Create(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if ( ! m_pInstance)
		m_pInstance = new CDXFont(pd3dDevice);
}

// 破棄
void CDXFont::Destroy()
{
	SAFE_DELETE( m_pInstance );
}


// ------------------------------------------------------------------------
//	関数	:CreateFont					フォント作成
//	引数	:pd3dDevice					デバイス
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDXFont::CreateFont(LPDIRECT3DDEVICE9 pd3dDevice)
{
	D3DXFONT_DESC Font;		// フォントスタイル構造体

	ZeroMemory(&Font,sizeof(Font));				// 構造体を初期化
	Font.Height = 16;							// 文字の高さ
	Font.Width = 12;							// 文字の幅
	Font.Weight = FW_NORMAL;					// ウェイト値
	Font.MipLevels = D3DX_DEFAULT;				// ミップマップレベル
	Font.Italic = FALSE;						// 斜体(しない)
	Font.CharSet = SHIFTJIS_CHARSET;			// 文字セット
	Font.OutputPrecision = OUT_DEFAULT_PRECIS;	// 出力精度
	Font.Quality = PROOF_QUALITY;				// 出力品質
	Font.PitchAndFamily = VARIABLE_PITCH;		// ピッチとファミリ
	::lstrcpy(Font.FaceName,"ＭＳ 明朝");		// フォント名

	D3DXCreateFontIndirect(	pd3dDevice,			// 描画デバイスを指定
							&Font,				// フォントスタイルを記述している構造体へのアドレスを指定
							&m_pd3dFont);		// 実際に作成するフォントオブジェクトのアドレスを指定
}
// ------------------------------------------------------------------------
//	関数	:DrawText					文字描画
//	引数	:x							X座標
//			:y							Y座標
//			:pText						文字列の先頭アドレス
//			:Color						色
//	戻り値	:							なし
// ------------------------------------------------------------------------
void CDXFont::DrawText(int x,int y,char* pText,D3DCOLOR Color)
{
	RECT rc;	// 矩形
	rc.left		= x;
	rc.top		= y;
	rc.right	= CW_USEDEFAULT;
	rc.bottom	= CW_USEDEFAULT;

	// テキストを矩形に描画
	m_pd3dFont->DrawTextA(	NULL,				// スプライトへのアドレス(設定なし)
							pText,				// 文字列の先頭アドレス
							-1,					// 描画する文字数指定(自動計算ささる)
							&rc,				// 描画位置情報の入った構造体のアドレス
							DT_LEFT	| DT_NOCLIP,// 表示形式を指定(左寄せ,クリッピングしない)
							Color);				// テキストの色を指定
}
