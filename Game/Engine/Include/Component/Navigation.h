#pragma once
#include "Component.h"

Engine_BEGIN

typedef struct Engine_DLL _tagOpenNode
{
	class CTile*	pTile;
	int				iIndex;
}OPENNODE, *POPENNODE;

class Engine_DLL CNavigation :
	public CComponent
{
protected:
	friend class CGameObject;

protected:
	CNavigation();
	CNavigation(const CNavigation& Nav);
	~CNavigation();

private:
	vector<class CTile*> m_vecTile;
	TERRAIN2D_TILE_TYPE		m_eTileType;
	unsigned int			m_iTileNumX;
	unsigned int			m_iTileNumY;
	unsigned int			m_iTileSizeX;
	unsigned int			m_iTileSizeY;
	vector<OPENNODE>		m_vecOpen;
	vector<int>				m_vecClose;
	list<int>				m_Road;
	Vector3					m_vEndPos;
	class CAnimation2D*		m_pAnimation;
	string		m_strMoveAnimationKey;

public:
	void SetMoveAnimationKey(const string& strKey);

private:
	void FindTileSet();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CNavigation* Clone();
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);

public:
	int GetTileIndex(const Vector3& vPos);
	int GetTileIndex(const Vector2& vPos);
	int GetTileIndex(const POINT& ptPos);
	int GetTileIndex(float x, float y);
	int GetTileIndexX(float x, float y);
	int GetTileIndexY(float x, float y);
	class CTile* GetTile(float x, float y);

public:
	void FindRoad(const Vector2& vEndPos);
	void FindRoad(const Vector3& vEndPos);

private:
	void GetOpenListIndex(int iIndex[8], int x, int y);

public:
	static bool Sort(OPENNODE& p1, OPENNODE& p2);
};

Engine_END
