#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CRenderer :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CRenderer();
	CRenderer(const CRenderer& renderer);
	~CRenderer();

private:
	class CMesh*	m_pMesh;
	class CShader*	m_pShader;
	class CShader*	m_pForwardShader;
	ID3D11InputLayout*	m_pInputLayout;
	vector<vector<class CMaterial*>>	m_vecMaterials;
	unordered_map<int, PRENDERERCBUFFER>	m_mapCBuffer;
	class CRenderState*	m_pRenderState[RST_END];
	bool	m_bAlpha;

public:
	class CMaterial* GetMaterial(int iContainer = 0, int iSubset = 0)	const;
	bool GetAlphaEnable()	const;
	void AlphaEnable(bool bAlpha);
	class CMesh* GetMesh()	const;
	bool CreateCBuffer(const string& strName, int iRegister,
		int iSize, int iShaderType);
	PRENDERERCBUFFER FindCBuffer(int iRegister);
	void UpdateCBuffer(const string& strName, int iRegister,
		int iSize, int iShaderType, void* pData);

public:
	void SetMesh(const string& strKey);
	void SetMesh(class CMesh* pMesh);
	void SetMesh(const string& strKey, const wchar_t* pFileName,
		const string& strPathKey = MESH_PATH);
	void SetMeshFromFullPath(const string& strKey, const wchar_t* pFullPath);
	void SetShader(const string& strKey);
	void SetShader(class CShader* pShader);
	void SetForwardShader(const string& strKey);
	void SetForwardShader(class CShader* pShader);
	void SetInputLayout(const string& strKey);
	void SetInputLayout(ID3D11InputLayout* pLayout);
	class CMaterial* CreateMaterial(int iContainer = 0);
	void SetRenderState(const string& strKey);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CRenderer* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void UpdateTransform();

public:
	void RenderForward(float fTime);
};

Engine_END