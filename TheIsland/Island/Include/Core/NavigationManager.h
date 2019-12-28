#pragma once

#include "../Engine.h"

GAME_BEGIN

typedef struct _tagLandScapeInfo
{
	Vector3		vMin;
	Vector3		vMax;
	int			iNumX;
	int			iNumY;
	vector<Vector3>*	pvecPos;
	class CScene*	pScene;
	class CTransform*	pTransform;
}LANDSCAPEINFO, *PLANDSCAPEINFO;

class GAME_DLL CNavigationManager
{
private:
	unordered_map<string, PLANDSCAPEINFO>	m_mapLandScapeInfo;
	unordered_map<string, class CNavigationMesh*>	m_mapNaviMesh;

public:
	void AddLandScapeInfo(const string& strKey, int iNumX, int iNumY,
		class CScene* pScene, class CTransform* pTransform, 
		vector<Vector3>* pvecPos);
	void DeleteLandScapeInfo(class CScene* pScene);
	PLANDSCAPEINFO FindLandScapeInfo(const string& strKey);
	PLANDSCAPEINFO FindLandScapeInfo(const Vector3& vPos);
	float GetY(const Vector3& vPos);
	CNavigationMesh* CreateNavigationMesh(const string& strKey);
	CNavigationMesh* FindNavigationMesh(const string& strKey);
	CNavigationMesh* FindNavigationMesh(const Vector3& vPos);
	const list<Vector3>* FindPath(const Vector3& vStart, const Vector3& vEnd);
	Vector3 IntersectCell(const Vector3& vOrigin, const Vector3& vDir);

	DECLARE_SINGLE(CNavigationManager)
};

GAME_END
