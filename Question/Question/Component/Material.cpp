#include "Material.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Sampler.h"
#include "../Rendering/ShaderManager.h"

CMaterial::CMaterial()
{
	SetTag("Material");
	SetTypeName("CMaterial");
	SetTypeID<CMaterial>();
	m_eType = CT_MATERIAL;

	m_tMtrl.vDif = Vector4(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.vAmb = Vector4(1.f, 1.f, 1.f, 1.f);
	m_tMtrl.vSpc = Vector4(1.f, 1.f, 1.f, 3.2f);
	m_tMtrl.vEmv = Vector4(0.f, 0.f, 0.f, 0.f);
}

CMaterial::CMaterial(const CMaterial & material)	:
	CComponent(material)
{
	m_tMtrl = material.m_tMtrl;
	m_tDifInfo = material.m_tDifInfo;

	if (m_tDifInfo.pTexture)
		m_tDifInfo.pTexture->AddRef();

	if (m_tDifInfo.pSampler)
		m_tDifInfo.pSampler->AddRef();
}

CMaterial::~CMaterial()
{
	SAFE_RELEASE(m_tDifInfo.pTexture);
	SAFE_RELEASE(m_tDifInfo.pSampler);
}

void CMaterial::SetMaterial(const Vector4 & vDif, const Vector4 & vAmb, const Vector4 & vSpc, 
	const Vector4 & vEmv, float fPower)
{
	m_tMtrl.vDif = vDif;
	m_tMtrl.vAmb = vAmb;
	m_tMtrl.vSpc = vSpc;
	m_tMtrl.vEmv = vEmv;
	m_tMtrl.vSpc.w = fPower;
}

void CMaterial::SetDiffuseTexInfo(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp,
	const TCHAR * pFileName, 
	const string & strPathKey)
{
	SAFE_RELEASE(m_tDifInfo.pTexture);
	SAFE_RELEASE(m_tDifInfo.pSampler);

	if (!pFileName)
		m_tDifInfo.pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_tDifInfo.pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strDifKey, pFileName, strPathKey);

	m_tDifInfo.pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_tDifInfo.iRegisterTex = iRegTex;
	m_tDifInfo.iRegisterSmp = iRegSmp;
}

void CMaterial::SetDiffuseTexInfo(const string & strSmpKey,
	const string & strDifKey, int iRegTex, int iRegSmp, 
	const vector<wstring>* vecFileName, const string & strPathKey)
{
	SAFE_RELEASE(m_tDifInfo.pTexture);
	SAFE_RELEASE(m_tDifInfo.pSampler);

	if (!vecFileName)
		m_tDifInfo.pTexture = GET_SINGLE(CResourcesManager)->FindTexture(strDifKey);

	else
		m_tDifInfo.pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strDifKey, *vecFileName, strPathKey);

	m_tDifInfo.pSampler = GET_SINGLE(CResourcesManager)->FindSampler(strSmpKey);

	m_tDifInfo.iRegisterTex = iRegTex;
	m_tDifInfo.iRegisterSmp = iRegSmp;
}

void CMaterial::SetDiffuseTex(CTexture * pTex)
{
	SAFE_RELEASE(m_tDifInfo.pTexture);

	pTex->AddRef();
	m_tDifInfo.pTexture = pTex;
}

void CMaterial::SetDiffuseTex(const string & strKey)
{
	SAFE_RELEASE(m_tDifInfo.pTexture);

	CTexture* pTex = GET_SINGLE(CResourcesManager)->FindTexture(strKey);

	m_tDifInfo.pTexture = pTex;
}

void CMaterial::SetDiffuseTex(const string & strKey, const char * pFullPath)
{
	SAFE_RELEASE(m_tDifInfo.pTexture);

	CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strKey, pFullPath);

	m_tDifInfo.pTexture = pTex;
}

void CMaterial::SetDiffuseTex(const string & strKey, const vector<string>& vecFullPath)
{
	SAFE_RELEASE(m_tDifInfo.pTexture);

	CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTextureFromFullPath(strKey, vecFullPath);

	m_tDifInfo.pTexture = pTex;
}

void CMaterial::SetMaterial(int iShaderConstantType)
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Material", &m_tMtrl, iShaderConstantType);

	if (m_tDifInfo.pTexture)
	{
		m_tDifInfo.pTexture->SetTexture(m_tDifInfo.iRegisterTex, iShaderConstantType);
		m_tDifInfo.pSampler->SetSampler(m_tDifInfo.iRegisterSmp, iShaderConstantType);
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