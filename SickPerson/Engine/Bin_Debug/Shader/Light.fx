
#include "Share.fx"

SamplerState	g_LightSmp	: register(s11);
Texture2D		g_GBufferAlbedo	: register(t11);
Texture2D		g_GBufferNrm	: register(t12);
Texture2D		g_GBufferDepth	: register(t13);
Texture2D		g_GBufferSpc	: register(t14);
Texture2D		g_LightDif	: register(t15);
Texture2D		g_LightSpc	: register(t16);
Texture2D		g_Deferred	: register(t17);

// HLSL에서 전역변수에 static을 붙여주면 해당 변수는 외부에서 값을
// 설정할 수 없다.
static float3	vPos[4]	=
{
	float3(-1.f, 1.f, 0.f),
	float3(1.f, 1.f, 0.f),
	float3(-1.f, -1.f, 0.f),
	float3(1.f, -1.f, 0.f)
};

static float2	vUV[4]	=
{
	float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};

struct PS_OUTPUT_LIGHTACC
{
	float4	vDif	: SV_Target0;
	float4	vSpc	: SV_Target1;
};

_tagLightInfo ComputeLightAcc(float3 vViewPos, float3 vViewNormal,
	float2 vUV, float4 vMtrlDif, float4 vMtrlAmb)
{
	_tagLightInfo	tInfo = (_tagLightInfo)0;

	// Vertex Shader에서 ViewPos, Normal을 View공간으로 변환해서 여기로 가지고왔다.
	// 조명의 정보들도 뷰공간으로 변환해 주어야 한다.
	float3	vLightDir = (float3)0;
	float	fLightIntensity = 1.f;

	// 방향성 조명일 경우
	if (g_iLightType == 0)
	{
		// 월드공간의 조명 방향을 뷰 공간으로 변환해준다.
		vLightDir = mul(float4(g_vLightDir, 0.f), g_matView).xyz;

		// 변환된 뷰 공간의 조명 방향을 단위벡터로 만들어주고 조명의 역방향을 구한다.
		// 조명의 역방향을 구해야 내적을 통해서 조명의 강도를 구할 수 있기 때문이다.
		vLightDir = -normalize(vLightDir);
	}

	// 점조명일 경우
	if (g_iLightType == 1)
	{
		// 조명 위치를 뷰공간으로 변환해준다.
		float3	vLightPos = mul(float4(g_vLightPos, 1.f), g_matView).xyz;

		// 조명 방향을 구해준다.
		vLightDir = vLightPos - vViewPos;
		vLightDir = normalize(vLightDir);

		// 조명의 감쇠 처리를 위한 정점과 조명의 거리를 구해준다.
		float	fDist = distance(vLightPos, vViewPos);

		if (fDist > g_fLightRange)
			fLightIntensity = 0.f;

		else if (fDist > g_fLightIntensityPercent * g_fLightRange)
		{
			// 감쇠를 처리할 나머지 거리를 구한다.
			float	fLightRange = g_fLightRange * (1.f - g_fLightIntensityPercent);
			fDist -= g_fLightRange * g_fLightIntensityPercent;

			fLightIntensity = 1.f - fDist / fLightRange;
			//fLightIntensity = 1.f / fDist;
			//fLightIntensity = 1.f / fDist;
		}

		//fLightIntensity = 1.f;
	}

	// Spot 조명일 경우
	if (g_iLightType == 2)
	{
		// Spot 조명이 바라보는 방향을 뷰공간으로 변환해준다.
		// 월드공간의 조명 방향을 뷰 공간으로 변환해준다.
		float3 vLightView = mul(float4(g_vLightDir, 0.f), g_matView).xyz;

		// 변환된 뷰 공간의 조명 방향을 단위벡터로 만들어주고 조명의 역방향을 구한다.
		// 조명의 역방향을 구해야 내적을 통해서 조명의 강도를 구할 수 있기 때문이다.
		vLightView = -normalize(vLightView);

		// 광원에서 정점을 바라보는 방향을 구해준다.
		// 조명 위치를 뷰공간으로 변환해준다.
		float3	vLightPos = mul(float4(g_vLightPos, 1.f), g_matView).xyz;

		// 조명 방향을 구해준다.
		vLightDir = vLightPos - vViewPos;
		vLightDir = normalize(vLightDir);

		// 조명과 정점과의 거리를 구해준다.
		float	fDist = distance(vLightPos, vViewPos);

		if (fDist < g_fLightRange)
		{
			// 조명에서 정점을 바라보는 방향을 구해준다.
			float3	vViewDir = -vLightDir;

			// 조명에서 정점을 바라보는 방향과 조명의 방향을 이용해서 각도를 구해준다.
			// cos 세타값을 이용해서 비교한다.
			float	fAngle = dot(vViewDir, vLightView);

			float	fInAngle = cos(g_fLightInAngle * 0.5f);

			// 안쪽 각도의 절반각도보다 작다면 범위 바깥이란 의미이다.
			if (fAngle < fInAngle)
			{
				// 바깥쪽과의 연산
				float	fOutAngle = cos(g_fLightOutAngle * 0.5f);

				// 바깥쪽 각도보다도 크므로 Intensity를 0으로 처리한다.
				if (fAngle < fOutAngle)
					fLightIntensity = 0.f;

				// 바깥쪽 각도 안에 들어왔을 경우
				else
				{
					fLightIntensity = 0.5f;
				}
			}

			// 안쪽 각도 안에 들어왔을 경우
			else
			{
				fLightIntensity = 1.f;
			}
		}

		// 조명 범위 바깥이라면 0으로 처리
		else
			fLightIntensity = 0.f;
	}
	//fLightIntensity = 1.f;
	//vViewNormal = vLightDir;
	//vLightDir = vViewNormal;

	// 위에서 구해준 뷰 공간의 조명의 역방향과 정점의 뷰공간의 노말을 내적해서 cos 세타를 구해
	// 준다. 이 값을 이용해서 조명의 강도로 처리를 하는데 이 값이 - 값이 나오게 되면 조명이
	// 뒤에서 비춘다는 의미이다. 그래서 음수가 나온다면 0으로 처리해준다.
	// 이 조명강도를 이용해서 Diffuse 에 적용해준다.
	// Diffuse는 조명의 Diffuse * 재질의 Diffuse * 조명강도가 된다.
	tInfo.vDif = g_vLightDiffuse * vMtrlDif *
		max(0, dot(vViewNormal, vLightDir)) * fLightIntensity;
	tInfo.vAmb = g_vLightAmbient * vMtrlAmb * fLightIntensity;

	// 정반사광 처리를 하기 위해서는 조명이 해당 정점에 부딪혔을때 반사되는 벡터가 필요하다.
	float3	vR = 2.f * vViewNormal * dot(vViewNormal, vLightDir) - vLightDir;
	float3	vView = -normalize(vViewPos);

	//float	fPower = g_vMtrlSpecular.w;
	float4	vMtrlSpc = g_GBufferSpc.Sample(g_LightSmp, vUV);

	tInfo.vSpc = g_vLightSpecular * vMtrlSpc *
		pow(max(0, dot(vR, vView)), vMtrlSpc.w) * fLightIntensity;

	//tInfo.vDif.xyz = float3(1.f, 1.f, 1.f);
	//tInfo.vSpc.xyz = float3(0.f, 0.f, 0.f);

	tInfo.vDif.w = 1.f;
	tInfo.vAmb.w = 1.f;
	tInfo.vSpc.w = 1.f;

	return tInfo;
}

VS_OUTPUT_TEX LightVS(uint iIndex : SV_VertexID)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	output.vPos = float4(vPos[iIndex], 1.f);
	output.vUV = vUV[iIndex];

	return output;
}

PS_OUTPUT_LIGHTACC LightAccPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_LIGHTACC	output = (PS_OUTPUT_LIGHTACC)0;

	// 깊이 텍스쳐에서 색상을 얻어온다.
	float4	vDepthTex = g_GBufferDepth.Sample(g_LightSmp, input.vUV);

	if (vDepthTex.w == 0.f)
		clip(-1);

	float4	vProjPos;
	vProjPos.x = (input.vUV.x * 2.f - 1.f) * vDepthTex.w;
	vProjPos.y = (input.vUV.y * -2.f + 1.f) * vDepthTex.w;
	vProjPos.z = vDepthTex.x * vDepthTex.w;
	vProjPos.w = vDepthTex.w;

	// 뷰공간에서의 위치를 구한다.
	float3	vViewPos = mul(vProjPos, g_matInvProj).xyz;

	// Normal Target에서 색상을 얻어온다.
	float4	vNormalTex = g_GBufferNrm.Sample(g_LightSmp, input.vUV);

	float3	vViewNormal = vNormalTex.xyz * 2.f - 1.f;

	float4	vMtrlDif = (float4)vDepthTex.y;
	float4	vMtrlAmb = (float4)vDepthTex.z;

	_tagLightInfo	tLightInfo = ComputeLightAcc(vViewPos, vViewNormal,
		input.vUV, vMtrlDif, vMtrlAmb);

	output.vDif = tLightInfo.vDif + tLightInfo.vAmb;
	output.vSpc = tLightInfo.vSpc;

	return output;
}

struct PS_OUTPUT_SINGLE
{
	float4	vColor	: SV_Target;
};

PS_OUTPUT_SINGLE LightBlendPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	vAlbedo = g_GBufferAlbedo.Sample(g_LightSmp, input.vUV);
	float4	vLightDif = g_LightDif.Sample(g_LightSmp, input.vUV);
	float4	vLightSpc = g_LightSpc.Sample(g_LightSmp, input.vUV);

	if (vAlbedo.a == 0.f)
		clip(-1);

	output.vColor = vAlbedo * vLightDif + vLightSpc;
	output.vColor.a = vAlbedo.a;

	return output;
}

PS_OUTPUT_SINGLE DeferredPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	vColor = g_Deferred.Sample(g_LightSmp, input.vUV);

	if (vColor.a == 0.f)
		clip(-1);

	output.vColor = vColor;

	return output;
}
