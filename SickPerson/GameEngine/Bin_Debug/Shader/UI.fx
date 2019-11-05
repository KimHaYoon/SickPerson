
#include "Share.fx"

struct PS_OUTPUT_SINGLE
{
	float4	vColor	: SV_Target;
};

cbuffer ButtonCBuffer	: register(b11)
{
	float4	g_vUIColor;
	float	g_fLight;
	float3	g_vUIEmpty;
}

cbuffer MultiTexture	: register(b12)
{
	float2	g_vMultiTextureAreaStart;
	float2	g_vMultiTextureAreaEnd;
}

Texture2D		g_MultiTex	: register(t11);
SamplerState	g_MultiSmp	: register(s11);

VS_OUTPUT_TEX UIVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWP);

	if (g_iAniType == 0)
		output.vUV = ComputeUV(input.vUV);

	else
		output.vUV = input.vUV;

	return output;
}

PS_OUTPUT_SINGLE UIPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	if (g_iAniType == 1)
	{
		float3	vUV;
		vUV.xy = input.vUV;
		vUV.z = g_iAniFrameX;
		output.vColor = g_DifArrTex.Sample(g_DifSmp, vUV);
	}

	else
	{
		output.vColor = g_DifTex.Sample(g_DifSmp, input.vUV);
	}

	// clip : 픽셀값을 쓰지 않고 폐기한다.
	// 이경우 깊이버퍼에도 값을 안쓴다.
	if (output.vColor.a == 0.f)
		clip(-1);

	return output;
}

PS_OUTPUT_SINGLE UIColorPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	if (g_iAniType == 1)
	{
		float3	vUV;
		vUV.xy = input.vUV;
		vUV.z = g_iAniFrameX;
		output.vColor = g_DifArrTex.Sample(g_DifSmp, vUV);
	}

	else
	{
		output.vColor = g_DifTex.Sample(g_DifSmp, input.vUV);
	}

	// clip : 픽셀값을 쓰지 않고 폐기한다.
	// 이경우 깊이버퍼에도 값을 안쓴다.
	if (output.vColor.a == 0.f)
		clip(-1);

	output.vColor *= g_vUIColor;

	return output;
}

PS_OUTPUT_SINGLE UIMultiTexturePS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	vColor = (float4)0;

	if (g_iAniType == 1)
	{
		float3	vUV;
		vUV.xy = input.vUV;
		vUV.z = g_iAniFrameX;
		vColor = g_DifArrTex.Sample(g_DifSmp, vUV);
	}

	else
	{
		vColor = g_DifTex.Sample(g_DifSmp, input.vUV);
	}

	// clip : 픽셀값을 쓰지 않고 폐기한다.
	// 이경우 깊이버퍼에도 값을 안쓴다.
	if (vColor.a == 0.f)
		clip(-1);

	output.vColor = vColor;

	if (g_vMultiTextureAreaStart.x <= input.vUV.x && input.vUV.x <= g_vMultiTextureAreaEnd.x &&
		g_vMultiTextureAreaStart.y <= input.vUV.y && input.vUV.y <= g_vMultiTextureAreaEnd.y)
	{
		float2	vSize;
		vSize.x = g_vMultiTextureAreaEnd.x - g_vMultiTextureAreaStart.x;
		vSize.y = g_vMultiTextureAreaEnd.y - g_vMultiTextureAreaStart.y;

		float2	vUV;
		vUV.x = (input.vUV.x - g_vMultiTextureAreaStart.x) / vSize.x;
		vUV.y = (input.vUV.y - g_vMultiTextureAreaStart.y) / vSize.y;

		float4	vMultiColor = g_MultiTex.Sample(g_MultiSmp, vUV);

		output.vColor += vMultiColor;
		/*float	fAlpha = output.vColor.a;
		output.vColor *= (1.f - fAlpha) + vMultiColor * fAlpha;*/
		//output.vColor.a = fAlpha;
	}

	return output;
}