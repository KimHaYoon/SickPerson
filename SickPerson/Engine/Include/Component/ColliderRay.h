#pragma once
#include "Collider.h"

Engine_BEGIN

class Engine_DLL CColliderRay :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CColliderRay();
	CColliderRay(const CColliderRay& collider);
	~CColliderRay();

private:
	RAY	m_tRay;
	AXIS	m_eAxis;

public:
	RAY GetRay()	const;
	PRAY GetRayAddress();

public:
	void SetPos(const Vector3& vPos);
	void SetDir(const Vector3& vDir);
	void SetDir(AXIS eAxis);
	void SetDistance(float fDist);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderRay* Clone();

public:
	virtual bool Collision(CCollider* pDest);
};

Engine_END
