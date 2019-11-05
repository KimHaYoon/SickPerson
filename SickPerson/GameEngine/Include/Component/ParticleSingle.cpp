#include "ParticleSingle.h"
#include "Renderer.h"
#include "Transform.h"
#include "Material.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"

Engine_USING

CParticleSingle::CParticleSingle()	:
	m_pRenderer(NULL)
{
	SetTag("ParticleSingle");
	SetTypeName("CParticleSingle");
	SetTypeID<CParticleSingle>();
	m_eType = CT_PARTICLE_SINGLE;

	m_vSize = Vector2::Zero;
}

CParticleSingle::CParticleSingle(const CParticleSingle & particle)	:
	CComponent(particle)
{
	m_pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);
}

CParticleSingle::~CParticleSingle()
{
	SAFE_RELEASE(m_pRenderer);
}

void CParticleSingle::SetSize(const Vector2 & vSize)
{
	m_vSize = vSize;
}

void CParticleSingle::SetSize(float x, float y)
{
	m_vSize = Vector2(x, y);
}

bool CParticleSingle::Init()
{
	m_pRenderer = m_pGameObject->AddComponent<CRenderer>("ParticleRenderer");

	m_pRenderer->SetMesh("ParticleMesh");
	m_pRenderer->SetShader(EFFECT_SHADER);
	m_pRenderer->SetInputLayout("VertexPos");
	m_pRenderer->AlphaEnable(true);

	// 파티클 상수버퍼를 만들어준다.
	m_pRenderer->CreateCBuffer("Particle", 11, sizeof(PARTICLECBUFFER),
		SCT_GEOMETRY | SCT_PIXEL);

	CMaterial*	pMaterial = m_pRenderer->CreateMaterial();

	pMaterial->SetDiffuseSampler(SAMPLER_LINEAR);

	SAFE_RELEASE(pMaterial);

	return true;
}

void CParticleSingle::Input(float fTime)
{
}

int CParticleSingle::Update(float fTime)
{
	return 0;
}

int CParticleSingle::LateUpdate(float fTime)
{
	CTransform*	pCamTr = m_pScene->GetMainCameraTr();

	PARTICLECBUFFER	tBuffer = {};

	tBuffer.vSize = m_vSize;
	tBuffer.vCamAxisX = pCamTr->GetWorldAxis(AXIS_X);
	tBuffer.vCamAxisY = pCamTr->GetWorldAxis(AXIS_Y);

	m_pRenderer->UpdateCBuffer("Particle", 11, sizeof(PARTICLECBUFFER),
		SCT_GEOMETRY | SCT_PIXEL, &tBuffer);

	SAFE_RELEASE(pCamTr);

	return 0;
}

void CParticleSingle::Collision(float fTime)
{
}

void CParticleSingle::Render(float fTime)
{
}

CParticleSingle * CParticleSingle::Clone()
{
	return new CParticleSingle(*this);
}

void CParticleSingle::Save(FILE * pFile)
{
}

void CParticleSingle::Load(FILE * pFile)
{
}
