#include "ColliderRay.h"
#include "Transform.h"
#include "ColliderSphere.h"

Engine_USING

CColliderRay::CColliderRay()
{
	SetTag("ColliderRay");
	SetTypeName("CColliderRay");
	SetTypeID<CColliderRay>();
	m_eCollType = CT_RAY;
	m_strGroup = "Default";
	m_eAxis = AXIS_NONE;

	memset(&m_tRay, 0, sizeof(RAY));
}

CColliderRay::CColliderRay(const CColliderRay & collider)	:
	CCollider(collider)
{
	m_tRay = collider.m_tRay;
}

CColliderRay::~CColliderRay()
{
}

RAY CColliderRay::GetRay() const
{
	return m_tRay;
}

PRAY CColliderRay::GetRayAddress()
{
	return &m_tRay;
}

void CColliderRay::SetPos(const Vector3 & vPos)
{
	m_tRay.vPos = vPos;
}

void CColliderRay::SetDir(const Vector3 & vDir)
{
	m_tRay.vDir = vDir;
}

void CColliderRay::SetDir(AXIS eAxis)
{
	m_eAxis = eAxis;
}

void CColliderRay::SetDistance(float fDist)
{
	m_tRay.fDist = fDist;
}

bool CColliderRay::Init()
{
	return true;
}

void CColliderRay::Input(float fTime)
{
}

int CColliderRay::Update(float fTime)
{
	return 0;
}

int CColliderRay::LateUpdate(float fTime)
{
	if (m_eAxis != AXIS_NONE)
		m_tRay.vDir = m_pTransform->GetWorldAxis(m_eAxis);

	return 0;
}

void CColliderRay::Collision(float fTime)
{
}

void CColliderRay::Render(float fTime)
{
}

CColliderRay * CColliderRay::Clone()
{
	return new CColliderRay(*this);
}

bool CColliderRay::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_SPHERE:
		return CollisionRayToSphere(&m_tRay, ((CColliderSphere*)pDest)->GetSphere());
	}

	return false;
}
