#pragma once

#include "../Engine.h"

Engine_BEGIN

enum COLLLIST_TYPE
{
	CLT_2D,
	CLT_3D
};

typedef struct Engine_DLL _tagCollisionSection
{
	class CCollider**	pArray;
	int			iSize;
	int			iCapasity;
	COLLLIST_TYPE	eType;
	Vector3		vMin;
	Vector3		vMax;

	_tagCollisionSection() :
		iSize(0),
		iCapasity(1000),
		eType(CLT_3D)
	{
		pArray = new CCollider*[iCapasity];
		memset(pArray, 0, sizeof(CCollider*) * iCapasity);
	}

	~_tagCollisionSection()
	{
		delete[]	pArray;
	}
}COLLISIONSECTION, *PCOLLISIONSECTION;

typedef struct Engine_DLL _tagCollisionGroup
{
	PCOLLISIONSECTION*	pSectionList;
	int			iSectionX;
	int			iSectionY;
	int			iSectionZ;
	int			iSectionMax;
	Vector3		vSize;
	Vector3		vMin;
	Vector3		vMax;

	_tagCollisionGroup() :
		iSectionX(1),
		iSectionY(1),
		iSectionZ(1),
		vSize(1000.f, 1000.f, 1000.f)
	{
	}

	~_tagCollisionGroup()
	{
		for (int i = 0; i < iSectionZ; ++i)
		{
			for (int j = 0; j < iSectionY; ++j)
			{
				for (int k = 0; k < iSectionX; ++k)
				{
					int iIndex = i * iSectionX * iSectionY + j * iSectionX + k;

					delete	pSectionList[iIndex];
				}
			}
		}

		delete[]	pSectionList;
	}
}COLLISIONGROUP, *PCOLLISIONGROUP;

class Engine_DLL CCollisionManager
{
private:
	//vector<class CGameObject*>	m_CollList[CLT_END];
	unordered_map<string, PCOLLISIONGROUP>	m_mapCollisionGroup;
	class CCollider**	m_pMouseCollArray;
	int					m_iMouseCollCount;
	int					m_iMouseCollCapasity;

public:
	bool CreateGroup(const string& strKey, int iSectionX, int iSectionY,
		int iSectionZ, const Vector3& vSize, COLLLIST_TYPE eType = CLT_3D);
	bool ModifyGroup(const string& strKey, int iSectionX, int iSectionY,
		int iSectionZ, const Vector3& vSize, COLLLIST_TYPE eType = CLT_3D);
	bool EraseGroup(const string& strKey);

private:
	PCOLLISIONGROUP FindGroup(const string& strKey);

public:
	bool Init();
	bool AddCollider(class CGameObject* pObj);
	void Collision(float fTime);

public:
	static bool SortZOrder(class CCollider* pSrc, class CCollider* pDest);
	static bool SortZ(class CCollider* pSrc, class CCollider* pDest);

	DECLARE_SINGLE(CCollisionManager)
};

Engine_END