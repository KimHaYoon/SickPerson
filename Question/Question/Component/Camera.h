#pragma once
#include "Component.h"

class  CCamera :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CCamera();
	CCamera(const CCamera& camera);
	~CCamera();

private:
	CAMERA_TYPE			m_eCameraType;
	class CTransform*	m_pAttach;
	Vector3				m_vPrevPos;
	RESOLUTION			m_tWorldRS;
	Vector2				m_vPivot;
	class CFrustum*		m_pFrustum;

public:
	void SetPivot(float x, float y);
	void SetPivot(const Vector2& vPivot);
	void SetWorldResolution(UINT x, UINT y);
	void SetWorldResolution(const RESOLUTION& tRS);
	void SetAttach(class CGameObject* pAttach);
	void SetAttach(class CComponent* pAttach);
	bool FrustumInPoint(const Vector3& vPos);
	bool FrustumInSphere(const Vector3& vCenter, float fRadius);
	bool FrustumInSphere(const SPHERE& tSphere);

private:
	PMatrix		m_matView;
	PMatrix		m_matProj;

public:
	Matrix GetViewMatrix()	const;
	Matrix GetProjMatrix()	const;

public:
	void SetOrthoProj(const RESOLUTION& tRS, float fNear, float fFar);
	void SetPerspectiveProj(float fViewAngle, float fAspect,
		float fNear, float fFar);

private:
	void ComputeOrthoView();
	void ComputePerspectiveView();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CCamera* Clone();
};
