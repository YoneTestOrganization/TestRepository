float4x4		matWorld;		// ワールド行列
float4x4		matView;		// ビュー行列
float4x4		matProjection;	// プロジェクション行列
texture			Texture;		// テクスチャ
sampler TextureSampler = 
sampler_state
{
	Texture = <Texture>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VS_INPUT
{
	float4 Position : POSITION;	// 描画座標
	float2 TextureUV: TEXCOORD;	// テクスチャUV値
};

struct VS_OUTPUT
{
	float4 Position : POSITION;	// 描画座標
	float2 TextureUV: TEXCOORD;	// UV値
};

VS_OUTPUT VS_VERTEX(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// 頂点座標の変換 (全画面表示)
	//Output.Position   = Input.Position;

	// 頂点座標の変換
	float4 WorldPosition = mul(Input.Position, matWorld);
	Output.Position   = mul(WorldPosition, matView);
	Output.Position   = mul(Output.Position, matProjection);

	// テクスチャUV値
	Output.TextureUV = Input.TextureUV;
	
	return Output;
}

float4 PS_VERTEX(VS_OUTPUT Input) : COLOR0
{
	float4 Color = tex2D(TextureSampler, Input.TextureUV);

	return Color;
}

technique ShaderTechnique
{
	Pass P0
	{
		VertexShader = compile vs_2_0 VS_VERTEX();
		PixelShader = compile ps_2_0 PS_VERTEX();
	}
}