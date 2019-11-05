#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CRenderer2D :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CRenderer2D();
	CRenderer2D(const CRenderer2D& renderer);
	~CRenderer2D();

private:
	class CMesh*	m_pMesh;
	class CShader*	m_pShader;
	ID3D11InputLayout*	m_pLayout;
	string				m_strLayoutKey;
	class CMaterial*	m_pMaterial;
	class CRenderState*	m_pRenderState[RST_END];
	unordered_map<int, PRENDERERCBUFFER>	m_mapCBuffer;

public:
	class CMaterial* GetMaterial()	const;
	bool CreateCBuffer(const string& strName, int iRegister,
		int iSize, int iShaderType);
	PRENDERERCBUFFER FindCBuffer(int iRegister);
	void UpdateCBuffer(const string& strName, int iRegister,
		int iSize, int iShaderType, void* pData);

public:
	void SetMesh(const string& strKey);
	void SetMesh(class CMesh* pMesh);
	void SetShader(const string& strKey);
	void SetShader(class CShader* pShader);
	void SetInputLayout(const string& strKey);
	void SetInputLayout(ID3D11InputLayout* pLayout);
	class CMaterial* CreateMaterial(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const wchar_t* pFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	class CMaterial* CreateMaterial(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const vector<wstring>* vecFileName = NULL,
		const string& strPathKey = TEXTURE_PATH);
	void SetRenderState(const string& strKey);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CRenderer2D* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void UpdateTransform();
};

Engine_END