#include "LandScape.h"
#include "Material.h"
#include "Renderer.h"
#include "../GameObject/GameObject.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Mesh.h"
#include "../Core/PathManager.h"
#include "../Core/NavigationManager.h"

Engine_USING

CLandScape::CLandScape()
{
	SetTag("LandScape");
	SetTypeName("CLandScape");
	SetTypeID<CLandScape>();
	m_eType = CT_LANDSCAPE;
	m_iDetailLevel = 37;
	m_iSplatCount = 0;
}


CLandScape::CLandScape(const CLandScape & landscape)	:
	CComponent(landscape)
{
	m_iDetailLevel = landscape.m_iDetailLevel;
	m_iSplatCount = landscape.m_iSplatCount;
}

CLandScape::~CLandScape()
{
}

void CLandScape::SetDetailLevel(int iDetailLevel)
{
	m_iDetailLevel = iDetailLevel;
}

bool CLandScape::CreateLandScape(const string& strMeshKey, int iVtxCount, bool bBump, const string& strTexKey,
	const wchar_t* pFileName,
	const wchar_t* pNormalName, const wchar_t* pSpecularName,
	const char* pHeightMap, const string& strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("LandScape");

	CMaterial*	pMaterial = pRenderer->CreateMaterial();

	pMaterial->SetDiffuseTexInfo("Linear", strTexKey, 0, 0, pFileName, strPathKey);
	pMaterial->SetNormalTexInfo("Linear", strTexKey + "_N", 1, 1,
		pNormalName, strPathKey);
	pMaterial->SetSpecularTexInfo("Linear", strTexKey + "_S", 2, 2,
		pSpecularName, strPathKey);

	SAFE_RELEASE(pMaterial);

	pRenderer->SetShader(LANDSCAPE_SHADER);
	pRenderer->SetInputLayout("Bump");

	// 높이맵을 읽어온다.
	const char* pPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(strPathKey);
	string	strPath;

	if (pPath)
		strPath = pPath;
	strPath += pHeightMap;

	DWORD*	pPixel = NULL;
	int	iBitCount = 3;

	FILE*	pFile = NULL;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (pFile)
	{
		BITMAPFILEHEADER	fh;
		BITMAPINFOHEADER	ih;

		fread(&fh, sizeof(fh), 1, pFile);
		fread(&ih, sizeof(ih), 1, pFile);

		fseek(pFile, fh.bfOffBits, SEEK_SET);

		pPixel = new DWORD[ih.biWidth * ih.biHeight];

		fread(pPixel, sizeof(DWORD), ih.biWidth * ih.biHeight, pFile);

		// 상하가 뒤집어져서 저장되기 때문에 바꿔준다.
		DWORD*	pLine = new DWORD[ih.biWidth];
		for (int i = 0; i < ih.biHeight / 2; ++i)
		{
			memcpy(pLine, &pPixel[i * ih.biWidth], sizeof(DWORD) * ih.biWidth);
			memcpy(&pPixel[i * ih.biWidth],
				&pPixel[(ih.biWidth - 1 - i) * ih.biWidth],
				sizeof(DWORD) * ih.biWidth);
			memcpy(&pPixel[(ih.biWidth - 1 - i) * ih.biWidth],
				pLine, sizeof(DWORD) * ih.biWidth);
		}

		SAFE_DELETE_ARRAY(pLine);

		fclose(pFile);
	}

	m_iNumX = iVtxCount;
	m_iNumZ = iVtxCount;

	vector<VERTEXBUMP>	vecVtx;
	vecVtx.resize(iVtxCount * iVtxCount);

	for (int i = 0; i < m_iNumZ; ++i)
	{
		for (int j = 0; j < m_iNumX; ++j)
		{
			VERTEXBUMP	tVtx = {};

			tVtx.vPos = Vector3((float)j, 
				(pPixel[i * m_iNumX + j] & 0x000000ff) * 0.05, (float)(m_iNumZ - 1) - i);
			m_vecPos.push_back(tVtx.vPos);

			tVtx.vNormal = Vector3(0.f, 1.f, 0.f);
			tVtx.vUV = Vector2((float)j / (float)(m_iNumX - 1),
				(float)i / (float)(m_iNumZ - 1));
			//tVtx.vUV = Vector2(j, i);

			vecVtx[i * m_iNumX + j] = tVtx;
		}
	}

	SAFE_DELETE_ARRAY(pPixel);
		
	vector<UINT>	vecIndex((m_iNumX - 1) * (m_iNumZ - 1) * 2 * 3);

	int		iCount = 0;

	for (int i = 0; i < m_iNumZ - 1; ++i)
	{
		for (int j = 0; j < m_iNumX - 1; ++j)
		{
			// 좌상단 정점의 인덱스를 구해준다.
			int	idx = i * m_iNumX + j;
			// 우상단 삼각형 인덱스
			vecIndex[iCount++] = idx;
			vecIndex[iCount++] = idx + 1;
			vecIndex[iCount++] = idx + m_iNumX + 1;

			// 삼각형의 면법선을 구한다.
			Vector3	vEdge1 = m_vecPos[idx + 1] - m_vecPos[idx];
			Vector3	vEdge2 = m_vecPos[idx + m_iNumX + 1] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			Vector3	vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());

			// 좌하단 삼각형 인덱스
			vecIndex[iCount++] = idx;
			vecIndex[iCount++] = idx + m_iNumX + 1;
			vecIndex[iCount++] = idx + m_iNumX;

			// 삼각형의 면법선을 구한다.
			vEdge1 = m_vecPos[idx + m_iNumX + 1] - m_vecPos[idx];
			vEdge2 = m_vecPos[idx + m_iNumX] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());
		}
	}

	ComputeNormal(vecVtx, vecIndex);
	ComputeTangent(vecVtx, vecIndex);

	CMesh*	pMesh = GET_SINGLE(CResourcesManager)->CreateMesh(strMeshKey,
		vecVtx.size(), sizeof(VERTEXBUMP), D3D12_USAGE_DEFAULT,
		D3D12_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecVtx[0],
		vecIndex.size(), 4, D3D12_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT, &vecIndex[0]);

	pMesh->SetShaderKey(LANDSCAPE_SHADER);
	pMesh->SetInputLayoutKey("Bump");
	pRenderer->SetMesh(pMesh);

	SAFE_RELEASE(pMesh);

	pRenderer->CreateCBuffer("LandScape", 12, sizeof(LANDSCAPECBUFFER),
		SCT_PIXEL);

	SAFE_RELEASE(pRenderer);

	// 내비게이션 관리자에 지형을 등록한다.
	GET_SINGLE(CNavigationManager)->AddLandScapeInfo(strMeshKey,
		m_iNumX, m_iNumZ, m_pScene, m_pTransform, &m_vecPos);

	return true;
}

bool CLandScape::SetDiffuseSplatting(const string & strSmpKey, const string & strDifKey,
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		return false;

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	if (pMaterial)
	{
		pMaterial->AddMultiTexture(strSmpKey, strDifKey, 11, 11,
			pvecPath, strPathKey);

		SAFE_RELEASE(pMaterial);
	}

	else
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pRenderer);

	++m_iSplatCount;

	return true;
}

bool CLandScape::SetNormalSplatting(const string & strSmpKey, const string & strDifKey,
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		return false;

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	if (pMaterial)
	{
		pMaterial->AddMultiTexture(strSmpKey, strDifKey, 12, 11,
			pvecPath, strPathKey);

		SAFE_RELEASE(pMaterial);
	}

	else
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pRenderer);

	++m_iSplatCount;

	return true;
}

bool CLandScape::SetSpecularSplatting(const string & strSmpKey, const string & strDifKey,
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		return false;

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	if (pMaterial)
	{
		pMaterial->AddMultiTexture(strSmpKey, strDifKey, 13, 11,
			pvecPath, strPathKey);

		SAFE_RELEASE(pMaterial);
	}

	else
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pRenderer);

	++m_iSplatCount;

	return true;
}

bool CLandScape::SetSplattingAlpha(const string & strSmpKey, 
	const string & strDifKey, int iRegTex, int iRegSmp, 
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		return false;

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	if (pMaterial)
	{
		pMaterial->AddMultiTexture(strSmpKey, strDifKey, 14, 11,
			pvecPath, strPathKey);

		SAFE_RELEASE(pMaterial);
	}

	else
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pRenderer);

	return true;
}

bool CLandScape::Init()
{
	return true;
}

void CLandScape::Input(float fTime)
{
}

int CLandScape::Update(float fTime)
{
	return 0;
}

int CLandScape::LateUpdate(float fTime)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (pRenderer)
	{
		LANDSCAPECBUFFER	tBuffer = {};
		tBuffer.iDetailLevel = m_iDetailLevel;
		tBuffer.iSplatCount = m_iSplatCount;
		pRenderer->UpdateCBuffer("LandScape", 12, sizeof(LANDSCAPECBUFFER),
			SCT_PIXEL, &tBuffer);
		SAFE_RELEASE(pRenderer);
	}

	return 0;
}

void CLandScape::Collision(float fTime)
{
}

void CLandScape::Render(float fTime)
{
}

CLandScape * CLandScape::Clone()
{
	return new CLandScape(*this);
}

void CLandScape::Save(FILE * pFile)
{
}

void CLandScape::Load(FILE * pFile)
{
}

void CLandScape::ComputeNormal(vector<VERTEXBUMP>& vecVtx, 
	const vector<UINT>& vecIdx)
{
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		int	idx0 = vecIdx[i * 3];
		int	idx1 = vecIdx[i * 3 + 1];
		int	idx2 = vecIdx[i * 3 + 2];

		vecVtx[idx0].vNormal += m_vecFaceNormal[i];
		vecVtx[idx1].vNormal += m_vecFaceNormal[i];
		vecVtx[idx2].vNormal += m_vecFaceNormal[i];
	}

	for (size_t i = 0; i < vecVtx.size(); ++i)
	{
		vecVtx[i].vNormal = vecVtx[i].vNormal.Normalize();
	}
}

void CLandScape::ComputeTangent(vector<VERTEXBUMP>& vecVtx, 
	const vector<UINT>& vecIdx)
{
	// 탄젠트 벡터 구함.
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		int	idx0 = vecIdx[i * 3];
		int	idx1 = vecIdx[i * 3 + 1];
		int	idx2 = vecIdx[i * 3 + 2];

		float	fVtx1[3], fVtx2[3];
		fVtx1[0] = vecVtx[idx1].vPos.x - vecVtx[idx0].vPos.x;
		fVtx1[1] = vecVtx[idx1].vPos.y - vecVtx[idx0].vPos.y;
		fVtx1[2] = vecVtx[idx1].vPos.z - vecVtx[idx0].vPos.z;

		fVtx2[0] = vecVtx[idx2].vPos.x - vecVtx[idx0].vPos.x;
		fVtx2[1] = vecVtx[idx2].vPos.y - vecVtx[idx0].vPos.y;
		fVtx2[2] = vecVtx[idx2].vPos.z - vecVtx[idx0].vPos.z;

		float	ftu[2], ftv[2];
		ftu[0] = vecVtx[idx1].vUV.x - vecVtx[idx0].vUV.x;
		ftv[0] = vecVtx[idx1].vUV.y - vecVtx[idx0].vUV.y;

		ftu[1] = vecVtx[idx2].vUV.x - vecVtx[idx0].vUV.x;
		ftv[1] = vecVtx[idx2].vUV.y - vecVtx[idx0].vUV.y;

		float	fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

		Vector3	vTangent;
		vTangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
		vTangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
		vTangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;

		vTangent = vTangent.Normalize();

		vecVtx[idx0].vTangent = vTangent;
		vecVtx[idx1].vTangent = vTangent;
		vecVtx[idx2].vTangent = vTangent;

		vecVtx[idx0].vBinormal = vecVtx[idx0].vNormal.Cross(vTangent).Normalize();
		vecVtx[idx1].vBinormal = vecVtx[idx1].vNormal.Cross(vTangent).Normalize();
		vecVtx[idx2].vBinormal = vecVtx[idx2].vNormal.Cross(vTangent).Normalize();
	}
}
