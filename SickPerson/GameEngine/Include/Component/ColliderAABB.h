#pragma once
#include "Collider.h"

Engine_BEGIN

class Engine_DLL CColliderAABB :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CColliderAABB();
	CColliderAABB(const CColliderAABB& collider);
	~CColliderAABB();

private:
	AABB	m_tRelativeInfo;
	AABB	m_tInfo;

public:
	AABB GetInfo()	const;
	AABB GetRelativeInfo()	const;

public:
	void SetAABB(const Vector3& vMin, const Vector3& vMax);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderAABB* Clone();

public:
	virtual bool Collision(CCollider* pDest);
};

Engine_END
