
#include "Share.fx"

struct PS_OUTPUT_SINGLE
{
	float4	vColor	: SV_Target;
};

// �Լ� ���ڿ� �������͸� �����س��� ��� ���ڸ� �ȳ־��ְ� �ش� �������Ϳ���
// ���� ����� ����Ѵ�.
VS_OUTPUT_COLOR StandardColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR	output = (VS_OUTPUT_COLOR)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT_SINGLE StandardColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor = input.vColor;

	return output;
}

VS_OUTPUT_COLOR_NORMAL StandardColorNormalVS(VS_INPUT_COLOR_NORMAL input)
{
	VS_OUTPUT_COLOR_NORMAL	output = (VS_OUTPUT_COLOR_NORMAL)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	// Normal�� ��������� ������ش�.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vColor = input.vColor;
	output.iDecal = 1;

	return output;
}

PS_OUTPUT StandardColorNormalPS(VS_OUTPUT_COLOR_NORMAL input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	//_tagLightInfo	tLight = ComputeLight(input.vViewPos, input.vNormal, float2(0.f, 0.f));

	//output.vColor = input.vColor * (tLight.vDif + tLight.vAmb) + tLight.vSpc;
	output.vColor = input.vColor;
	output.vColor1.xyz = input.vNormal * 0.5f + 0.5f;
	output.vColor1.w = 1.f;
	output.vColor2.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor2.w = input.vProjPos.w;

	output.vColor5.w = (float)input.iDecal;


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

	output.iDecal = 1;

	return output;
}

PS_OUTPUT_SINGLE StandardTexPS(VS_OUTPUT_TEX input)
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

	// clip : �ȼ����� ���� �ʰ� ����Ѵ�.
	// �̰�� ���̹��ۿ��� ���� �Ⱦ���.
	if (output.vColor.a == 0.f)
		clip(-1);

	return output;
}

VS_OUTPUT_TEX_NORMAL StandardTexNormalVS(VS_INPUT_TEX_NORMAL input)
{
	VS_OUTPUT_TEX_NORMAL	output = (VS_OUTPUT_TEX_NORMAL)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(input.vPos, 1.f), g_matWV).xyz;

	output.vUV = input.vUV;
	output.iDecal = 1;

	return output;
}

PS_OUTPUT StandardTexNormalPS(VS_OUTPUT_TEX_NORMAL input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	/*_tagLightInfo	tLight = ComputeLight(input.vViewPos, input.vNormal, 
		input.vUV);

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);
	output.vColor = vColor *
		(tLight.vDif + tLight.vAmb) + tLight.vSpc;
	output.vColor.w = vColor.w;*/

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);
	// clip : �ȼ����� ���� �ʰ� ����Ѵ�.
	// �̰�� ���̹��ۿ��� ���� �Ⱦ���.
	if (vColor.a == 0.f)
		clip(-1);

	output.vColor5.w = (float)input.iDecal;

	output.vColor = vColor;
	output.vColor1.xyz = input.vNormal * 0.5f + 0.5f;
	output.vColor1.w = 1.f;
	output.vColor2.x = input.vProjPos.z / input.vProjPos.w;
	output.vColor2.w = input.vProjPos.w;

	// ���� Diffuse, Ambient ���� �����Ѵ�.
	output.vColor2.y = g_vMtrlDiffuse.x;
	output.vColor2.z = g_vMtrlAmbient.x;

	float4	vMtrlSpc;
	if (g_vMtrlAmbient.w == 1)
	{
		vMtrlSpc = g_SpecularTex.Sample(g_DifSmp, input.vUV);
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

	return output;
}

VS_OUTPUT_BUMP StandardBumpVS(VS_INPUT_BUMP input)
{
	VS_OUTPUT_BUMP	output = (VS_OUTPUT_BUMP)0;

	float3	vPos = input.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	// Normal�� ��������� ������ش�.
	output.vNormal = normalize(mul(float4(input.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(vPos, 1.f), g_matWV).xyz;
	output.vTangent = normalize(mul(float4(input.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(input.vBinormal, 0.f), g_matWV).xyz);
	output.vUV = input.vUV;
	output.iDecal = 1;

	return output;
}

VS_OUTPUT_TEX_NORMAL StandardTexNormalAnimVS(VS_INPUT_ANIM input)
{
	VS_OUTPUT_TEX_NORMAL	output = (VS_OUTPUT_TEX_NORMAL)0;

	_tagSkinning tSkinning = Skinning(input.vPos, input.vNormal, input.vWeights,
		input.vIndices);

	tSkinning.vPos = tSkinning.vPos - g_vTrLength * g_vPivot;
	/*_tagSkinning tSkinning;
	tSkinning.vNormal = input.vNormal;
	tSkinning.vPos = input.vPos - g_vTrLength * g_vPivot;*/

	output.vProjPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	output.vViewPos = mul(float4(tSkinning.vPos, 1.f), g_matWV).xyz;
	output.vNormal = mul(float4(tSkinning.vNormal, 0.f), g_matWV);

	output.vUV = input.vUV;
	output.iDecal = 0;

	return output;
}

VS_OUTPUT_BUMP StandardBumpAnimVS(VS_INPUT_BUMP_ANIM input)
{
	VS_OUTPUT_BUMP	output = (VS_OUTPUT_BUMP)0;

	_tagSkinning tSkinning = Skinning(input.vPos, input.vNormal, 
		input.vTangent, input.vBinormal, input.vWeights,
		input.vIndices);

	tSkinning.vPos = tSkinning.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;
	// Normal�� ��������� ������ش�.
	output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.f), g_matWV).xyz);
	output.vViewPos = mul(float4(tSkinning.vPos, 1.f), g_matWV).xyz;
	output.vTangent = normalize(mul(float4(tSkinning.vTangent, 0.f), g_matWV).xyz);
	output.vBinormal = normalize(mul(float4(tSkinning.vBinormal, 0.f), g_matWV).xyz);
	output.vUV = input.vUV;
	output.iDecal = 0;

	return output;
}

PS_OUTPUT StandardBumpPS(VS_OUTPUT_BUMP input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);

	if (vColor.a == 0.f)
		clip(-1);

	// Normal�� �����ش�.
	float3x3	mat =
	{
		input.vTangent,
		input.vBinormal,
		input.vNormal
	};

	output.vColor4.xyz = input.vTangent * 0.5f + 0.5f;
	output.vColor4.w = 1.f;

	output.vColor5.xyz = input.vBinormal * 0.5f + 0.5f;
	output.vColor5.w = (float)input.iDecal;

	// Normal �ؽ��Ŀ��� �ȼ��� ���ͼ� �������ͷ� ��ȯ�Ѵ�.
	// ������ 0 ~ 1 ������ ���̹Ƿ� �� ���� ���������� ���� -1 ~ 1 ���̷�
	// ��ȯ���ַ��� * 2 - 1 �� ���ָ� �ȴ�.
	float3	vBumpNormal = g_NormalTex.Sample(g_DifSmp, input.vUV).xyz;
	vBumpNormal = vBumpNormal * 2.f - 1.f;

	float3	vViewNormal = normalize(mul(vBumpNormal, mat));
	
	output.vColor = vColor;
	output.vColor1.xyz = vViewNormal * 0.5f + 0.5f;
	output.vColor1.w = 1.f;
	output.vColor2.x = (input.vProjPos.z / input.vProjPos.w);
	output.vColor2.w = input.vProjPos.w;

	// ���� Diffuse, Ambient ���� �����Ѵ�.
	output.vColor2.y = g_vMtrlDiffuse.x;
	output.vColor2.z = g_vMtrlAmbient.x;

	float4	vMtrlSpc;
	if (g_vMtrlAmbient.w == 1)
	{
		vMtrlSpc = g_SpecularTex.Sample(g_DifSmp, input.vUV);
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

	/*_tagLightInfo	tLight = ComputeLight(input.vViewPos, vViewNormal, input.vUV);

	output.vColor.xyz = vColor.xyz * (tLight.vDif.xyz + tLight.vAmb.xyz) + tLight.vSpc.xyz;
	output.vColor.w = vColor.w;*/

	//output.vColor1 = float4(1.f, 0.f, 0.f, 1.f);
	//output.vColor2 = float4(0.f, 1.f, 0.f, 1.f);
	//output.vColor3 = float4(0.f, 0.f, 1.f, 1.f);

	//output.vColor *= float4(0.f, 1.f, 0.f, 1.f);

	return output;
}

PS_OUTPUT_SINGLE StandardBumpForwardPS(VS_OUTPUT_BUMP input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);

	if (vColor.a == 0.f)
		clip(-1);

	// Normal�� �����ش�.
	float3x3	mat =
	{
		input.vTangent,
		input.vBinormal,
		input.vNormal
	};

	// Normal �ؽ��Ŀ��� �ȼ��� ���ͼ� �������ͷ� ��ȯ�Ѵ�.
	// ������ 0 ~ 1 ������ ���̹Ƿ� �� ���� ���������� ���� -1 ~ 1 ���̷�
	// ��ȯ���ַ��� * 2 - 1 �� ���ָ� �ȴ�.
	float3	vBumpNormal = g_NormalTex.Sample(g_DifSmp, input.vUV).xyz;
	vBumpNormal = vBumpNormal * 2.f - 1.f;

	float3	vViewNormal = normalize(mul(vBumpNormal, mat));

	_tagLightInfo	tLight = ComputeLight(input.vViewPos, vViewNormal, input.vUV);

	output.vColor.xyz = vColor.xyz * (tLight.vDif.xyz + tLight.vAmb.xyz) + tLight.vSpc.xyz;
	output.vColor.w = vColor.w;

	return output;
}

PS_OUTPUT_SINGLE StandardTexNormalForwardPS(VS_OUTPUT_TEX_NORMAL input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	float4	vColor = g_DifTex.Sample(g_DifSmp, input.vUV);

	if (vColor.a == 0.f)
		clip(-1);

	_tagLightInfo	tLight = ComputeLight(input.vViewPos, input.vNormal, input.vUV);

	output.vColor.xyz = vColor.xyz * (tLight.vDif.xyz + tLight.vAmb.xyz) + tLight.vSpc.xyz;
	output.vColor.w = vColor.w;

	return output;
}

VS_OUTPUT_BUMP StandardShadowBumpAnimVS(VS_INPUT_BUMP_ANIM input)
{
	VS_OUTPUT_BUMP	output = (VS_OUTPUT_BUMP)0;

	_tagSkinning tSkinning = Skinning(input.vPos, input.vNormal,
		input.vTangent, input.vBinormal, input.vWeights,
		input.vIndices);

	tSkinning.vPos = tSkinning.vPos - g_vTrLength * g_vPivot;

	output.vProjPos = mul(float4(tSkinning.vPos, 1.f), g_matWVP);
	output.vPos = output.vProjPos;

	return output;
}

PS_OUTPUT_SINGLE StandardShadowColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor.w = 1.f;

	return output;
}

PS_OUTPUT_SINGLE StandardShadowColorNormalPS(VS_OUTPUT_COLOR_NORMAL input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor.w = 1.f;

	return output;
}

PS_OUTPUT_SINGLE StandardShadowTexNormalPS(VS_OUTPUT_TEX_NORMAL input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor.w = 1.f;

	return output;
}

PS_OUTPUT_SINGLE StandardShadowBumpPS(VS_OUTPUT_BUMP input)
{
	PS_OUTPUT_SINGLE	output = (PS_OUTPUT_SINGLE)0;

	output.vColor.xyz = (float3)(input.vProjPos.z / input.vProjPos.w);
	output.vColor.w = 1.f;

	return output;
}