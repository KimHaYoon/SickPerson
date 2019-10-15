#pragma once
#include "Component.h"

class  CTransform :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CTransform();
	CTransform(const CTransform& transform);
	~CTransform();

private:
	int		m_iParentModify;
	CTransform*	m_pParent;

public:
	CTransform* GetParent()	const;
	void SetParentModifyType(int iModifyType);

	// ============= Local Area =================
private:
	Vector3		m_vPivot;
	Vector3		m_vLocalScale;
	Vector3		m_vLocalRot;
	Vector3		m_vLocalPos;
	Vector3		m_vLocalAxis[AXIS_END];

	PMatrix		m_matLocalScale;
	PMatrix		m_matLocalRotX;
	PMatrix		m_matLocalRotY;
	PMatrix		m_matLocalRotZ;
	PMatrix		m_matLocalRot;
	PMatrix		m_matLocalPos;
	PMatrix		m_matLocal;

public:
	Vector3 GetPivot()	const;
	Vector3 GetLocalScale()	const;
	Vector3 GetLocalRot()	const;
	Vector3 GetLocalPos()	const;
	Vector3 GetLocalAxis(AXIS axis)	const;

	Matrix GetLocalScaleMatrix()	const;
	Matrix GetLocalRotMatrix()	const;
	Matrix GetLocalPosMatrix()	const;
	Matrix GetLocalMatrix()	const;

public:
	void SetPivot(float x, float y, float z);
	void SetPivot(float f[3]);
	void SetPivot(const Vector3& v);
	void SetPivot(const XMVECTOR& v);
	void SetLocalScale(float x, float y, float z);
	void SetLocalScale(float f[3]);
	void SetLocalScale(const Vector3& v);
	void SetLocalScale(const XMVECTOR& v);
	void SetLocalRot(float x, float y, float z);
	void SetLocalRot(float f[3]);
	void SetLocalRot(const Vector3& v);
	void SetLocalRot(const XMVECTOR& v);
	void SetLocalRotX(float x);
	void SetLocalRotY(float y);
	void SetLocalRotZ(float z);
	void SetLocalPos(float x, float y, float z);
	void SetLocalPos(float f[3]);
	void SetLocalPos(const Vector3& v);
	void SetLocalPos(const XMVECTOR& v);

	// ============= World Area =================
private:
	Vector3		m_vWorldScale;
	Vector3		m_vWorldRelativeScale;
	Vector3		m_vWorldRot;
	Vector3		m_vWorldRelativeRot;
	Vector3		m_vWorldPos;
	Vector3		m_vWorldRelativePos;
	Vector3		m_vWorldAxis[AXIS_END];	// รเ
	AXIS		m_eOriginAxis;

	PMatrix		m_matWorldScale;
	PMatrix		m_matWorldRotX;
	PMatrix		m_matWorldRotY;
	PMatrix		m_matWorldRotZ;
	PMatrix		m_matWorldRot;
	PMatrix		m_matWorldPos;
	PMatrix		m_matParent;
	PMatrix		m_matWorld;

public:
	Vector3 GetWorldScale()	const;
	Vector3 GetWorldRot()	const;
	Vector3 GetWorldPos()	const;
	Vector3 GetWorldAxis(AXIS axis)	const;
	Vector3 GetParentScale()	const;
	Vector3 GetParentRot()	const;
	Vector3	GetParentPos()	const;

	Matrix GetWorldScaleMatrix()	const;
	Matrix GetWorldRotMatrix()	const;
	Matrix GetWorldPosMatrix()	const;
	Matrix GetParentMatrix()	const;
	Matrix GetWorldMatrix()	const;

public:
	void SetOriginAxis(AXIS eAxis);
	void SetWorldScale(float x, float y, float z);
	void SetWorldScale(float f[3]);
	void SetWorldScale(const Vector3& v);
	void SetWorldScale(const XMVECTOR& v);
	void SetWorldRot(float x, float y, float z);
	void SetWorldRot(float f[3]);
	void SetWorldRot(const Vector3& v);
	void SetWorldRot(const XMVECTOR& v);
	void SetWorldRotX(float x);
	void SetWorldRotY(float y);
	void SetWorldRotZ(float z);
	void SetWorldPos(float x, float y, float z, bool bSelf = false);
	void SetWorldPos(float f[3], bool bSelf = false);
	void SetWorldPos(const Vector3& v, bool bSelf = false);
	void SetWorldPos(const XMVECTOR& v, bool bSelf = false);
	void SetWorldPosY(float y, bool bSelf = false);
	void SetWorldAxis(const Vector3& vAxis, AXIS eAxis);
	void ComputeWorldRotationMatrix(Vector3 vAxis[AXIS_END]);

public:
	void MoveLocal(const Vector3& vMove);
	void MoveLocal(const Vector3& vDir, float fDist);
	void MoveLocal(const Vector3& vDir, float fSpeed, float fTime);
	void MoveLocal(AXIS axis, float fDist);
	void MoveLocal(AXIS axis, float fSpeed, float fTime);

	void MoveWorld(const Vector3& vMove);
	void MoveWorld(const Vector3& vDir, float fDist);
	void MoveWorld(const Vector3& vDir, float fSpeed, float fTime);
	void MoveWorld(AXIS axis, float fDist);
	void MoveWorld(AXIS axis, float fSpeed, float fTime);

public:
	void RotateLocal(const Vector3& vRot);
	void RotateLocal(const Vector3& vRot, float fTime);
	void RotateLocalX(float fX);
	void RotateLocalX(float fX, float fTime);
	void RotateLocalY(float fY);
	void RotateLocalY(float fY, float fTime);
	void RotateLocalZ(float fZ);
	void RotateLocalZ(float fZ, float fTime);

	void RotateWorld(const Vector3& vRot);
	void RotateWorld(const Vector3& vRot, float fTime);
	void RotateWorldX(float fX);
	void RotateWorldX(float fX, float fTime);
	void RotateWorldY(float fY);
	void RotateWorldY(float fY, float fTime);
	void RotateWorldZ(float fZ);
	void RotateWorldZ(float fZ, float fTime);

public:
	void ComputeLocalAxis();
	void ComputeWorldAxis();
	void ComputeWorldAxisRot();
	void LookAt(class CGameObject* pObj);
	void LookAt(CTransform* pTransform);
	void LookAt(const Vector3& vPos);

private:
	bool	m_bUpdate;
	bool	m_bStatic;

public:
	void Static(bool bStatic);
	void UpdateHierarchy();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CTransform* Clone();
};