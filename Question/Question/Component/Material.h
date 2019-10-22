#pragma once
#include "Component.h"

typedef struct _tagTextureInfo
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

class CMaterial :
	public CComponent
{
private:
	friend class CRenderer;
	friend class CRenderer2D;

private:
	CMaterial();
	CMaterial(const CMaterial& material);
	~CMaterial();

private:
	MATERIAL		m_tMtrl;
	TEXTUREINFO		m_tDifInfo;

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
	void SetDiffuseTex(class CTexture* pTex);
	void SetDiffuseTex(const string& strKey);
	void SetDiffuseTex(const string& strKey, const char* pFullPath);
	void SetDiffuseTex(const string& strKey, const vector<string>& vecFullPath);
	void SetMaterial(int iShaderConstantType);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CMaterial* Clone();
};