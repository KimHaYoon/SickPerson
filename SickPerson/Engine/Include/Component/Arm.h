#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CArm :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CArm();
	CArm(const CArm& arm);
	~CArm();

private:
	CTransform*		m_pTarget;
	Vector3			m_vDist;
	Vector3			m_vLookAtDist;
	float			m_fMinDist;
	float			m_fMaxDist;
	bool			m_bZoom;

public:
	void SetTarget(class CTransform* pTarget);
	void SetTarget(class CGameObject* pTarget);
	void SetDist(const Vector3& vDist);
	void SetLookAtDist(const Vector3& vLookAtDist);
	void SetMinDist(float fMin);
	void SetMaxDist(float fMax);
	void Zoom(const Vector3& vDir, float fSpeed);
	void EnableZoom(bool bZoom);
	void RotationY(float fAngle);
	void RotationAxis(const Vector3& vRotAxis, float fAngle);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CArm* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

Engine_END
