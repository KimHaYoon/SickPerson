
struct VS_INPUT_POS
{
	float3	vPos	: POSITION;
};

// 정점버퍼에서 넘어오는 정점정보를 받아오기 위한 구조체
struct VS_INPUT_COLOR
{
	// : 뒤에 오는 요소를 Semantic 이라고 한다. 의미자라는 뜻이다.
	// 이 변수가 입력레지스터 혹은 출력레지스터의 어떤 레지스터에서
	// 값을 가져오거나 보낼지 결정해준다.
	// Semantic은 뒤에 번호가 붙을 수 있다. 이 번호는 0 ~ 15 번까지이다.
	// 번호를 안붙여주면 기본으로 0이다.
	float3	vPos	: POSITION;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR
{
	// SV가 붙으면 System Value가 된다. 즉 System에서 사용하는 값이다.
	// 이 레지스터에 넣어주면 우리가 다시 사용할 수 없다.
	float4	vPos	: SV_POSITION;
	float4	vColor	: COLOR;
	int		iDecal : DECAL;
};

struct VS_INPUT_COLOR_NORMAL
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR_NORMAL
{
	float4	vPos	: SV_POSITION;
	float3	vNormal	: NORMAL;
	float4	vColor	: COLOR;
	float3	vViewPos	: POSITION;
	float4	vProjPos	: POSITION1;
	int		iDecal : DECAL;
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
	int		iDecal : DECAL;
};

struct VS_INPUT_TEX_NORMAL
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float2	vUV		: TEXCOORD;
};

struct VS_OUTPUT_TEX_NORMAL
{
	float4	vPos	: SV_POSITION;
	float3	vNormal	: NORMAL;
	float2	vUV		: TEXCOORD;
	float3	vViewPos	: POSITION;
	float4	vProjPos	: POSITION1;
	int		iDecal : DECAL;
};

struct VS_INPUT_BUMP
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float2	vUV		: TEXCOORD;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;
};

struct VS_INPUT_ANIM
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float2	vUV		: TEXCOORD;
	float4	vWeights	: BLENDWEIGHTS;
	float4	vIndices	: BLENDINDICES;
};

struct VS_INPUT_BUMP_ANIM
{
	float3	vPos	: POSITION;
	float3	vNormal	: NORMAL;
	float2	vUV		: TEXCOORD;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;
	float4	vWeights	: BLENDWEIGHTS;
	float4	vIndices	: BLENDINDICES;
};

struct VS_OUTPUT_BUMP
{
	float4	vPos	: SV_POSITION;
	float3	vNormal	: NORMAL;
	float2	vUV		: TEXCOORD;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;
	float3	vViewPos	: POSITION;
	float4	vProjPos	: POSITION1;
	int		iDecal : DECAL;
};

struct PS_OUTPUT
{
	float4	vColor	: SV_Target;
	float4	vColor1	: SV_Target1;
	float4	vColor2	: SV_Target2;
	float4	vColor3	: SV_Target3;
	float4	vColor4	: SV_Target4;
	float4	vColor5	: SV_Target5;
};

Texture2D		g_DifTex	: register(t0);
Texture2D		g_NormalTex	: register(t1);
Texture2D		g_SpecularTex	: register(t2);
SamplerState	g_DifSmp	: register(s0);

Texture2D		g_BoneTex	: register(t7);

Texture2DArray	g_DifArrTex	: register(t10);

cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	matrix	g_matWP;
	matrix	g_matVP;
	matrix	g_matInvProj;
	matrix	g_matInvWVP;
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


cbuffer Animation2D	: register(b10)
{
	int		g_iAniType;
	int		g_iAniFrameX;
	int		g_iAniFrameY;
	int		g_iAniLengthX;
	int		g_iAniLengthY;
	float3	g_vAniEmpty;
}

float2 ComputeUV(float2 vInputUV)
{
	float2	vUV;

	vUV.x = (g_iAniFrameX + vInputUV.x) / g_iAniLengthX;
	vUV.y = (g_iAniFrameY + vInputUV.y) / g_iAniLengthY;

	return vUV;
}

struct _tagLightInfo
{
	float4	vDif;
	float4	vAmb;
	float4	vSpc;
};

_tagLightInfo ComputeLight(float3 vViewPos, float3 vViewNormal,
	float2 vUV)
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
		//vMtrlSpc = float4(g_vMtrlSpecular.xyz, 1.f);
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

struct _tagSkinning
{
	float3	vPos;
	float3	vNormal;
	float3	vTangent;
	float3	vBinormal;
};

matrix GetBoneMatrix(int idx)
{
	matrix	matBone =
	{
		g_BoneTex.Load(int3(idx * 4, 0, 0)),
		g_BoneTex.Load(int3(idx * 4 + 1, 0, 0)),
		g_BoneTex.Load(int3(idx * 4 + 2, 0, 0)),
		g_BoneTex.Load(int3(idx * 4 + 3, 0, 0))
	};

	return matBone;
}

_tagSkinning Skinning(float3 vPos, float3 vNormal, float3 vTangent,
	float3 vBinormal, float4 vWeights, float4 vIndices)
{
	_tagSkinning	tSkinning = (_tagSkinning)0;

	float	fWeights[4];
	fWeights[0] = vWeights.x;
	fWeights[1] = vWeights.y;
	fWeights[2] = vWeights.z;
	fWeights[3] = 1.f - vWeights.x - vWeights.y - vWeights.z;

	for (int i = 0; i < 4; ++i)
	{
		matrix	matBone = GetBoneMatrix((int)vIndices[i]);

		tSkinning.vPos += fWeights[i] * mul(float4(vPos, 1.f), matBone).xyz;
		tSkinning.vNormal += fWeights[i] * mul(float4(vNormal, 0.f), matBone).xyz;
		tSkinning.vTangent += fWeights[i] * mul(float4(vTangent, 0.f), matBone).xyz;
		tSkinning.vBinormal += fWeights[i] * mul(float4(vBinormal, 0.f), matBone).xyz;
	}

	tSkinning.vNormal = normalize(tSkinning.vNormal);
	tSkinning.vTangent = normalize(tSkinning.vTangent);
	tSkinning.vBinormal = normalize(tSkinning.vBinormal);

	return tSkinning;
}

_tagSkinning Skinning(float3 vPos, float3 vNormal, float4 vWeights,
	float4 vIndices)
{
	_tagSkinning	tSkinning = (_tagSkinning)0;

	float	fWeights[4];
	fWeights[0] = vWeights.x;
	fWeights[1] = vWeights.y;
	fWeights[2] = vWeights.z;
	fWeights[3] = 1.f - vWeights.x - vWeights.y - vWeights.z;

	for (int i = 0; i < 4; ++i)
	{
		matrix	matBone = GetBoneMatrix((int)vIndices[i]);

		tSkinning.vPos += fWeights[i] * mul(float4(vPos, 1.f), matBone).xyz;
		tSkinning.vNormal += fWeights[i] * mul(float4(vNormal, 0.f), matBone).xyz;
	}

	tSkinning.vNormal = normalize(tSkinning.vNormal);

	return tSkinning;
}
