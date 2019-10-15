#include "Navigation3D.h"
#include "../Core/NavigationManager.h"
#include "Transform.h"

Engine_USING

CNavigation3D::CNavigation3D()
{
	m_eType = CT_NAVIGATION3D;
	SetTag("Navigation3D");
	SetTypeName("CNavigation3D");
	SetTypeID<CNavigation3D>();
}

CNavigation3D::CNavigation3D(const CNavigation3D & nav)	:
	CComponent(nav)
{
	*this = nav;
}

CNavigation3D::~CNavigation3D()
{
}

bool CNavigation3D::Init()
{
	return true;
}

void CNavigation3D::Input(float fTime)
{
}

int CNavigation3D::Update(float fTime)
{
	float	fY = GET_SINGLE(CNavigationManager)->GetY(m_pTransform->GetWorldPos());

	m_pTransform->SetWorldPosY(fY);

	return 0;
}

int CNavigation3D::LateUpdate(float fTime)
{
	return 0;
}

void CNavigation3D::Collision(float fTime)
{
}

void CNavigation3D::Render(float fTime)
{
}

CNavigation3D * CNavigation3D::Clone()
{
	return new CNavigation3D(*this);
}
