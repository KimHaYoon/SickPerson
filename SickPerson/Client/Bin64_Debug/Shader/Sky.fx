
cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	matrix	g_matWP;
	float3	g_vPivot;
	float	g_fTrEmpty;
	float3	g_vTrLength;
	float	g_fTrEmpty1;
}

struct VS_INPUT_POS
{
	float3	vPos	: POSITION;
};

struct VS_OUTPUT_SKY
{
	float4	vPos	: SV_POSITION;
	float3	vUVW	: POSITION;
};

TextureCube		g_DiffuseCube	: register(t0);
SamplerState	g_DiffuseSampler	: register(s0);

VS_OUTPUT_SKY SkyVS(VS_INPUT_POS input)
{
	VS_OUTPUT_SKY	output = (VS_OUTPUT_SKY)0;

	output.vPos = mul(float4(input.vPos, 1.f), g_matWVP).xyww;
	output.vUVW = input.vPos;

	return output;
}

struct PS_OUTPUT
{
	float4	vColor	: SV_TARGET;
};

PS_OUTPUT SkyPS(VS_OUTPUT_SKY input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = g_DiffuseCube.Sample(g_DiffuseSampler, input.vUVW);

	return output;
}