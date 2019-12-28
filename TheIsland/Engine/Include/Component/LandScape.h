#pragma once
#include "Component.h"

GAME_BEGIN

class GAME_DLL CLandScape :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CLandScape();
	CLandScape(const CLandScape& landscape);
	~CLandScape();

private:
	int		m_iNumX;
	int		m_iNumZ;
	vector<Vector3>	m_vecPos;
	vector<Vector3>	m_vecFaceNormal;
	int		m_iDetailLevel;
	int		m_iSplatCount;

public:
	void SetDetailLevel(int iDetailLevel);
	bool CreateLandScape(const string& strMeshKey, int iVtxCount, bool bBump, 
		const string& strTexKey, const wchar_t* pFileName, 
		const wchar_t* pNormalName, const wchar_t* pSpecularName,
		const char* pHeightMap = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetDiffuseSplatting(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetNormalSplatting(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetSpecularSplatting(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetSplattingAlpha(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLandScape* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void ComputeNormal(vector<VERTEXBUMP>& vecVtx, const vector<UINT>& vecIdx);
	void ComputeTangent(vector<VERTEXBUMP>& vecVtx, const vector<UINT>& vecIdx);
};

GAME_END
