#include "LightPoint.h"
#include "Transform.h"

GAME_USING

CLightPoint::CLightPoint()
{
	SetTag("LightPoint");
	SetTypeName("CLightPoint");
	SetTypeID<CLightPoint>();
	m_tInfo.iType = LT_POINT;
	m_tInfo.fIntensityPercent = 0.5f;
}

CLightPoint::CLightPoint(const CLightPoint & light)	:
	CLight(light)
{
}

CLightPoint::~CLightPoint()
{
}

bool CLightPoint::Init()
{
	return true;
}

void CLightPoint::Input(float fTime)
{
}

int CLightPoint::Update(float fTime)
{
	return 0;
}

int CLightPoint::LateUpdate(float fTime)
{
	m_tInfo.vPos = m_pTransform->GetWorldPos();

	return 0;
}

void CLightPoint::Collision(float fTime)
{
}

void CLightPoint::Render(float fTime)
{
}

CLightPoint * CLightPoint::Clone()
{
	return new CLightPoint(*this);
}
