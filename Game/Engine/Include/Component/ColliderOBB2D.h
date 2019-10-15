#pragma once
#include "Collider.h"

Engine_BEGIN

class Engine_DLL CColliderOBB2D :
	public CCollider
{
private:
	friend class CGameObject;

private:
	CColliderOBB2D();
	CColliderOBB2D(const CColliderOBB2D& coll);
	~CColliderOBB2D();

private:
	OBB2DINFO		m_tInfo;
	OBB2DINFO		m_tOriginInfo;

public:
	OBB2DINFO GetInfo()	const;
	OBB2DINFO GetOriginInfo()	const;

public:
	void SetInfo(const Vector3& vCenter, const Vector3& vAxisX,
		const Vector3& vAxisY, float fLengthX, float fLengthY);
	void SetOriginInfo(const Vector3& vCenter, const Vector3& vAxisX,
		const Vector3& vAxisY, float fLengthX, float fLengthY);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CColliderOBB2D* Clone();

public:
	virtual bool Collision(CCollider* pDest);
};

Engine_END
