#pragma once
#include "Collider.h"

Engine_BEGIN

class Engine_DLL CColliderRect :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CColliderRect();
	CColliderRect(const CColliderRect& coll);
	~CColliderRect();

private:
	RECTINFO		m_tInfo;

public:
	void SetRect(float l, float t, float w, float h);

public:
	RECTINFO GetInfo()	const;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderRect* Clone();

public:
	virtual bool Collision(CCollider* pDest);
};

Engine_END
