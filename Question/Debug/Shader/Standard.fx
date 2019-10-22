
#include "Share.fx"

// 함수 인자에 레지스터를 지정해놓는 경우 인자를 안넣어주고 해당 레지스터에서
// 값을 끌어다 사용한다.
VS_OUTPUT_COLOR StandardColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT StandardColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	output.vColor = input.vColor;

	return output;
}

VS_OUTPUT_COLOR_NORMAL StandardColorNormalVS(VS_INPUT_COLOR_NORMAL input)
{
	VS_OUTPUT_COLOR_NORMAL	output = (VS_OUTPUT_COLOR_NORMAL)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	// Normal을 뷰공간으로 만들어준다.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT StandardColorNormalPS(VS_OUTPUT_COLOR_NORMAL input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	_tagLightInfo	tLight = ComputeLight(input.vViewPos, input.vNormal);

	output.vColor = input.vColor * (tLight.vDif + tLight.vAmb) + tLight.vSpc;


	// 조명처리 샘플
	// 조명 방향을 만들어준다.
	/*float3	vLightDir = float3(-1.f, -1.f, 0.f);
	vLightDir = -normalize(vLightDir);

	// 조명방향을 뷰공간으로 변환한다.
	vLightDir = normalize(mul(float4(vLightDir, 0.f), g_matView).xyz);

	float4	vDif = float4(0.f, 1.f, 0.f, 1.f) * g_vMtrlDiffuse *
		max(0, dot(vLightDir, input.vNormal));
	float4	vAmb = float4(0.f, 0.2f, 0.f, 1.f) * g_vMtrlAmbient;

	// 정점에서 카메라를 향하는 벡터를 만들어준다.
	float3	vView = -normalize(input.vViewPos);

	// 반사벡터를 구해준다.
	float3	vR = 2.f * dot(vLightDir, input.vNormal) * input.vNormal - vLightDir;

	float4	vSpc = float4(0.f, 1.f, 0.f, 1.f) * g_vMtrlSpecular *
		pow(max(0, dot(vView, vR)), 3.2f);

	output.vColor = input.vColor * (vDif + vAmb) + vSpc;*/
	//output.vColor = float4(1.f, 1.f, 0.f, 1.f);

	return output;
}

VS_OUTPUT_TEX StandardTexVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);

	if (g_iAniType == 0)
		output.vUV = ComputeUV(input.vUV);

	else
		output.vUV = input.vUV;

	return output;
}

PS_OUTPUT StandardTexPS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

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
