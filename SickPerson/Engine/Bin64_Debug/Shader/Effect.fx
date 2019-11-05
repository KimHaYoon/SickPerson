
#include "Share.fx"

SamplerState	g_DepthSmp	: register(s11);
Texture2D		g_GBufferDepth	: register(t13);

struct PS_OUTPUT_SINGLE
{
	float4	vColor	: SV_Target;
};

cbuffer ParticleCBuffer	: register(b11)
{
	float3	g_vCameraAxisX;
	float	g_vParticleEmpty1;
	float3	g_vCameraAxisY;
	float	g_vParticleEmpty2;
	float2	g_vParticleSize;
	float2	g_vParticleEmpty3;
}

struct VS_OUTPUT_PARTICLE_POS
{
	float3	vCenter	: POSITION;
};

struct GS_OUTPUT_PARTICLE
{
	float4	vPos	: SV_POSITION;
	float4	vProjPos	: POSITION;
	float2	vUV		: TEXCOORD;
};

VS_OUTPUT_PARTICLE_POS EffectVS(VS_INPUT_POS input)
{
	VS_OUTPUT_PARTICLE_POS	output = (VS_OUTPUT_PARTICLE_POS)0;

	output.vCenter = mul(float4(input.vPos, 1.f), g_matWorld).xyz;

	return output;
}

[maxvertexcount(4)]
void EffectGS(point VS_OUTPUT_PARTICLE_POS input[1],
	inout TriangleStream<GS_OUTPUT_PARTICLE> TriStream)
{
	float	fHalfX = g_vParticleSize.x * 0.5f;
	float	fHalfY = g_vParticleSize.y * 0.5f;

	float4	vPos[4];

	vPos[0]	= float4(input[0].vCenter - g_vCameraAxisX * fHalfX -
		g_vCameraAxisY * fHalfY, 1.f);
	vPos[1] = float4(input[0].vCenter - g_vCameraAxisX * fHalfX +
		g_vCameraAxisY * fHalfY, 1.f);
	vPos[2] = float4(input[0].vCenter + g_vCameraAxisX * fHalfX -
		g_vCameraAxisY * fHalfY, 1.f);
	vPos[3] = float4(input[0].vCenter + g_vCameraAxisX * fHalfX +
		g_vCameraAxisY * fHalfY, 1.f);
	
	GS_OUTPUT_PARTICLE	output = (GS_OUTPUT_PARTICLE)0;

	float2	vUV[4];
	vUV[0] = float2(0.f, 1.f);
	vUV[1] = float2(0.f, 0.f);
	vUV[2] = float2(1.f, 1.f);
	vUV[3] = float2(1.f, 0.f);

	for (int i = 0; i < 4; ++i)
	{
		output.vProjPos = mul(vPos[i], g_matVP);
		output.vPos = output.vProjPos;
		//output.vPos = vPos[i];
		output.vUV = vUV[i];

		TriStream.Append(output);
	}
}

PS_OUTPUT_SINGLE EffectPS(GS_OUTPUT_PARTICLE input)
{
	PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

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

	// 화면상의 깊이를 구하기 위한 UV 좌표를 구해준다.
	float2	_vUV;
	_vUV.x = (input.vProjPos.x / input.vProjPos.w) * 0.5f + 0.5f;
	_vUV.y = (-input.vProjPos.y / input.vProjPos.w) * 0.5f + 0.5f;

	// 현재 화면의 깊이를 구해온다.
	float4	vDepth	= g_GBufferDepth.Sample(g_DepthSmp, _vUV);

	// 현재 출력할 이펙트의 깊이를 구한다.
	float	fEffectDepth = input.vProjPos.z / input.vProjPos.w;
	//float	fViewDepth = input.vProjPos.w - vDepth.w;
	float	fDepthGap = (vDepth.x - fEffectDepth) * 5000.f;

	if (fDepthGap >= 0.f)
	{
		float	fAlpha = (fDepthGap / 10.f);

		if (fAlpha > 1.f)
			fAlpha = 1.f;
		output.vColor.a *= fAlpha;
	}

	return output;
}