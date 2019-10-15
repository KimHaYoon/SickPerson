#include "LightDir.h"
#include "Transform.h"

Engine_USING

CLightDir::CLightDir()
{
	SetTag("LightDir");
	SetTypeName("CLightDir");
	SetTypeID<CLightDir>();
	m_tInfo.iType = LT_DIR;
}

CLightDir::CLightDir(const CLightDir & light)	:
	CLight(light)
{
}

CLightDir::~CLightDir()
{
}

bool CLightDir::Init()
{
	return true;
}

void CLightDir::Input(float fTime)
{
}

int CLightDir::Update(float fTime)
{
	return 0;
}

int CLightDir::LateUpdate(float fTime)
{
	// 조명의 방향을 얻어온다. 조명은 게임오브젝트로 구성이 되어있다. 게임오브젝트가 가지고 있는
	// Transform 에서 Z축방향을 얻어와서 조명의 방향으로 사용하도록 한다.
	m_tInfo.vDir = m_pTransform->GetWorldAxis(AXIS_Z);

	return 0;
}

void CLightDir::Collision(float fTime)
{
}

void CLightDir::Render(float fTime)
{
}

CLightDir * CLightDir::Clone()
{
	return new CLightDir(*this);
}
