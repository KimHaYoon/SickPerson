
#include "Share.fx"

SamplerState	g_LightSmp	: register(s11);
Texture2D		g_GBufferAlbedo	: register(t11);
Texture2D		g_GBufferNrm	: register(t12);
Texture2D		g_GBufferDepth	: register(t13);
Texture2D		g_GBufferSpc	: register(t14);
Texture2D		g_LightDif	: register(t15);
Texture2D		g_LightSpc	: register(t16);
Texture2D		g_Deferred	: register(t17);

// HLSL���� ���������� static�� �ٿ��ָ� �ش� ������ �ܺο��� ����
// ������ �� ����.
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
	tInfo.vDif = g_vLightDiffuse * vMtrlDif *
		max(0, dot(vViewNormal, vLightDir)) * fLightIntensity;
	tInfo.vAmb = g_vLightAmbient * vMtrlAmb * fLightIntensity;

	// ���ݻ籤 ó���� �ϱ� ���ؼ��� ������ �ش� ������ �ε������� �ݻ�Ǵ� ���Ͱ� �ʿ��ϴ�.
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

	// ���� �ؽ��Ŀ��� ������ ���´�.
	float4	vDepthTex = g_GBufferDepth.Sample(g_LightSmp, input.vUV);

	if (vDepthTex.w == 0.f)
		clip(-1);

	float4	vProjPos;
	vProjPos.x = (input.vUV.x * 2.f - 1.f) * vDepthTex.w;
	vProjPos.y = (input.vUV.y * -2.f + 1.f) * vDepthTex.w;
	vProjPos.z = vDepthTex.x * vDepthTex.w;
	vProjPos.w = vDepthTex.w;

	// ����������� ��ġ�� ���Ѵ�.
	float3	vViewPos = mul(vProjPos, g_matInvProj).xyz;

	// Normal Target���� ������ ���´�.
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
