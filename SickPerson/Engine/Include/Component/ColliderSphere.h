#pragma once
#include "Collider.h"

Engine_BEGIN

class Engine_DLL CColliderSphere :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CColliderSphere();
	CColliderSphere(const CColliderSphere& collider);
	~CColliderSphere();

private:
	SPHERE	m_tRelativeSphere;
	SPHERE	m_tSphere;

public:
	SPHERE GetSphere()	const;
	void SetSphere(const Vector3& vCenter, float fRadius);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderSphere* Clone();

public:
	virtual bool Collision(CCollider* pDest);
};

Engine_END
