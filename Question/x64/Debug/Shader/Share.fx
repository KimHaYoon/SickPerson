
// �������ۿ��� �Ѿ���� ���������� �޾ƿ��� ���� ����ü
struct VS_INPUT_COLOR
{
	// : �ڿ� ���� ��Ҹ� Semantic �̶�� �Ѵ�. �ǹ��ڶ�� ���̴�.
	// �� ������ �Է·������� Ȥ�� ��·��������� � �������Ϳ���
	// ���� �������ų� ������ �������ش�.
	// Semantic�� �ڿ� ��ȣ�� ���� �� �ִ�. �� ��ȣ�� 0 ~ 15 �������̴�.
	// ��ȣ�� �Ⱥٿ��ָ� �⺻���� 0�̴�.
	float3	vPos	: POSITION;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR
{
	// SV�� ������ System Value�� �ȴ�. �� System���� ����ϴ� ���̴�.
	// �� �������Ϳ� �־��ָ� �츮�� �ٽ� ����� �� ����.
	float4	vPos	: SV_POSITION;
	float4	vColor	: COLOR;
};

struct VS_INPUT_COLOR_NORMAL
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR_NORMAL
{
	// SV�� ������ System Value�� �ȴ�. �� System���� ����ϴ� ���̴�.
	// �� �������Ϳ� �־��ָ� �츮�� �ٽ� ����� �� ����.
	float4	vPos	: SV_POSITION;
	float3	vNormal	: NORMAL;
	float4	vColor	: COLOR;
	float3	vViewPos	: POSITION;
};

struct VS_INPUT_TEX
{
	float3	vPos	: POSITION;
	float2	vUV		: TEXCOORD;
};

struct VS_OUTPUT_TEX
{
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD;
};

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
	//float4	vColor1	: SV_Target1;
};

cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	matrix	g_matWP;
	float3	g_vPivot;
	float	g_fTrEmpty;
	float3	g_vTrLength;
	float	g_fTrEmpty1;
}

cbuffer Material	: register(b1)
{
	float4	g_vMtrlDiffuse;
	float4	g_vMtrlAmbient;
	float4	g_vMtrlSpecular;
	float4	g_vMtrlEmissive;
}

cbuffer Light	: register(b2)
{
	float4	g_vLightDiffuse;
	float4	g_vLightAmbient;
	float4	g_vLightSpecular;
	int		g_iLightType;
	float3	g_vLightDir;
	float3	g_vLightPos;
	float	g_fLightRange;
	float	g_fLightInAngle;
	float	g_fLightOutAngle;
	float	g_fLightIntensityPercent;
	float	g_fLightEmpty;
}

struct _tagLightInfo
{
	float4	vDif;
	float4	vAmb;
	float4	vSpc;
};

_tagLightInfo ComputeLight(float3 vViewPos, float3 vViewNormal)
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

	float4	vMtrlSpc = float4(g_vMtrlSpecular.xyz, 1.f);
	tInfo.vSpc = g_vLightSpecular * vMtrlSpc *
		pow(max(0, dot(vR, vView)), g_vMtrlSpecular.w) * fLightIntensity;

	//tInfo.vDif.xyz = float3(1.f, 1.f, 1.f);
	//tInfo.vSpc.xyz = float3(0.f, 0.f, 0.f);

	tInfo.vDif.w = 1.f;
	tInfo.vAmb.w = 1.f;
	tInfo.vSpc.w = 1.f;

	return tInfo;
}
