float4x4		matWorld;		// ���[���h�s��
float4x4		matView;		// �r���[�s��
float4x4		matProjection;	// �v���W�F�N�V�����s��
texture			Texture;		// �e�N�X�`��
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
	float4 Position : POSITION;	// �`����W
	float2 TextureUV: TEXCOORD;	// �e�N�X�`��UV�l
};

struct VS_OUTPUT
{
	float4 Position : POSITION;	// �`����W
	float2 TextureUV: TEXCOORD;	// UV�l
};

VS_OUTPUT VS_VERTEX(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	// ���_���W�̕ϊ� (�S��ʕ\��)
	//Output.Position   = Input.Position;

	// ���_���W�̕ϊ�
	float4 WorldPosition = mul(Input.Position, matWorld);
	Output.Position   = mul(WorldPosition, matView);
	Output.Position   = mul(Output.Position, matProjection);

	// �e�N�X�`��UV�l
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