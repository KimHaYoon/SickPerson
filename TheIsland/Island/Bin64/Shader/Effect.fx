
#include "Share.fx"

SamplerState	g_DepthSmp	: register(s11);
Texture2D		g_GBufferDepth	: register(t13);
Texture2D		g_GBufferNormal	: register(t14);
Texture2D		g_GBufferTangent	: register(t15);
Texture2D		g_GBufferBinormal	: register(t16);
Texture2D		g_GBufferSpecular	: register(t17);

struct PS_OUTPUT_SINGLE
{
	float4	vColor	: SV_Target;
};

struct PS_OUTPUT_DECAL
{
	float4	vAlbedo	: SV_Target;
	float4	vNormal	: SV_Target1;
	float4	vSpecular	: SV_Target2;
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

struct VS_OUTPUT_DECAL
{
	float4	vPos	: SV_POSITION;
	float4	vProjPos	: POSITION;
};

VS_OUTPUT_DECAL ScreenSpaceDecalVS(VS_INPUT_POS input)
{
	VS_OUTPUT_DECAL	output = (VS_OUTPUT_DECAL)0;

	output.vProjPos = mul(float4(input.vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;

	return output;
}

PS_OUTPUT_DECAL ScreenSpaceDecalPS(VS_OUTPUT_DECAL input)
{
	PS_OUTPUT_DECAL	output = (PS_OUTPUT_DECAL)0;

	// 화면공간에서의 UV좌표를 구한다.
	// 투영공간의 위치 정보를 w로 나누어준다.
	float2	vUV = input.vProjPos.xy / input.vProjPos.w;
	vUV = vUV * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);

	// 현재 화면의 깊이를 구해온다.
	float4	vDepth = g_GBufferDepth.Sample(g_DepthSmp, vUV);
	//float4	vNormal = g_GBufferNormal.Sample(g_DepthSmp, vUV);
	float4	vTangent = g_GBufferTangent.Sample(g_DepthSmp, vUV);
	float4	vBinormal = g_GBufferBinormal.Sample(g_DepthSmp, vUV);
	//float4	vSpecular = g_GBufferSpecular.Sample(g_DepthSmp, vUV);

	if (vBinormal.w == 0.f)
		clip(-1);

	//vNormal.xyz = vNormal.xyz * 2.f - 1.f;
	vTangent.xyz = vTangent.xyz * 2.f - 1.f;
	vBinormal.xyz = vBinormal.xyz * 2.f - 1.f;

	float4	vNormal;
	vNormal.xyz = normalize(cross(vTangent.xyz, vBinormal.xyz));
	vNormal.w = 1.f;

	if (vDepth.w == 0.f)
		clip(-1);

	// 깊이값을 이용해서 위치를 뽑아낸다. 이 육면체의 로컬공간으로 이 위치를 변환해서
	// -0.5 ~ 0.5 사이에 있는지를 판단해야 한다.
	float4	vPos;
	vPos.x = vUV.x * 2.f - 1.f;
	vPos.y = vUV.y * -2.f + 1.f;
	vPos.z = vDepth.r;

	vPos.xyz *= vDepth.w;
	vPos.w = vDepth.w;

	float3	vLocalPos = mul(vPos, g_matInvWVP);

	/*if (abs(vLocalPos.x) > 0.5f)
		clip(-1);

	else if (abs(vLocalPos.y) > 0.5f)
		clip(-1);

	else if (abs(vLocalPos.z) > 0.5f)
		clip(-1);*/
	clip(0.5f - abs(vLocalPos));

	float2 vScreenUV = vLocalPos.xz + float2(0.5f, 0.5f);
	vScreenUV.y = 1.f - vScreenUV.y;

	float4	vAlbedo = g_DifTex.Sample(g_DifSmp, vScreenUV);

	if (vAlbedo.a == 0.f)
		clip(-1);

	output.vAlbedo = vAlbedo;

	// Normal Texture가 있을 경우
	if (g_vMtrlEmissive.w == 1.f)
	{
		// Normal을 얻어온다.
		float3	vDecalNormal = g_NormalTex.Sample(g_DifSmp, vScreenUV).xyz;
		vDecalNormal = vDecalNormal * 2.f - 1.f;

		float3x3	mat =
		{
			vTangent.xyz,
			vBinormal.xyz,
			vNormal.xyz
		};
		
		float3	vViewNormal = normalize(mul(vDecalNormal, mat));
		vViewNormal = vViewNormal * 0.5f + 0.5f;

		output.vNormal.xyz = vViewNormal;
		output.vNormal.w = vNormal.w;
	}

	else
	{
		output.vNormal = vNormal;
	}

	// Specular Texture가 있을 경우
	if (g_vMtrlAmbient.w == 1.f)
	{
		float4	vDecalSpc = g_SpecularTex.Sample(g_DifSmp, vScreenUV);
		vDecalSpc.w = vTangent.w;

		output.vSpecular = vDecalSpc;
	}

	else
	{
		output.vSpecular = float4(0.f, 0.f, 0.f, vTangent.w);
	}

	return output;
}

