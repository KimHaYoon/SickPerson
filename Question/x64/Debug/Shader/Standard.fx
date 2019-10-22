
#include "Share.fx"

// �Լ� ���ڿ� �������͸� �����س��� ��� ���ڸ� �ȳ־��ְ� �ش� �������Ϳ���
// ���� ����� ����Ѵ�.
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
	// Normal�� ��������� ������ش�.
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


	// ����ó�� ����
	// ���� ������ ������ش�.
	/*float3	vLightDir = float3(-1.f, -1.f, 0.f);
	vLightDir = -normalize(vLightDir);

	// ��������� ��������� ��ȯ�Ѵ�.
	vLightDir = normalize(mul(float4(vLightDir, 0.f), g_matView).xyz);

	float4	vDif = float4(0.f, 1.f, 0.f, 1.f) * g_vMtrlDiffuse *
		max(0, dot(vLightDir, input.vNormal));
	float4	vAmb = float4(0.f, 0.2f, 0.f, 1.f) * g_vMtrlAmbient;

	// �������� ī�޶� ���ϴ� ���͸� ������ش�.
	float3	vView = -normalize(input.vViewPos);

	// �ݻ纤�͸� �����ش�.
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

	// clip : �ȼ����� ���� �ʰ� ����Ѵ�.
	// �̰�� ���̹��ۿ��� ���� �Ⱦ���.
	if (output.vColor.a == 0.f)
		clip(-1);

	return output;
}
