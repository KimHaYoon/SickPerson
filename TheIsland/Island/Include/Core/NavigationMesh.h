#pragma once

#include "../Engine.h"

GAME_BEGIN

enum NAVIMESH_SELL_LIST_TYPE
{
	NSLT_NONE,
	NSLT_OPEN,
	NSLT_CLOSE
};

typedef struct GAME_DLL _tagAdjInfo
{
	int	iIndex;
	int	iEdgeIndex;
}ADJINFO, *PADJINFO;

class GAME_DLL CNavigationCell
{
private:
	friend class CNavigationMesh;

private:
	CNavigationCell();

public:
	~CNavigationCell();

private:
	NAVIMESH_SELL_LIST_TYPE	m_eType;
	Vector3		m_vPos[3];
	Vector3		m_vEdge[3];
	Vector3		m_vEdgeCenter[3];
	Vector3		m_vCenter;
	vector<ADJINFO>	m_vecAdj;
	int			m_iIndex;
	int			m_iParentIdx;
	float		m_fG;
	float		m_fH;
	float		m_fTotal;
};

class GAME_DLL CNavigationMesh
{
private:
	friend class CNavigationManager;

private:
	CNavigationMesh();

public:
	~CNavigationMesh();

private:
	vector<CNavigationCell*>	m_vecCell;
	// ¿­¸°¸ñ·Ï, ´ÝÈù¸ñ·Ï
	vector<CNavigationCell*>	m_OpenList;
	vector<CNavigationCell*>	m_CloseList;
	stack<int>					m_Path;
	list<Vector3>				m_PathList;
	Vector3		m_vMin;
	Vector3		m_vMax;
	bool		m_bFindEnd;

public:
	Vector3 GetMin()	const;
	Vector3 GetMax()	const;
	const list<Vector3>* GetPathList()	const;

public:
	bool Init();
	void AddCell(const Vector3 vPos[3]);
	void AddAdj(int iCellIdx, int iAdjIdx);
	void CreateGridMapAdj(int iLineRectCount);
	void CreateAdj();

public:
	void FindPath(const Vector3& vStart, const Vector3& vEnd);

private:
	void AddOpenList(CNavigationCell* pCell, CNavigationCell* pEndCell,
		const Vector3& vEnd, const Vector3& vStart);
	CNavigationCell* FindGridNavigationCell(const Vector3& vPos);
	CNavigationCell* FindNavigationCell(const Vector3& vPos);
	bool CheckOnEdge(int iSrc, int iDest, const Vector3& vOrigin1, const Vector3& vOrigin2,
		const Vector3& vEdge, float fEdgeLength, int iEdge1, int iEdge2);
	bool CheckPathDir(const Vector3& vSrc1, const Vector3& vSrc2,
		const Vector3& vDest1, const Vector3& vDest2, Vector3& vIntersect);

public:
	void Save(const char* pFileName, const string& strPathKey = DATA_PATH);
	void SaveFromFullPath(const char* pFullPath);
	void Load(const char* pFileName, const string& strPathKey = DATA_PATH);
	void LoadFromFullPath(const char* pFullPath);

public:
	float GetY(const Vector3& vPos);
	Vector3 IntersectCell(const Vector3& vOrigin, const Vector3& vDir);

private:
	bool RayIntersectTriangle(Vector3 rayOrigin, Vector3 rayDir, Vector3 v0, 
		Vector3 v1, Vector3 v2, float& t, Vector3& vIntersect);

public:
	static bool Sort(CNavigationCell* pSrc, CNavigationCell* pDest);
};

GAME_END
