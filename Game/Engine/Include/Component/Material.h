#pragma once
#include "Component.h"

Engine_BEGIN

typedef struct Engine_DLL _tagTextureInfo
{
	class CTexture*		pTexture;
	int					iRegisterTex;
	class CSampler*		pSampler;
	int					iRegisterSmp;

	_tagTextureInfo() :
		pTexture(NULL),
		pSampler(NULL),
		iRegisterTex(0),
		iRegisterSmp(0)
	{
	}
}TEXTUREINFO, *PTEXTUREINFO;

class Engine_DLL CMaterial :
	public CComponent
{
private:
	friend class CRenderer;
	friend class CRenderer2D;
	friend class CMesh;

private:
	CMaterial();
	CMaterial(const CMaterial& material);
	~CMaterial();

private:
	MATERIAL		m_tMtrl;
	PTEXTUREINFO	m_pDifInfo;
	PTEXTUREINFO	m_pNormalInfo;
	PTEXTUREINFO	m_pSpecularInfo;
	vector<PTEXTUREINFO>	m_vecMultiTex;

public:
	void SetMaterial(const Vector4& vDif, const Vector4& vAmb, const Vector4& vSpc,
		const Vector4& vEmv, float fPower);
	void SetDiffuseTexInfo(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void SetDiffuseTexInfo(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const vector<wstring>* vecFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void SetDiffuseTexInfoFromFullPath(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const char* pFullPath = NULL);
	void SetDiffuseTex(class CTexture* pTex);
	void SetDiffuseTex(const string& strKey);
	void SetDiffuseTex(const string& strKey, const char* pFullPath);
	void SetDiffuseTex(const string& strKey, const vector<string>& vecFullPath);
	void SetDiffuseTexRegister(int iRegister);
	void SetDiffuseSampler(const string& strKey);

	void SetNormalTexInfo(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void SetNormalTexInfoFromFullPath(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const char* pFullPath = NULL);

	void SetSpecularTexInfo(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void SetSpecularTexInfoFromFullPath(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const char* pFullPath = NULL);

	void AddMultiTexture(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const TCHAR* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void AddMultiTexture(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void AddMultiTexture(const string& strSmpKey,
		ID3D12ShaderResourceView* pSRV,
		int iRegTex = 0, int iRegSmp = 0);

	void SetMaterial(int iShaderConstantType);
	void ResetMaterial(int iShaderConstantType);
	void SetTexturePathKey(const string& strPathKey);
	void ChangeTexturePath(const string& strPath);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CMaterial* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	PTEXTUREINFO LoadTexture(FILE* pFile);
};

Engine_END