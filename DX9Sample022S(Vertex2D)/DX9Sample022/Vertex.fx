


struct VS_INPUT
{
	float4 Position : POSITION;	// 描画座標
	float4 Color	: COLOR0;	// 頂点色
};

struct VS_OUTPUT
{
	float4 Position : POSITION;	// 描画座標
	float4 Color	: COLOR0;	// 頂点色
};

VS_OUTPUT VS_VERTEX(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// 頂点座標を-1～+1内に変更
	Output.Position   = Input.Position;
	Output.Position.x = Output.Position.x / 800.0 * 2.0 - 1.0;
	Output.Position.y = (600.0 - Output.Position.y) / 600.0 * 2.0 - 1.0;
	
	Output.Color = Input.Color;

	return Output;
}

float4 PS_VERTEX(VS_OUTPUT Input) : COLOR0
{
	float4 Color = Input.Color;

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