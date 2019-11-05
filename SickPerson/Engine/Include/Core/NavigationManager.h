#pragma once

#include "../Engine.h"

Engine_BEGIN

typedef struct Engine_DLL _tagLandScapeInfo
{
	Vector3		vMin;
	Vector3		vMax;
	int			iNumX;
	int			iNumY;
	vector<Vector3>*	pvecPos;
	class CScene*	pScene;
	class CTransform*	pTransform;
}LANDSCAPEINFO, *PLANDSCAPEINFO;

class Engine_DLL CNavigationManager
{
private:
	unordered_map<string, PLANDSCAPEINFO>	m_mapLandScapeInfo;

public:
	void AddLandScapeInfo(const string& strKey, int iNumX, int iNumY,
		class CScene* pScene, class CTransform* pTransform,
		vector<Vector3>* pvecPos);
	void DeleteLandScapeInfo(class CScene* pScene);
	PLANDSCAPEINFO FindLandScapeInfo(const string& strKey);
	PLANDSCAPEINFO FindLandScapeInfo(const Vector3& vPos);
	float GetY(const Vector3& vPos);

	DECLARE_SINGLE(CNavigationManager)
};

Engine_END