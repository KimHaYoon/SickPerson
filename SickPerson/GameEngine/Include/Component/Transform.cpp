#include "Transform.h"
#include "../GameObject/GameObject.h"

Engine_USING

CTransform::CTransform() :
	m_matLocalScale(NULL),
	m_matLocalRotX(NULL),
	m_matLocalRotY(NULL),
	m_matLocalRotZ(NULL),
	m_matLocalRot(NULL),
	m_matLocalPos(NULL),
	m_matLocal(NULL),
	m_matWorldScale(NULL),
	m_matWorldRotX(NULL),
	m_matWorldRotY(NULL),
	m_matWorldRotZ(NULL),
	m_matWorldRot(NULL),
	m_matWorldPos(NULL),
	m_matParent(NULL),
	m_matWorld(NULL),
	m_pParent(NULL),
	m_eOriginAxis(AXIS_Z),
	m_bUpdate(true),
	m_iParentModify(PMT_NONE)
{
	SetTag("Transform");
	SetTypeName("CTransform");
	SetTypeID<CTransform>();
	m_eType = CT_TRANSFORM;
}

CTransform::CTransform(const CTransform & transform) :
	CComponent(transform)
{
	*this = transform;

	m_matLocalScale = new Matrix;
	m_matLocalRotX = new Matrix;
	m_matLocalRotY = new Matrix;
	m_matLocalRotZ = new Matrix;
	m_matLocalRot = new Matrix;
	m_matLocalPos = new Matrix;
	m_matLocal = new Matrix;

	m_matWorldScale = new Matrix;
	m_matWorldRotX = new Matrix;
	m_matWorldRotY = new Matrix;
	m_matWorldRotZ = new Matrix;
	m_matWorldRot = new Matrix;
	m_matWorldPos = new Matrix;
	m_matParent = new Matrix;
	m_matWorld = new Matrix;

	*m_matLocalScale = *transform.m_matLocalScale;
	*m_matLocalRotX = *transform.m_matLocalRotX;
	*m_matLocalRotY = *transform.m_matLocalRotY;
	*m_matLocalRotZ = *transform.m_matLocalRotZ;
	*m_matLocalRot = *transform.m_matLocalRot;
	*m_matLocalPos = *transform.m_matLocalPos;
	*m_matLocal = *transform.m_matLocal;

	*m_matWorldScale = *transform.m_matWorldScale;
	*m_matWorldRotX = *transform.m_matWorldRotX;
	*m_matWorldRotY = *transform.m_matWorldRotY;
	*m_matWorldRotZ = *transform.m_matWorldRotZ;
	*m_matWorldRot = *transform.m_matWorldRot;
	*m_matWorldPos = *transform.m_matWorldPos;
	*m_matWorld = *transform.m_matWorld;

	m_bUpdate = true;
}

CTransform::~CTransform()
{
	SAFE_DELETE(m_matLocalScale);
	SAFE_DELETE(m_matLocalRotX);
	SAFE_DELETE(m_matLocalRotY);
	SAFE_DELETE(m_matLocalRotZ);
	SAFE_DELETE(m_matLocalRot);
	SAFE_DELETE(m_matLocalPos);
	SAFE_DELETE(m_matLocal);

	SAFE_DELETE(m_matWorldScale);
	SAFE_DELETE(m_matWorldRotX);
	SAFE_DELETE(m_matWorldRotY);
	SAFE_DELETE(m_matWorldRotZ);
	SAFE_DELETE(m_matWorldRot);
	SAFE_DELETE(m_matWorldPos);
	SAFE_DELETE(m_matParent);
	SAFE_DELETE(m_matWorld);
}

CTransform * CTransform::GetParent() const
{
	return m_pParent;
}

void CTransform::SetParentModifyType(int iModifyType)
{
	m_iParentModify = iModifyType;
}

Vector3 CTransform::GetPivot() const
{
	return m_vPivot;
}

Vector3 CTransform::GetLocalScale() const
{
	return m_vLocalScale;
}

Vector3 CTransform::GetLocalRot() const
{
	return m_vLocalRot;
}

Vector3 CTransform::GetLocalPos() const
{
	return m_vLocalPos;
}

Vector3 CTransform::GetLocalAxis(AXIS axis) const
{
	return m_vLocalAxis[axis];
}

Matrix CTransform::GetLocalScaleMatrix() const
{
	return *m_matLocalScale;
}

Matrix CTransform::GetLocalRotMatrix() const
{
	return *m_matLocalRot;
}

Matrix CTransform::GetLocalPosMatrix() const
{
	return *m_matLocalPos;
}

Matrix CTransform::GetLocalMatrix() const
{
	return *m_matLocal;
}

void CTransform::SetPivot(float x, float y, float z)
{
	m_vPivot = Vector3(x, y, z);
}

void CTransform::SetPivot(float f[3])
{
	m_vPivot = Vector3(f[0], f[1], f[2]);
}

void CTransform::SetPivot(const Vector3 & v)
{
	m_vPivot = v;
}

void CTransform::SetPivot(const XMVECTOR & v)
{
	XMStoreFloat3(&m_vPivot, v);
}

void CTransform::SetLocalScale(float x, float y, float z)
{
	m_vLocalScale = Vector3(x, y, z);

	*m_matLocalScale = XMMatrixScaling(x, y, z);

	UpdateHierarchy();
}

void CTransform::SetLocalScale(float f[3])
{
	m_vLocalScale = Vector3(f[0], f[1], f[2]);

	*m_matLocalScale = XMMatrixScaling(f[0], f[1], f[2]);

	UpdateHierarchy();
}

void CTransform::SetLocalScale(const Vector3 & v)
{
	m_vLocalScale = Vector3(v.x, v.y, v.z);

	*m_matLocalScale = XMMatrixScaling(v.x, v.y, v.z);

	UpdateHierarchy();
}

void CTransform::SetLocalScale(const XMVECTOR & v)
{
	Vector3	v1;
	XMStoreFloat3(&v1, v);

	m_vLocalScale = Vector3(v1.x, v1.y, v1.z);

	*m_matLocalScale = XMMatrixScaling(v1.x, v1.y, v1.z);

	UpdateHierarchy();
}

void CTransform::SetLocalRot(float x, float y, float z)
{
	m_vLocalRot = Vector3(x, y, z);

	*m_matLocalRotX = XMMatrixRotationX(x);
	*m_matLocalRotY = XMMatrixRotationY(y);
	*m_matLocalRotZ = XMMatrixRotationZ(z);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY *
		*m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::SetLocalRot(float f[3])
{
	m_vLocalRot = Vector3(f[0], f[1], f[2]);

	*m_matLocalRotX = XMMatrixRotationX(f[0]);
	*m_matLocalRotY = XMMatrixRotationY(f[1]);
	*m_matLocalRotZ = XMMatrixRotationZ(f[2]);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY *
		*m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::SetLocalRot(const Vector3 & v)
{
	m_vLocalRot = Vector3(v.x, v.y, v.z);

	*m_matLocalRotX = XMMatrixRotationX(v.x);
	*m_matLocalRotY = XMMatrixRotationY(v.y);
	*m_matLocalRotZ = XMMatrixRotationZ(v.z);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY *
		*m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::SetLocalRot(const XMVECTOR & v)
{
	Vector3	v1;
	XMStoreFloat3(&v1, v);

	m_vLocalRot = Vector3(v1.x, v1.y, v1.z);

	*m_matLocalRotX = XMMatrixRotationX(v1.x);
	*m_matLocalRotY = XMMatrixRotationY(v1.y);
	*m_matLocalRotZ = XMMatrixRotationZ(v1.z);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY *
		*m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::SetLocalRotX(float x)
{
	m_vLocalRot.x = x;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY *
		*m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::SetLocalRotY(float y)
{
	m_vLocalRot.y = y;

	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY *
		*m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::SetLocalRotZ(float z)
{
	m_vLocalRot.z = z;

	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY *
		*m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::SetLocalPos(float x, float y, float z)
{
	m_vLocalPos = Vector3(x, y, z);

	*m_matLocalPos = XMMatrixTranslation(x, y, z);

	UpdateHierarchy();
}

void CTransform::SetLocalPos(float f[3])
{
	m_vLocalPos = Vector3(f[0], f[1], f[2]);

	*m_matLocalPos = XMMatrixTranslation(f[0], f[1], f[2]);

	UpdateHierarchy();
}

void CTransform::SetLocalPos(const Vector3 & v)
{
	m_vLocalPos = Vector3(v.x, v.y, v.z);

	*m_matLocalPos = XMMatrixTranslation(v.x, v.y, v.z);

	UpdateHierarchy();
}

void CTransform::SetLocalPos(const XMVECTOR & v)
{
	Vector3	v1;
	XMStoreFloat3(&v1, v);
	m_vLocalPos = Vector3(v1.x, v1.y, v1.z);

	*m_matLocalPos = XMMatrixTranslation(v1.x, v1.y, v1.z);

	UpdateHierarchy();
}

Vector3 CTransform::GetWorldScale() const
{
	return m_vWorldScale;
}

Vector3 CTransform::GetWorldRot() const
{
	return m_vWorldRot;
}

Vector3 CTransform::GetWorldPos() const
{
	return m_vWorldPos;
}

Vector3 CTransform::GetWorldAxis(AXIS axis) const
{
	return m_vWorldAxis[axis];
}


Vector3 CTransform::GetParentScale()	const
{
	Vector3	vScale = m_vWorldRelativeScale;

	if (m_pParent && m_iParentModify & PMT_SCALE)
		vScale *= m_pParent->GetParentScale();

	return vScale;
}

Vector3 CTransform::GetParentRot()	const
{
	Vector3	vRot = m_vWorldRelativeRot;

	if (m_pParent && m_iParentModify & PMT_ROT)
		vRot += m_pParent->GetParentRot();

	return vRot;
}

Vector3	CTransform::GetParentPos()	const
{
	Vector3	vPos = m_vWorldRelativePos;

	if (m_pParent && m_iParentModify & PMT_POS)
		vPos += m_pParent->GetParentPos();

	return vPos;
}

Matrix CTransform::GetWorldScaleMatrix() const
{
	return *m_matWorldScale;
}

Matrix CTransform::GetWorldRotMatrix() const
{
	return *m_matWorldRot;
}

Matrix CTransform::GetWorldPosMatrix() const
{
	return *m_matWorldPos;
}

Matrix CTransform::GetParentMatrix() const
{
	if (m_pParent)
	{
		*m_matParent = XMMatrixIdentity();

		if (m_iParentModify & PMT_SCALE)
		{
			*m_matParent *= m_pParent->GetWorldScaleMatrix();
		}

		if (m_iParentModify & PMT_ROT)
		{
			*m_matParent *= m_pParent->GetWorldRotMatrix();
		}

		if (m_iParentModify & PMT_POS)
		{
			*m_matParent *= m_pParent->GetWorldPosMatrix();
		}

		return *m_matParent * m_pParent->GetParentMatrix();
	}

	*m_matParent = XMMatrixIdentity();

	return *m_matParent;
}

Matrix CTransform::GetWorldMatrix() const
{
	return *m_matWorld;
}

void CTransform::SetOriginAxis(AXIS eAxis)
{
	m_eOriginAxis = eAxis;
}

void CTransform::SetWorldScale(float x, float y, float z)
{
	m_vWorldRelativeScale = Vector3(x, y, z);

	if (m_pParent && m_iParentModify & PMT_SCALE)
		m_vWorldScale = m_vWorldRelativeScale * m_pParent->GetParentScale();

	else
		m_vWorldScale = m_vWorldRelativeScale;

	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x,
		m_vWorldScale.y, m_vWorldScale.z);

	UpdateHierarchy();
}

void CTransform::SetWorldScale(float f[3])
{
	m_vWorldRelativeScale = Vector3(f[0], f[1], f[2]);

	if (m_pParent && m_iParentModify & PMT_SCALE)
		m_vWorldScale = m_vWorldRelativeScale * m_pParent->GetParentScale();

	else
		m_vWorldScale = m_vWorldRelativeScale;

	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x,
		m_vWorldScale.y, m_vWorldScale.z);

	UpdateHierarchy();
}

void CTransform::SetWorldScale(const Vector3 & v)
{
	m_vWorldRelativeScale = v;

	if (m_pParent && m_iParentModify & PMT_SCALE)
		m_vWorldScale = m_vWorldRelativeScale * m_pParent->GetParentScale();

	else
		m_vWorldScale = m_vWorldRelativeScale;

	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x,
		m_vWorldScale.y, m_vWorldScale.z);

	UpdateHierarchy();
}

void CTransform::SetWorldScale(const XMVECTOR & v)
{
	m_vWorldRelativeScale = v;

	if (m_pParent && m_iParentModify & PMT_SCALE)
		m_vWorldScale = m_vWorldRelativeScale * m_pParent->GetParentScale();

	else
		m_vWorldScale = m_vWorldRelativeScale;

	*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x,
		m_vWorldScale.y, m_vWorldScale.z);

	UpdateHierarchy();
}

void CTransform::SetWorldRot(float x, float y, float z)
{
	m_vWorldRelativeRot = Vector3(x, y, z);

	if (m_pParent && m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::SetWorldRot(float f[3])
{
	m_vWorldRelativeRot = Vector3(f[0], f[1], f[2]);

	if (m_pParent && m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::SetWorldRot(const Vector3 & v)
{
	m_vWorldRelativeRot = v;

	if (m_pParent && m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::SetWorldRot(const XMVECTOR & v)
{
	m_vWorldRelativeRot = v;

	if (m_pParent && m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::SetWorldRotX(float x)
{
	m_vWorldRelativeRot.x = x;

	if (m_pParent && m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::SetWorldRotY(float y)
{
	m_vWorldRelativeRot.y = y;

	if (m_pParent && m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::SetWorldRotZ(float z)
{
	m_vWorldRelativeRot.z = z;

	if (m_pParent && m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::SetWorldPos(float x, float y, float z, bool bSelf)
{
	m_vWorldRelativePos = Vector3(x, y, z);

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	if (!bSelf)
		UpdateHierarchy();

	else
		m_bUpdate = true;
}

void CTransform::SetWorldPos(float f[3], bool bSelf)
{
	m_vWorldRelativePos = Vector3(f[0], f[1], f[2]);

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	if (!bSelf)
		UpdateHierarchy();

	else
		m_bUpdate = true;
}

void CTransform::SetWorldPos(const Vector3 & v, bool bSelf)
{
	m_vWorldRelativePos = v;

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	if (!bSelf)
		UpdateHierarchy();

	else
		m_bUpdate = true;
}

void CTransform::SetWorldPos(const XMVECTOR & v, bool bSelf)
{
	m_vWorldRelativePos = v;

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	if (!bSelf)
		UpdateHierarchy();

	else
		m_bUpdate = true;
}

void CTransform::SetWorldPosY(float y, bool bSelf)
{
	m_vWorldRelativePos.y = y;

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	if (!bSelf)
		UpdateHierarchy();

	else
		m_bUpdate = true;
}

void CTransform::SetWorldAxis(const Vector3 & vAxis, AXIS eAxis)
{
	m_vWorldAxis[eAxis] = vAxis;
}

void CTransform::ComputeWorldRotationMatrix(Vector3 vAxis[AXIS_END])
{
	*m_matWorldRot = XMMatrixIdentity();

	for (int i = 0; i < AXIS_END; ++i)
	{
		memcpy(&m_matWorldRot->m[i][0], &vAxis[i], sizeof(Vector3));
	}
}

void CTransform::MoveLocal(const Vector3 & vMove)
{
	m_vLocalPos += vMove;

	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x,
		m_vLocalPos.y, m_vLocalPos.z);

	UpdateHierarchy();
}

void CTransform::MoveLocal(const Vector3 & vDir, float fDist)
{
	m_vLocalPos += vDir * fDist;

	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x,
		m_vLocalPos.y, m_vLocalPos.z);

	UpdateHierarchy();
}

void CTransform::MoveLocal(const Vector3 & vDir, float fSpeed, float fTime)
{
	m_vLocalPos += vDir * fSpeed * fTime;

	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x,
		m_vLocalPos.y, m_vLocalPos.z);

	UpdateHierarchy();
}

void CTransform::MoveLocal(AXIS axis, float fDist)
{
	m_vLocalPos += m_vLocalAxis[axis] * fDist;

	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x,
		m_vLocalPos.y, m_vLocalPos.z);

	UpdateHierarchy();
}

void CTransform::MoveLocal(AXIS axis, float fSpeed, float fTime)
{
	m_vLocalPos += m_vLocalAxis[axis] * fSpeed * fTime;

	*m_matLocalPos = XMMatrixTranslation(m_vLocalPos.x,
		m_vLocalPos.y, m_vLocalPos.z);

	UpdateHierarchy();
}

void CTransform::MoveWorld(const Vector3 & vMove)
{
	m_vWorldRelativePos += vMove;

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	UpdateHierarchy();
}

void CTransform::MoveWorld(const Vector3 & vDir, float fDist)
{
	m_vWorldRelativePos += vDir * fDist;

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	UpdateHierarchy();
}

void CTransform::MoveWorld(const Vector3 & vDir, float fSpeed, float fTime)
{
	m_vWorldRelativePos += vDir * fSpeed * fTime;

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	UpdateHierarchy();
}

void CTransform::MoveWorld(AXIS axis, float fDist)
{
	m_vWorldRelativePos += m_vWorldAxis[axis] * fDist;

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	UpdateHierarchy();
}

void CTransform::MoveWorld(AXIS axis, float fSpeed, float fTime)
{
	m_vWorldRelativePos += m_vWorldAxis[axis] * fSpeed * fTime;

	Matrix	matScale, matRot, matPos;

	if (m_pParent && m_iParentModify & PMT_SCALE)
	{
		Vector3	vScale = m_pParent->GetParentScale();
		matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
	}

	if (m_pParent && m_iParentModify & PMT_ROT)
	{
		Vector3	vRot = m_pParent->GetParentRot();
		Matrix	matRotX = XMMatrixRotationX(vRot.x);
		Matrix	matRotY = XMMatrixRotationY(vRot.y);
		Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

		matRot = matRotX * matRotY * matRotZ;
	}

	if (m_pParent && m_iParentModify & PMT_POS)
	{
		Vector3	vPos = m_pParent->GetParentPos();
		matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	}

	Matrix		matParent = matScale * matRot * matPos;

	m_vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

	*m_matWorldPos = XMMatrixTranslation(m_vWorldPos.x,
		m_vWorldPos.y, m_vWorldPos.z);

	UpdateHierarchy();
}

void CTransform::RotateLocal(const Vector3 & vRot)
{
	m_vLocalRot += vRot;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY * *m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::RotateLocal(const Vector3 & vRot, float fTime)
{
	m_vLocalRot += vRot * fTime;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY * *m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::RotateLocalX(float fX)
{
	m_vLocalRot.x += fX;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY * *m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::RotateLocalX(float fX, float fTime)
{
	m_vLocalRot.x += fX * fTime;

	*m_matLocalRotX = XMMatrixRotationX(m_vLocalRot.x);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY * *m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::RotateLocalY(float fY)
{
	m_vLocalRot.y += fY;

	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY * *m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::RotateLocalY(float fY, float fTime)
{
	m_vLocalRot.y += fY * fTime;

	*m_matLocalRotY = XMMatrixRotationY(m_vLocalRot.y);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY * *m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::RotateLocalZ(float fZ)
{
	m_vLocalRot.z += fZ;

	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY * *m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::RotateLocalZ(float fZ, float fTime)
{
	m_vLocalRot.z += fZ * fTime;

	*m_matLocalRotZ = XMMatrixRotationZ(m_vLocalRot.z);
	*m_matLocalRot = *m_matLocalRotX * *m_matLocalRotY * *m_matLocalRotZ;

	UpdateHierarchy();
}

void CTransform::RotateWorld(const Vector3 & vRot)
{
	m_vWorldRelativeRot += vRot;

	if (m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::RotateWorld(const Vector3 & vRot, float fTime)
{
	m_vWorldRelativeRot += vRot * fTime;

	if (m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::RotateWorldX(float fX)
{
	m_vWorldRelativeRot.x += fX;

	if (m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::RotateWorldX(float fX, float fTime)
{
	m_vWorldRelativeRot.x += fX * fTime;

	if (m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::RotateWorldY(float fY)
{
	m_vWorldRelativeRot.y += fY;

	if (m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::RotateWorldY(float fY, float fTime)
{
	m_vWorldRelativeRot.y += fY * fTime;

	if (m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::RotateWorldZ(float fZ)
{
	m_vWorldRelativeRot.z += fZ;

	if (m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::RotateWorldZ(float fZ, float fTime)
{
	m_vWorldRelativeRot.z += fZ * fTime;

	if (m_iParentModify & PMT_ROT)
		m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

	else
		m_vWorldRot = m_vWorldRelativeRot;

	*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);
	*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;

	UpdateHierarchy();
}

void CTransform::ComputeLocalAxis()
{
	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vLocalAxis[i] = Vector3::Axis[i].TransformNormal(m_matLocal->mat);
		m_vLocalAxis[i] = m_vLocalAxis[i].Normalize();
	}
}

void CTransform::ComputeWorldAxis()
{
	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(m_matWorld->mat);
		m_vWorldAxis[i] = m_vWorldAxis[i].Normalize();
	}
}

void CTransform::ComputeWorldAxisRot()
{
	for (int i = 0; i < AXIS_END; ++i)
	{
		m_vWorldAxis[i] = Vector3::Axis[i].TransformNormal(m_matWorldRot->mat);
		m_vWorldAxis[i] = m_vWorldAxis[i].Normalize();
	}
}

void CTransform::LookAt(CGameObject * pObj)
{
	CTransform*	pTransform = pObj->GetTransform();

	LookAt(pTransform->GetWorldPos());

	SAFE_RELEASE(pTransform);
}

void CTransform::LookAt(CTransform * pTransform)
{
	LookAt(pTransform->GetWorldPos());
}

void CTransform::LookAt(const Vector3 & vPos)
{
	Vector3	vDir = vPos - m_vWorldPos;
	vDir = vDir.Normalize();

	// 원본 축 벡터와 바라볼 벡터간 각도를 구해준다.
	// 이 각도를 이용해서 회전한다.
	float	fAngle = vDir.Angle(Vector3::Axis[m_eOriginAxis]);

	// 회전 축을 구한다. 2D의 경우 Z, -Z 축으로 나오게 된다.
	Vector3	vAxis = Vector3::Axis[m_eOriginAxis].Cross(vDir);

	if (vAxis == Vector3::Zero)
		return;

	// 임의의 축을 이용한 회전 행렬을 구한다.
	*m_matWorldRot = XMMatrixRotationAxis(vAxis.Convert(), fAngle);

	UpdateHierarchy();
}

void CTransform::Save(FILE * pFile)
{
	CComponent::Save(pFile);

	// Local
	fwrite(&m_vPivot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vLocalScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vLocalRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vLocalPos, sizeof(Vector3), 1, pFile);
	fwrite(&m_vLocalAxis, sizeof(Vector3), AXIS_END, pFile);

	fwrite(m_matLocalScale, sizeof(Matrix), 1, pFile);
	fwrite(m_matLocalRotX, sizeof(Matrix), 1, pFile);
	fwrite(m_matLocalRotY, sizeof(Matrix), 1, pFile);
	fwrite(m_matLocalRotZ, sizeof(Matrix), 1, pFile);
	fwrite(m_matLocalRot, sizeof(Matrix), 1, pFile);
	fwrite(m_matLocalPos, sizeof(Matrix), 1, pFile);
	fwrite(m_matLocal, sizeof(Matrix), 1, pFile);

	// World
	fwrite(&m_vWorldScale, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldRot, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldPos, sizeof(Vector3), 1, pFile);
	fwrite(&m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);

	fwrite(m_matWorldScale, sizeof(Matrix), 1, pFile);
	fwrite(m_matWorldRotX, sizeof(Matrix), 1, pFile);
	fwrite(m_matWorldRotY, sizeof(Matrix), 1, pFile);
	fwrite(m_matWorldRotZ, sizeof(Matrix), 1, pFile);
	fwrite(m_matWorldRot, sizeof(Matrix), 1, pFile);
	fwrite(m_matWorldPos, sizeof(Matrix), 1, pFile);
	fwrite(m_matParent, sizeof(Matrix), 1, pFile);
	fwrite(m_matWorld, sizeof(Matrix), 1, pFile);

	fwrite(&m_bUpdate, sizeof(bool), 1, pFile);
	fwrite(&m_bStatic, sizeof(bool), 1, pFile);
}

void CTransform::Load(FILE * pFile)
{
	CComponent::Load(pFile);

	// Local
	fread(&m_vPivot, sizeof(Vector3), 1, pFile);
	fread(&m_vLocalScale, sizeof(Vector3), 1, pFile);
	fread(&m_vLocalRot, sizeof(Vector3), 1, pFile);
	fread(&m_vLocalPos, sizeof(Vector3), 1, pFile);
	fread(&m_vLocalAxis, sizeof(Vector3), AXIS_END, pFile);

	fread(m_matLocalScale, sizeof(Matrix), 1, pFile);
	fread(m_matLocalRotX, sizeof(Matrix), 1, pFile);
	fread(m_matLocalRotY, sizeof(Matrix), 1, pFile);
	fread(m_matLocalRotZ, sizeof(Matrix), 1, pFile);
	fread(m_matLocalRot, sizeof(Matrix), 1, pFile);
	fread(m_matLocalPos, sizeof(Matrix), 1, pFile);
	fread(m_matLocal, sizeof(Matrix), 1, pFile);

	// World
	fread(&m_vWorldScale, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldRot, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldPos, sizeof(Vector3), 1, pFile);
	fread(&m_vWorldAxis, sizeof(Vector3), AXIS_END, pFile);

	fread(m_matWorldScale, sizeof(Matrix), 1, pFile);
	fread(m_matWorldRotX, sizeof(Matrix), 1, pFile);
	fread(m_matWorldRotY, sizeof(Matrix), 1, pFile);
	fread(m_matWorldRotZ, sizeof(Matrix), 1, pFile);
	fread(m_matWorldRot, sizeof(Matrix), 1, pFile);
	fread(m_matWorldPos, sizeof(Matrix), 1, pFile);
	fread(m_matParent, sizeof(Matrix), 1, pFile);
	fread(m_matWorld, sizeof(Matrix), 1, pFile);

	fread(&m_bUpdate, sizeof(bool), 1, pFile);
	fread(&m_bStatic, sizeof(bool), 1, pFile);

	m_bUpdate = true;
}

void CTransform::Static(bool bStatic)
{
	m_bStatic = bStatic;
}

void CTransform::UpdateHierarchy()
{
	m_bUpdate = true;

	m_pGameObject->UpdateTransformHierarchy();
}

bool CTransform::Init()
{
	m_matLocalScale = new Matrix;
	m_matLocalRotX = new Matrix;
	m_matLocalRotY = new Matrix;
	m_matLocalRotZ = new Matrix;
	m_matLocalRot = new Matrix;
	m_matLocalPos = new Matrix;
	m_matLocal = new Matrix;

	m_matWorldScale = new Matrix;
	m_matWorldRotX = new Matrix;
	m_matWorldRotY = new Matrix;
	m_matWorldRotZ = new Matrix;
	m_matWorldRot = new Matrix;
	m_matWorldPos = new Matrix;
	m_matParent = new Matrix;
	m_matWorld = new Matrix;

	*m_matLocalScale = XMMatrixIdentity();
	*m_matLocalRotX = XMMatrixIdentity();
	*m_matLocalRotY = XMMatrixIdentity();
	*m_matLocalRotZ = XMMatrixIdentity();
	*m_matLocalRot = XMMatrixIdentity();
	*m_matLocalPos = XMMatrixIdentity();
	*m_matLocal = XMMatrixIdentity();

	*m_matWorldScale = XMMatrixIdentity();
	*m_matWorldRotX = XMMatrixIdentity();
	*m_matWorldRotY = XMMatrixIdentity();
	*m_matWorldRotZ = XMMatrixIdentity();
	*m_matWorldRot = XMMatrixIdentity();
	*m_matWorldPos = XMMatrixIdentity();
	*m_matParent = XMMatrixIdentity();
	*m_matWorld = XMMatrixIdentity();

	m_vLocalScale = Vector3::One;
	memcpy(m_vLocalAxis, Vector3::Axis, sizeof(Vector3) * AXIS_END);

	m_vWorldScale = Vector3::One;
	m_vWorldRelativeScale = Vector3::One;
	memcpy(m_vWorldAxis, Vector3::Axis, sizeof(Vector3) * AXIS_END);

	m_bUpdate = true;
	m_bStatic = false;

	return true;
}

void CTransform::Input(float fTime)
{
}

int CTransform::Update(float fTime)
{
	if (!m_bUpdate)
		return 0;

	else if (m_bStatic)
		return 0;

	if (m_pParent)
	{
		if (m_iParentModify & PMT_SCALE)
		{
			m_vWorldScale = m_vWorldRelativeScale * m_pParent->GetParentScale();

			*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y,
				m_vWorldScale.z);
		}

		if (m_iParentModify & PMT_ROT)
		{
			m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

			*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
			*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
			*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);

			*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;
		}

		if (m_iParentModify & PMT_POS)
		{
			Matrix	matScale, matRot, matPos;

			if (m_iParentModify & PMT_SCALE)
			{
				Vector3	vScale = m_pParent->GetParentScale();
				matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
			}

			if (m_iParentModify & PMT_ROT)
			{
				Vector3	vRot = m_pParent->GetParentRot();
				Matrix	matRotX = XMMatrixRotationX(vRot.x);
				Matrix	matRotY = XMMatrixRotationY(vRot.y);
				Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

				matRot = matRotX * matRotY * matRotZ;
			}

			if (m_iParentModify & PMT_POS)
			{
				Vector3	vPos = m_pParent->GetWorldPos();
				matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
			}

			Matrix		matParent = matScale * matRot * matPos;

			Vector3	vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

			*m_matWorldPos = XMMatrixTranslation(vWorldPos.x, vWorldPos.y,
				vWorldPos.z);

			m_vWorldPos = vWorldPos;
		}
	}

	*m_matLocal = *m_matLocalScale * *m_matLocalRot * *m_matLocalPos;
	*m_matWorld = *m_matWorldScale * *m_matWorldRot * *m_matWorldPos;

	m_bUpdate = false;

	ComputeLocalAxis();
	ComputeWorldAxis();

	return 0;
}

int CTransform::LateUpdate(float fTime)
{
	if (!m_bUpdate)
		return 0;

	else if (m_bStatic)
		return 0;

	if (m_pParent)
	{
		if (m_iParentModify & PMT_SCALE)
		{
			m_vWorldScale = m_vWorldRelativeScale * m_pParent->GetParentScale();

			*m_matWorldScale = XMMatrixScaling(m_vWorldScale.x, m_vWorldScale.y,
				m_vWorldScale.z);
		}

		if (m_iParentModify & PMT_ROT)
		{
			m_vWorldRot = m_vWorldRelativeRot + m_pParent->GetParentRot();

			*m_matWorldRotX = XMMatrixRotationX(m_vWorldRot.x);
			*m_matWorldRotY = XMMatrixRotationY(m_vWorldRot.y);
			*m_matWorldRotZ = XMMatrixRotationZ(m_vWorldRot.z);

			*m_matWorldRot = *m_matWorldRotX * *m_matWorldRotY * *m_matWorldRotZ;
		}

		if (m_iParentModify & PMT_POS)
		{
			Matrix	matScale, matRot, matPos;

			if (m_iParentModify & PMT_SCALE)
			{
				Vector3	vScale = m_pParent->GetParentScale();
				matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
			}

			if (m_iParentModify & PMT_ROT)
			{
				Vector3	vRot = m_pParent->GetParentRot();
				Matrix	matRotX = XMMatrixRotationX(vRot.x);
				Matrix	matRotY = XMMatrixRotationY(vRot.y);
				Matrix	matRotZ = XMMatrixRotationZ(vRot.z);

				matRot = matRotX * matRotY * matRotZ;
			}

			if (m_iParentModify & PMT_POS)
			{
				Vector3	vPos = m_pParent->GetWorldPos();
				matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
			}

			Matrix		matParent = matScale * matRot * matPos;

			Vector3	vWorldPos = m_vWorldRelativePos.TransformCoord(matParent.mat);

			*m_matWorldPos = XMMatrixTranslation(vWorldPos.x, vWorldPos.y,
				vWorldPos.z);

			m_vWorldPos = vWorldPos;
		}
	}

	*m_matLocal = *m_matLocalScale * *m_matLocalRot * *m_matLocalPos;
	*m_matWorld = *m_matWorldScale * *m_matWorldRot * *m_matWorldPos;

	m_bUpdate = false;

	ComputeLocalAxis();
	ComputeWorldAxis();

	return 0;
}

void CTransform::Collision(float fTime)
{
}

void CTransform::Render(float fTime)
{
}

CTransform * CTransform::Clone()
{
	return new CTransform(*this);
}
