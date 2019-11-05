#include "Decal.h"
#include "Renderer.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"

Engine_USING

CDecal::CDecal()
{
	SetTag("Decal");
	SetTypeName("CDecal");
	SetTypeID<CDecal>();
	m_eType = CT_DECAL;
}

CDecal::CDecal(const CDecal & decal) : 
	CComponent(decal)
{
}

CDecal::~CDecal()
{
}

bool CDecal::Init()
{
	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("DecalRender");

	pRenderer->SetMesh("Cube");
	pRenderer->SetInputLayout("VertexPos");
	pRenderer->SetShader(DECAL_SHADER);
	pRenderer->SetRenderState(DEPTH_DISABLE);
	pRenderer->SetRenderState(CULLING_NONE);
	
	SAFE_RELEASE(pRenderer);

	return true;
}

void CDecal::Input(float fTime)
{
}

int CDecal::Update(float fTime)
{
	return 0;
}

int CDecal::LateUpdate(float fTime)
{
	return 0;
}

void CDecal::Collision(float fTime)
{
}

void CDecal::Render(float fTime)
{
}

CDecal * CDecal::Clone()
{
	return new CDecal(*this);
}
