#include "LightSpot.h"
#include "Transform.h"

GAME_USING

CLightSpot::CLightSpot()
{
	SetTag("LightSpot");
	SetTypeName("CLightSpot");
	SetTypeID<CLightSpot>();
	m_tInfo.iType = LT_SPOT;
	m_tInfo.fRange = 5.f;
	m_tInfo.fInAngle = PI / 6.f;
	m_tInfo.fOutAngle = PI / 4.f;
}


CLightSpot::CLightSpot(const CLightSpot & light)
{
}

CLightSpot::~CLightSpot()
{
}

bool CLightSpot::Init()
{
	return true;
}

void CLightSpot::Input(float fTime)
{
}

int CLightSpot::Update(float fTime)
{
	return 0;
}

int CLightSpot::LateUpdate(float fTime)
{
	// 조명의 방향을 얻어온다. 조명은 게임오브젝트로 구성이 되어있다. 게임오브젝트가 가지고 있는
	// Transform 에서 Z축방향을 얻어와서 조명의 방향으로 사용하도록 한다.
	m_tInfo.vDir = m_pTransform->GetWorldAxis(AXIS_Z);
	m_tInfo.vPos = m_pTransform->GetWorldPos();

	return 0;
}

void CLightSpot::Collision(float fTime)
{
}

void CLightSpot::Render(float fTime)
{
}

CLightSpot * CLightSpot::Clone()
{
	return new CLightSpot(*this);
}
