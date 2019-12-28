
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
	// Normal�� ��������� ������ش�.
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

	// Vertex Shader���� ViewPos, Normal�� View�������� ��ȯ�ؼ� ����� ������Դ�.
	// ������ �����鵵 ��������� ��ȯ�� �־�� �Ѵ�.
	float3	vLightDir = (float3)0;
	float	fLightIntensity = 1.f;

	// ���⼺ ������ ���
	if (g_iLightType == 0)
	{
		// ��������� ���� ������ �� �������� ��ȯ���ش�.
		vLightDir = mul(float4(g_vLightDir, 0.f), g_matView).xyz;

		// ��ȯ�� �� ������ ���� ������ �������ͷ� ������ְ� ������ �������� ���Ѵ�.
		// ������ �������� ���ؾ� ������ ���ؼ� ������ ������ ���� �� �ֱ� �����̴�.
		vLightDir = -normalize(vLightDir);
	}

	// �������� ���
	if (g_iLightType == 1)
	{
		// ���� ��ġ�� ��������� ��ȯ���ش�.
		float3	vLightPos = mul(float4(g_vLightPos, 1.f), g_matView).xyz;

		// ���� ������ �����ش�.
		vLightDir = vLightPos - vViewPos;
		vLightDir = normalize(vLightDir);

		// ������ ���� ó���� ���� ������ ������ �Ÿ��� �����ش�.
		float	fDist = distance(vLightPos, vViewPos);

		if (fDist > g_fLightRange)
			fLightIntensity = 0.f;

		else if (fDist > g_fLightIntensityPercent * g_fLightRange)
		{
			// ���踦 ó���� ������ �Ÿ��� ���Ѵ�.
			float	fLightRange = g_fLightRange * (1.f - g_fLightIntensityPercent);
			fDist -= g_fLightRange * g_fLightIntensityPercent;

			fLightIntensity = 1.f - fDist / fLightRange;
			//fLightIntensity = 1.f / fDist;
			//fLightIntensity = 1.f / fDist;
		}

		//fLightIntensity = 1.f;
	}

	// Spot ������ ���
	if (g_iLightType == 2)
	{
		// Spot ������ �ٶ󺸴� ������ ��������� ��ȯ���ش�.
		// ��������� ���� ������ �� �������� ��ȯ���ش�.
		float3 vLightView = mul(float4(g_vLightDir, 0.f), g_matView).xyz;

		// ��ȯ�� �� ������ ���� ������ �������ͷ� ������ְ� ������ �������� ���Ѵ�.
		// ������ �������� ���ؾ� ������ ���ؼ� ������ ������ ���� �� �ֱ� �����̴�.
		vLightView = -normalize(vLightView);

		// �������� ������ �ٶ󺸴� ������ �����ش�.
		// ���� ��ġ�� ��������� ��ȯ���ش�.
		float3	vLightPos = mul(float4(g_vLightPos, 1.f), g_matView).xyz;

		// ���� ������ �����ش�.
		vLightDir = vLightPos - vViewPos;
		vLightDir = normalize(vLightDir);

		// ����� �������� �Ÿ��� �����ش�.
		float	fDist = distance(vLightPos, vViewPos);

		if (fDist < g_fLightRange)
		{
			// ������ ������ �ٶ󺸴� ������ �����ش�.
			float3	vViewDir = -vLightDir;

			// ������ ������ �ٶ󺸴� ����� ������ ������ �̿��ؼ� ������ �����ش�.
			// cos ��Ÿ���� �̿��ؼ� ���Ѵ�.
			float	fAngle = dot(vViewDir, vLightView);

			float	fInAngle = cos(g_fLightInAngle * 0.5f);

			// ���� ������ ���ݰ������� �۴ٸ� ���� �ٱ��̶� �ǹ��̴�.
			if (fAngle < fInAngle)
			{
				// �ٱ��ʰ��� ����
				float	fOutAngle = cos(g_fLightOutAngle * 0.5f);

				// �ٱ��� �������ٵ� ũ�Ƿ� Intensity�� 0���� ó���Ѵ�.
				if (fAngle < fOutAngle)
					fLightIntensity = 0.f;

				// �ٱ��� ���� �ȿ� ������ ���
				else
				{
					fLightIntensity = 0.5f;
				}
			}

			// ���� ���� �ȿ� ������ ���
			else
			{
				fLightIntensity = 1.f;
			}
		}

		// ���� ���� �ٱ��̶�� 0���� ó��
		else
			fLightIntensity = 0.f;
	}
	//fLightIntensity = 1.f;
	//vViewNormal = vLightDir;
	//vLightDir = vViewNormal;

	// ������ ������ �� ������ ������ ������� ������ ������� �븻�� �����ؼ� cos ��Ÿ�� ����
	// �ش�. �� ���� �̿��ؼ� ������ ������ ó���� �ϴµ� �� ���� - ���� ������ �Ǹ� ������
	// �ڿ��� ����ٴ� �ǹ��̴�. �׷��� ������ ���´ٸ� 0���� ó�����ش�.
	// �� �������� �̿��ؼ� Diffuse �� �������ش�.
	// Diffuse�� ������ Diffuse * ������ Diffuse * �������� �ȴ�.
	tInfo.vDif = g_vLightDiffuse * g_vMtrlDiffuse *
		max(0, dot(vViewNormal, vLightDir)) * fLightIntensity;
	tInfo.vAmb = g_vLightAmbient * g_vMtrlAmbient;

	// ���ݻ籤 ó���� �ϱ� ���ؼ��� ������ �ش� ������ �ε������� �ݻ�Ǵ� ���Ͱ� �ʿ��ϴ�.
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
	output.vColor5.w = 1.f;

	// Normal �ؽ��Ŀ��� �ȼ��� ���ͼ� �������ͷ� ��ȯ�Ѵ�.
	// ������ 0 ~ 1 ������ ���̹Ƿ� �� ���� ���������� ���� -1 ~ 1 ���̷�
	// ��ȯ���ַ��� * 2 - 1 �� ���ָ� �ȴ�.
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

	// ���� Diffuse, Ambient ���� �����Ѵ�.
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
