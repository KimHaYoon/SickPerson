
#include "Share.fx"

cbuffer LandScapeCBuffer	: register(b12)
{
	int		g_iDetailLevel;
	int		g_iSplatCount;
	float2	g_vLandScapeEmpty;
}

Texture2DArray	g_SplatDif	: register(t11);
Texture2DArray	g_SplatNrm	: register(t12);
Texture2DArray	g_SplatSpc	: register(t13);
Texture2DArray	g_AlphaTex	: register(t14);
SamplerState	g_SplatSmp	: register(s11);

VS_OUTPUT_BUMP LandScapeVS(VS_INPUT_BUMP input)
{
	VS_OUTPUT_BUMP	output = (VS_OUTPUT_BUMP)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	// Normal을 뷰공간으로 만들어준다.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(input.vBinormal, 0.f), g_matWV).xyz);
	output.vUV = input.vUV;
	output.iDecal = 1;

	return output;
}

_tagLightInfo ComputeSplatLight(float3 vViewPos, float3 vViewNormal,
	float2 vUV, float2 vAlphaUV)
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
	tInfo.vDif = g_vLightDiffuse * g_vMtrlDiffuse *
		max(0, dot(vViewNormal, vLightDir)) * fLightIntensity;
	tInfo.vAmb = g_vLightAmbient * g_vMtrlAmbient;

	// 정반사광 처리를 하기 위해서는 조명이 해당 정점에 부딪혔을때 반사되는 벡터가 필요하다.
	float3	vR = 2.f * vViewNormal * dot(vViewNormal, vLightDir) - vLightDir;
	float3	vView = -normalize(vViewPos);

	float	fPower = g_vMtrlSpecular.w;
	float4	vMtrlSpc;

	if (g_vMtrlAmbient.w == 1)
	{
		vMtrlSpc = g_SpecularTex.Sample(g_DifSmp, vUV);
		for (int i = 0; i < g_iSplatCount; i++)
		{
			float3	vSplatUV;
			vSplatUV.xy = vUV;
			vSplatUV.z = i;
			float4	vSplatSpecular = g_SplatSpc.Sample(g_SplatSmp, vSplatUV);

			vSplatUV.xy = vAlphaUV;
			float4	vSplatAlpha = g_AlphaTex.Sample(g_SplatSmp, vSplatUV);

			vMtrlSpc = (vMtrlSpc * (float4(1.f, 1.f, 1.f, 1.f) - vSplatAlpha) +
				vSplatSpecular * vSplatAlpha);
		}
	}

	else
	{
		vMtrlSpc = float4(g_vMtrlSpecular.xyz, 1.f);
	}

	tInfo.vSpc = g_vLightSpecular * vMtrlSpc *
		pow(max(0, dot(vR, vView)), fPower) * fLightIntensity;

	//tInfo.vDif.xyz = float3(1.f, 1.f, 1.f);
	//tInfo.vSpc.xyz = float3(0.f, 0.f, 0.f);

	tInfo.vDif.w = 1.f;
	tInfo.vAmb.w = 1.f;
	tInfo.vSpc.w = 1.f;

	return tInfo;
}

PS_OUTPUT LandScapePS(VS_OUTPUT_BUMP input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	float2	vUV = input.vUV * g_iDetailLevel;

	float4	vColor = g_DifTex.Sample(g_DifSmp, vUV);

	if (vColor.a == 0.f)
		clip(-1);

	// Normal을 구해준다.
	float3x3	mat =
	{
		input.vTangent,
		input.vBinormal,
		input.vNormal
	};

	output.vColor4.xyz = input.vTangent * 0.5f + 0.5f;
	output.vColor4.w = 1.f;

	output.vColor5.xyz = input.vBinormal * 0.5f + 0.5f;
	output.vColor5.w = 1.f;

	// Normal 텍스쳐에서 픽셀을 얻어와서 법선벡터로 변환한다.
	// 색상은 0 ~ 1 사이의 값이므로 이 값을 단위벡터의 값인 -1 ~ 1 사이로
	// 변환해주려면 * 2 - 1 을 해주면 된다.
	float3	vBumpNormal	= g_NormalTex.Sample(g_DifSmp, vUV).xyz;
	vBumpNormal = vBumpNormal * 2.f - 1.f;

	for (int i = 0; i < g_iSplatCount; i++)
	{
		float3	vSplatUV;
		vSplatUV.xy = vUV;
		vSplatUV.z = i;
		float4	vSplatColor	= g_SplatDif.Sample(g_SplatSmp, vSplatUV);
		float3	vSplatNormal = g_SplatNrm.Sample(g_SplatSmp, vSplatUV).xyz;
		vSplatNormal = vSplatNormal * 2.f - 1.f;
		vBumpNormal += vSplatNormal;

		vSplatUV.xy = input.vUV;
		float4	vSplatAlpha = g_AlphaTex.Sample(g_SplatSmp, vSplatUV);

		vColor = (vColor * (float4(1.f, 1.f, 1.f, 1.f) - vSplatAlpha) +
			vSplatColor * vSplatAlpha);
	}

	vBumpNormal = normalize(vBumpNormal);

	float3	vViewNormal = normalize(mul(vBumpNormal, mat));

	output.vColor = vColor;
	output.vColor1.xyz = vViewNormal * 0.5f + 0.5f;
	output.vColor1.w = 1.f;
	output.vColor2.x = input.vProjPos.z / input.vProjPos.w;
	output.vColor2.w = input.vProjPos.w;

	// 재질 Diffuse, Ambient 값을 저장한다.
	output.vColor2.y = g_vMtrlDiffuse.x;
	output.vColor2.z = g_vMtrlAmbient.x;

	float4	vMtrlSpc;
	if (g_vMtrlAmbient.w == 1)
	{
		vMtrlSpc = g_SpecularTex.Sample(g_DifSmp, vUV);
		vMtrlSpc.w = g_vMtrlSpecular.w;
		output.vColor4.w = vMtrlSpc.w;
		//vMtrlSpc = float4(g_vMtrlSpecular.xyz, 1.f);
	}

	else
	{
		vMtrlSpc = g_vMtrlSpecular;
		output.vColor4.w = g_vMtrlSpecular.w;
	}
	output.vColor3 = vMtrlSpc;
	/*_tagLightInfo	tLight = ComputeSplatLight(input.vViewPos, vViewNormal, vUV,
		input.vUV);

	output.vColor.xyz = vColor.xyz * (tLight.vDif.xyz + tLight.vAmb.xyz) + tLight.vSpc.xyz;
	output.vColor.w = vColor.w;*/

	//output.vColor *= float4(0.f, 1.f, 0.f, 1.f);

	return output;
}
