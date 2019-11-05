#include "Material.h"
#include "../Device.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Sampler.h"
#include "../Rendering/ShaderManager.h"

Engine_USING

CMaterial::CMaterial()
{
	SetTag("Material");
	SetTypeName("CMaterial");
	SetTypeID<CMaterial>();
	m_eType = CT_MATERIAL;

	m_tMtrl.vDif = Vector4(1.f, 1.f, 1.f, 0.f);
	m_tMtrl.vAmb = Vector4(0.2f, 0.2f, 0.2f, 0.f);
	m_tMtrl.vSpc = Vector4(1.f, 1.f, 1.f, 3.2f);
	m_tMtrl.vEmv = Vector4(0.f, 0.f, 0.f, 0.f);

	m_pDifInfo = NULL;
	m_pNormalInfo = NULL;
	m_pSpecularInfo = NULL;
}

CMaterial::CMaterial(const CMaterial & material) :
	CComponent(material)
{
	m_tMtrl = material.m_tMtrl;

	m_pDifInfo = NULL;
	if (material.m_pDifInfo)
	{
		m_pDifInfo = new TEXTUREINFO;
		*m_pDifInfo = *material.m_pDifInfo;
		if (m_pDifInfo->pTexture)
			m_pDifInfo->pTexture->AddRef();

		if (m_pDifInfo->pSampler)
			m_pDifInfo->pSampler->AddRef();
	}

	m_pNormalInfo = NULL;
	if (material.m_pNormalInfo)
	{
		m_pNormalInfo = new TEXTUREINFO;
		*m_pNormalInfo = *material.m_pNormalInfo;
		if (m_pNormalInfo->pTexture)
			m_pNormalInfo->pTexture->AddRef();

		if (m_pNormalInfo->pSampler)
			m_pNormalInfo->pSampler->AddRef();
	}

	m_pSpecularInfo = NULL;
	if (material.m_pSpecularInfo)
	{
		m_pSpecularInfo = new TEXTUREINFO;
		*m_pSpecularInfo = *material.m_pSpecularInfo;
		if (m_pSpecularInfo->pTexture)
			m_pSpecularInfo->pTexture->AddRef();

		if (m_pSpecularInfo->pSampler)
			m_pSpecularInfo->pSampler->AddRef();
	}

	m_vecMultiTex.clear();
	for (size_t i = 0; i < material.m_vecMultiTex.size(); ++i)
	{
		PTEXTUREINFO	pInfo = new TEXTUREINFO;
		*pInfo = *material.m_vecMultiTex[i];
		if (pInfo->pTexture)
			pInfo->pTexture->AddRef();

		if (pInfo->pSampler)
			pInfo->pSampler->AddRef();

		m_vecMultiTex.push_back(pInfo);
	}
}

CMaterial::~CMaterial()
{
	if (m_pDifInfo)
	{
		SAFE_RELEASE(m_pDifInfo->pTexture);
		SAFE_RELEASE(m_pDifInfo->pSampler);
		SAFE_DELETE(m_pDifInfo);
	}
	if (m_pNormalInfo)
	{
		SAFE_RELEASE(m_pNormalInfo->pTexture);
		SAFE_RELEASE(m_pNormalInfo->pSampler);
		SAFE_DELETE(m_pNormalInfo);
	}
	if (m_pSpecularInfo)
	{
		SAFE_RELEASE(m_pSpecularInfo->pTexture);
		SAFE_RELEASE(m_pSpecularInfo->pSampler);
		SAFE_DELETE(m_pSpecularInfo);
	}

	for (size_t i = 0; i < m_vecMultiTex.size(); ++i)
	{
		SAFE_RELEASE(m_vecMultiTex[i]->pTexture);
		SAFE_RELEASE(m_vecMultiTex[i]->pSampler);
		SAFE_DELETE(m_vecMultiTex[i]);
	}
}

void CMaterial::SetMaterial(const Vector4 & vDif, const Vector4 & vAmb, const Vector4 & vSpc,
	const Vector4 & vEmv, float fPower)
{
	m_tMtrl.vDif = vDif;
	m_tMtrl.vDif.w = 0.f;
	m_tMtrl.vAmb = vAmb;
	m_tMtrl.vAmb.w = 0.f;
	m_tMtrl.vSpc = vSpc;
	m_tMtrl.vEmv = vEmv;
	m_tMtrl.vSpc.w = fPower;
}

void CMaterial::SetDiffuseTexInfo(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const TCHAR * pFileName,
	const string & strPathKey)
{
	if (m_pDifInfo)
	{
		SAFE_RELEASE(m_pDifInfo->pTexture);
		SAFE_RELEASE(m_pDifInfo->pSampler);
	}

	else
		m_pDifInfo = new TEXTUREINFO;

	if (!pFileName)
		m_pDifInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_pDifInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strDifKey, pFileName, strPathKey);

	m_pDifInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_pDifInfo->iRegisterTex = iRegTex;
	m_pDifInfo->iRegisterSmp = iRegSmp;
}

void CMaterial::SetDiffuseTexInfo(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const vector<wstring>* vecFileName, const string & strPathKey)
{
	if (m_pDifInfo)
	{
		SAFE_RELEASE(m_pDifInfo->pTexture);
		SAFE_RELEASE(m_pDifInfo->pSampler);
	}

	else
		m_pDifInfo = new TEXTUREINFO;

	if (!vecFileName)
		m_pDifInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_pDifInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strDifKey, *vecFileName, strPathKey);

	m_pDifInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_pDifInfo->iRegisterTex = iRegTex;
	m_pDifInfo->iRegisterSmp = iRegSmp;
}

void CMaterial::SetDiffuseTexInfoFromFullPath(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const char * pFullPath)
{
	if (m_pDifInfo)
	{
		SAFE_RELEASE(m_pDifInfo->pTexture);
		SAFE_RELEASE(m_pDifInfo->pSampler);
	}

	else
		m_pDifInfo = new TEXTUREINFO;

	if (!pFullPath)
		m_pDifInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_pDifInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strDifKey, pFullPath);

	m_pDifInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_pDifInfo->iRegisterTex = iRegTex;
	m_pDifInfo->iRegisterSmp = iRegSmp;
}

void CMaterial::SetDiffuseTex(CTexture * pTex)
{
	if (m_pDifInfo)
	{
		SAFE_RELEASE(m_pDifInfo->pTexture);
	}

	else
		m_pDifInfo = new TEXTUREINFO;

	pTex->AddRef();
	m_pDifInfo->pTexture = pTex;
}

void CMaterial::SetDiffuseTex(const string & strKey)
{
	if (m_pDifInfo)
	{
		SAFE_RELEASE(m_pDifInfo->pTexture);
	}

	else
		m_pDifInfo = new TEXTUREINFO;

	CTexture* pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);

	m_pDifInfo->pTexture = pTex;
}

void CMaterial::SetDiffuseTex(const string & strKey, const char * pFullPath)
{
	if (m_pDifInfo)
	{
		SAFE_RELEASE(m_pDifInfo->pTexture);
	}

	else
		m_pDifInfo = new TEXTUREINFO;

	CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strKey, pFullPath);

	m_pDifInfo->pTexture = pTex;
}

void CMaterial::SetDiffuseTex(const string & strKey, const vector<string>& vecFullPath)
{
	if (m_pDifInfo)
	{
		SAFE_RELEASE(m_pDifInfo->pTexture);
	}

	else
		m_pDifInfo = new TEXTUREINFO;

	CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strKey, vecFullPath);

	m_pDifInfo->pTexture = pTex;
}

void CMaterial::SetDiffuseTexRegister(int iRegister)
{
	if (!m_pDifInfo)
		m_pDifInfo = new TEXTUREINFO;
	m_pDifInfo->iRegisterTex = iRegister;
}

void CMaterial::SetDiffuseSampler(const string & strKey)
{
	if (!m_pDifInfo)
		m_pDifInfo = new TEXTUREINFO;

	else
		SAFE_RELEASE(m_pDifInfo->pSampler);
	m_pDifInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strKey);
}

void CMaterial::SetNormalTexInfo(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp, const TCHAR * pFileName,
	const string & strPathKey)
{
	if (m_pNormalInfo)
	{
		SAFE_RELEASE(m_pNormalInfo->pTexture);
		SAFE_RELEASE(m_pNormalInfo->pSampler);
	}

	else
		m_pNormalInfo = new TEXTUREINFO;

	if (!pFileName)
		m_pNormalInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_pNormalInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strDifKey, pFileName, strPathKey);

	m_pNormalInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_pNormalInfo->iRegisterTex = iRegTex;
	m_pNormalInfo->iRegisterSmp = iRegSmp;
}

void CMaterial::SetNormalTexInfoFromFullPath(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const char * pFullPath)
{
	if (m_pNormalInfo)
	{
		SAFE_RELEASE(m_pNormalInfo->pTexture);
		SAFE_RELEASE(m_pNormalInfo->pSampler);
	}

	else
		m_pNormalInfo = new TEXTUREINFO;

	if (!pFullPath)
		m_pNormalInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_pNormalInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strDifKey, pFullPath);

	m_pNormalInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_pNormalInfo->iRegisterTex = iRegTex;
	m_pNormalInfo->iRegisterSmp = iRegSmp;
}

void CMaterial::SetSpecularTexInfo(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp, const TCHAR * pFileName,
	const string & strPathKey)
{
	if (m_pSpecularInfo)
	{
		SAFE_RELEASE(m_pSpecularInfo->pTexture);
		SAFE_RELEASE(m_pSpecularInfo->pSampler);
	}

	else
		m_pSpecularInfo = new TEXTUREINFO;

	if (!pFileName)
		m_pSpecularInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_pSpecularInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strDifKey, pFileName, strPathKey);

	m_pSpecularInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_pSpecularInfo->iRegisterTex = iRegTex;
	m_pSpecularInfo->iRegisterSmp = iRegSmp;

	m_tMtrl.vAmb.w = 1;
}

void CMaterial::SetSpecularTexInfoFromFullPath(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const char * pFullPath)
{
	if (m_pSpecularInfo)
	{
		SAFE_RELEASE(m_pSpecularInfo->pTexture);
		SAFE_RELEASE(m_pSpecularInfo->pSampler);
	}

	else
		m_pSpecularInfo = new TEXTUREINFO;

	if (!pFullPath)
		m_pSpecularInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_pSpecularInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strDifKey, pFullPath);

	m_pSpecularInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_pSpecularInfo->iRegisterTex = iRegTex;
	m_pSpecularInfo->iRegisterSmp = iRegSmp;

	m_tMtrl.vAmb.w = 1;
}

void CMaterial::AddMultiTexture(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const TCHAR * pFileName, const string & strPathKey)
{
	PTEXTUREINFO	pInfo = new TEXTUREINFO;

	if (!pFileName)
		pInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		pInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strDifKey, pFileName, strPathKey);

	pInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	pInfo->iRegisterTex = iRegTex;
	pInfo->iRegisterSmp = iRegSmp;

	m_vecMultiTex.push_back(pInfo);
}

void CMaterial::AddMultiTexture(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	PTEXTUREINFO	pInfo = new TEXTUREINFO;

	if (!pvecPath)
		pInfo->pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		pInfo->pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strDifKey, *pvecPath, strPathKey);

	pInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	pInfo->iRegisterTex = iRegTex;
	pInfo->iRegisterSmp = iRegSmp;

	m_vecMultiTex.push_back(pInfo);
}

void CMaterial::AddMultiTexture(const string & strSmpKey, ID3D11ShaderResourceView * pSRV, int iRegTex, int iRegSmp)
{
	CTexture* pTexture = new CTexture;

	pTexture->SetShaderResourceView(pSRV);

	PTEXTUREINFO	pInfo = new TEXTUREINFO;

	pInfo->pTexture = pTexture;
	pInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);
	pInfo->iRegisterTex = iRegTex;
	pInfo->iRegisterSmp = iRegSmp;

	m_vecMultiTex.push_back(pInfo);
}

void CMaterial::SetMaterial(int iShaderConstantType)
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material", &m_tMtrl, iShaderConstantType);

	if (m_pDifInfo)
	{
		if (m_pDifInfo->pTexture)
			m_pDifInfo->pTexture->SetTexture(m_pDifInfo->iRegisterTex, iShaderConstantType);

		if (m_pDifInfo->pSampler)
			m_pDifInfo->pSampler->SetSampler(m_pDifInfo->iRegisterSmp, iShaderConstantType);
	}

	if (m_pNormalInfo)
	{
		m_pNormalInfo->pTexture->SetTexture(m_pNormalInfo->iRegisterTex, iShaderConstantType);
		m_pNormalInfo->pSampler->SetSampler(m_pNormalInfo->iRegisterSmp, iShaderConstantType);
	}

	if (m_pSpecularInfo)
	{
		m_pSpecularInfo->pTexture->SetTexture(m_pSpecularInfo->iRegisterTex, iShaderConstantType);
		m_pSpecularInfo->pSampler->SetSampler(m_pSpecularInfo->iRegisterSmp, iShaderConstantType);
	}

	for (size_t i = 0; i < m_vecMultiTex.size(); ++i)
	{
		if (m_vecMultiTex[i]->pTexture)
			m_vecMultiTex[i]->pTexture->SetTexture(m_vecMultiTex[i]->iRegisterTex, iShaderConstantType);

		if (m_vecMultiTex[i]->pSampler)
			m_vecMultiTex[i]->pSampler->SetSampler(m_vecMultiTex[i]->iRegisterSmp, iShaderConstantType);
	}
}

void CMaterial::ResetMaterial(int iShaderConstantType)
{
	ID3D11ShaderResourceView*	pSRV = NULL;
	ID3D11SamplerState*			pSmp = NULL;

	if (m_pDifInfo)
	{
		if (m_pDifInfo->pTexture)
		{
			if (iShaderConstantType & SCT_VERTEX)
				CONTEXT->VSSetShaderResources(m_pDifInfo->iRegisterTex, 1, &pSRV);

			if (iShaderConstantType & SCT_PIXEL)
				CONTEXT->PSSetShaderResources(m_pDifInfo->iRegisterTex, 1, &pSRV);
		}
	}

	if (m_pNormalInfo)
	{
		if (m_pNormalInfo->pTexture)
		{
			if (iShaderConstantType & SCT_PIXEL)
				CONTEXT->PSSetShaderResources(m_pNormalInfo->iRegisterTex, 1, &pSRV);
		}
	}

	if (m_pSpecularInfo)
	{
		if (m_pSpecularInfo->pTexture)
		{
			if (iShaderConstantType & SCT_PIXEL)
				CONTEXT->PSSetShaderResources(m_pSpecularInfo->iRegisterTex, 1, &pSRV);
		}
	}

	for (size_t i = 0; i < m_vecMultiTex.size(); ++i)
	{
		if (m_vecMultiTex[i]->pTexture)
		{
			if (iShaderConstantType & SCT_PIXEL)
				CONTEXT->PSSetShaderResources(m_vecMultiTex[i]->iRegisterTex, 1, &pSRV);
		}
	}
}

void CMaterial::SetTexturePathKey(const string & strPathKey)
{
	if (m_pDifInfo)
		m_pDifInfo->pTexture->SetTexturePathKey(strPathKey);

	if (m_pNormalInfo)
		m_pNormalInfo->pTexture->SetTexturePathKey(strPathKey);

	if (m_pSpecularInfo)
		m_pSpecularInfo->pTexture->SetTexturePathKey(strPathKey);

	for (size_t i = 0; i < m_vecMultiTex.size(); ++i)
	{
		m_vecMultiTex[i]->pTexture->SetTexturePathKey(strPathKey);
	}
}

void CMaterial::ChangeTexturePath(const string & strPath)
{
	if (m_pDifInfo)
		m_pDifInfo->pTexture->ChangeTexturePath(strPath);

	if (m_pNormalInfo)
		m_pNormalInfo->pTexture->ChangeTexturePath(strPath);

	if (m_pSpecularInfo)
		m_pSpecularInfo->pTexture->ChangeTexturePath(strPath);

	for (size_t i = 0; i < m_vecMultiTex.size(); ++i)
	{
		m_vecMultiTex[i]->pTexture->ChangeTexturePath(strPath);
	}
}

bool CMaterial::Init()
{
	return true;
}

void CMaterial::Input(float fTime)
{
}

int CMaterial::Update(float fTime)
{
	return 0;
}

int CMaterial::LateUpdate(float fTime)
{
	return 0;
}

void CMaterial::Collision(float fTime)
{
}

void CMaterial::Render(float fTime)
{
}

CMaterial * CMaterial::Clone()
{
	return new CMaterial(*this);
}

void CMaterial::Save(FILE * pFile)
{
	CComponent::Save(pFile);

	// 재질 색상 정보를 저장한다.
	fwrite(&m_tMtrl, sizeof(MATERIAL), 1, pFile);

	bool	bTexInfo = false;

	// 텍스쳐 정보를 저장한다.
	if (m_pDifInfo)
	{
		bTexInfo = true;
		fwrite(&bTexInfo, sizeof(bool), 1, pFile);
		m_pDifInfo->pTexture->Save(pFile);
		fwrite(&m_pDifInfo->iRegisterTex, 4, 1, pFile);

		string	strKey = m_pDifInfo->pSampler->GetKey();
		int	iLength = strKey.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strKey.c_str(), 1, iLength, pFile);

		fwrite(&m_pDifInfo->iRegisterSmp, 4, 1, pFile);
	}

	else
		fwrite(&bTexInfo, sizeof(bool), 1, pFile);

	bTexInfo = false;
	if (m_pNormalInfo)
	{
		bTexInfo = true;
		fwrite(&bTexInfo, sizeof(bool), 1, pFile);
		m_pNormalInfo->pTexture->Save(pFile);
		fwrite(&m_pNormalInfo->iRegisterTex, 4, 1, pFile);

		string	strKey = m_pNormalInfo->pSampler->GetKey();
		int	iLength = strKey.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strKey.c_str(), 1, iLength, pFile);

		fwrite(&m_pNormalInfo->iRegisterSmp, 4, 1, pFile);
	}

	else
		fwrite(&bTexInfo, sizeof(bool), 1, pFile);

	bTexInfo = false;
	if (m_pSpecularInfo)
	{
		bTexInfo = true;
		fwrite(&bTexInfo, sizeof(bool), 1, pFile);
		m_pSpecularInfo->pTexture->Save(pFile);
		fwrite(&m_pSpecularInfo->iRegisterTex, 4, 1, pFile);

		string	strKey = m_pSpecularInfo->pSampler->GetKey();
		int	iLength = strKey.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strKey.c_str(), 1, iLength, pFile);

		fwrite(&m_pSpecularInfo->iRegisterSmp, 4, 1, pFile);
	}

	else
		fwrite(&bTexInfo, sizeof(bool), 1, pFile);

	size_t	iSize = m_vecMultiTex.size();
	fwrite(&iSize, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecMultiTex[i]->pTexture->Save(pFile);
		fwrite(&m_vecMultiTex[i]->iRegisterTex, 4, 1, pFile);

		string	strKey = m_vecMultiTex[i]->pSampler->GetKey();
		int	iLength = strKey.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strKey.c_str(), 1, iLength, pFile);

		fwrite(&m_vecMultiTex[i]->iRegisterSmp, 4, 1, pFile);
	}
}

void CMaterial::Load(FILE * pFile)
{
	CComponent::Load(pFile);

	// 재질 색상 정보를 불러온다.
	fread(&m_tMtrl, sizeof(MATERIAL), 1, pFile);

	// diffuse
	bool	bTexInfo = false;
	fread(&bTexInfo, sizeof(bool), 1, pFile);

	if (bTexInfo)
		m_pDifInfo = LoadTexture(pFile);

	// Normal
	fread(&bTexInfo, sizeof(bool), 1, pFile);

	if (bTexInfo)
		m_pNormalInfo = LoadTexture(pFile);

	// Specular
	fread(&bTexInfo, sizeof(bool), 1, pFile);

	if (bTexInfo)
		m_pSpecularInfo = LoadTexture(pFile);

	size_t	iSize = 0;
	fread(&iSize, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecMultiTex.push_back(LoadTexture(pFile));
	}
}

PTEXTUREINFO CMaterial::LoadTexture(FILE * pFile)
{
	PTEXTUREINFO	pTexInfo = new TEXTUREINFO;

	int	iLength = 0;
	char	strKey[MAX_PATH] = {};
	fread(&iLength, 4, 1, pFile);
	fread(strKey, 1, iLength, pFile);

	char	strPathKey[MAX_PATH] = {};
	fread(&iLength, sizeof(int), 1, pFile);
	fread(strPathKey, sizeof(char), iLength, pFile);

	vector<string>	vecFullPath;

	int	iCount;
	fread(&iCount, 4, 1, pFile);

	for (size_t i = 0; i < iCount; ++i)
	{
		char	strPath[MAX_PATH] = {};
		fread(&iLength, 4, 1, pFile);
		fread(strPath, 1, iLength, pFile);

		vecFullPath.push_back(strPath);
	}

	if (vecFullPath.size() == 1)
	{
		CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, vecFullPath[0].c_str(), strPathKey);

		pTexInfo->pTexture = pTex;
	}

	else
	{
		vector<wstring>	vecPath;

		for (size_t i = 0; i < vecFullPath.size(); ++i)
		{
			wchar_t	strPath[MAX_PATH] = {};
			MultiByteToWideChar(CP_ACP, 0, vecFullPath[i].c_str(), -1,
				strPath, vecFullPath[i].length() * 2);

			vecPath.push_back(strPath);
		}

		CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, vecPath, strPathKey);

		pTexInfo->pTexture = pTex;
	}

	fread(&pTexInfo->iRegisterTex, 4, 1, pFile);

	fread(&iLength, 4, 1, pFile);
	fread(strKey, 1, iLength, pFile);
	strKey[iLength] = 0;
	fread(&pTexInfo->iRegisterSmp, 4, 1, pFile);
	pTexInfo->pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strKey);

	return pTexInfo;
}