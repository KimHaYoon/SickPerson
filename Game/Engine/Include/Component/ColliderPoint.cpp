#include "ColliderPoint.h"
#include "ColliderRect.h"

Engine_USING

CColliderPoint::CColliderPoint()
{
	SetTag("ColliderPoint");
	SetTypeName("CColliderPoint");
	SetTypeID<CColliderPoint>();
	m_eCollType = CT_POINT;
	m_strGroup = "UI";
}

CColliderPoint::CColliderPoint(const CColliderPoint & collider)	:
	CCollider(collider)
{
	m_vPos = collider.m_vPos;
}

CColliderPoint::~CColliderPoint()
{
}

void CColliderPoint::SetPos(const Vector2 & vPos)
{
	m_vPos = vPos;

	m_vPrevPos = Vector3(vPos.x, vPos.y, 0.f);
}

void CColliderPoint::SetPos(float x, float y)
{
	m_vPos.x = x;
	m_vPos.y = y;

	m_vPrevPos = Vector3(m_vPos.x, m_vPos.y, 0.f);
}

void CColliderPoint::SetPos(const POINT & ptPos)
{
	m_vPos.x = ptPos.x;
	m_vPos.y = ptPos.y;

	m_vPrevPos = Vector3(m_vPos.x, m_vPos.y, 0.f);
}

Vector2 CColliderPoint::GetPos() const
{
	return m_vPos;
}

bool CColliderPoint::Init()
{
	return true;
}

void CColliderPoint::Input(float fTime)
{
	CCollider::Input(fTime);
}

int CColliderPoint::Update(float fTime)
{
	CCollider::Update(fTime);

	m_vPos.x += m_vMove.x;
	m_vPos.y += m_vMove.y;

	return 0;
}

int CColliderPoint::LateUpdate(float fTime)
{
	CCollider::LateUpdate(fTime);

	m_vCollMin = Vector3(m_vPos.x, m_vPos.y, 0.f);
	m_vCollMax = Vector3(m_vPos.x, m_vPos.y, 0.f);

	return 0;
}

void CColliderPoint::Collision(float fTime)
{
	CCollider::Collision(fTime);
}

void CColliderPoint::Render(float fTime)
{
}

CColliderPoint * CColliderPoint::Clone()
{
	return new CColliderPoint(*this);
}

bool CColliderPoint::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToPoint(((CColliderRect*)pDest)->GetInfo(), m_vPos);
	case CT_POINT:
		return m_vPos == ((CColliderPoint*)pDest)->m_vPos;
	}

	return false;
}
