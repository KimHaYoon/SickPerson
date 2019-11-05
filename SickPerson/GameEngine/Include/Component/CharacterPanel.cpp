#include "CharacterPanel.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Material.h"
#include "../Device.h"
#include "../GameObject/GameObject.h"
#include "../Rendering/RenderingTarget.h"
#include "../Rendering/RenderManager.h"
#include "../Scene/Scene.h"

Engine_USING

CCharacterPanel::CCharacterPanel() :
	m_bAddResourceView(false),
	m_pTarget(NULL),
	m_pTargetTr(NULL),
	m_pTargetCamera(NULL),
	m_pTargetCameraTr(NULL)
{
}

CCharacterPanel::CCharacterPanel(const CCharacterPanel & panel)
{
	m_pTarget = NULL;
	m_pTargetTr = NULL;
	m_pTargetCamera = NULL;
	m_pTargetCameraTr = NULL;
	m_bAddResourceView = false;
}

CCharacterPanel::~CCharacterPanel()
{
	SAFE_RELEASE(m_pTargetTr);
	SAFE_RELEASE(m_pTargetCamera);
	SAFE_RELEASE(m_pTargetCameraTr);
}	

void CCharacterPanel::SetTarget(CGameObject * pTarget)
{
	m_pTarget = pTarget;
	m_pTargetTr = m_pTarget->GetTransform();
}

bool CCharacterPanel::Init()
{
	CRenderingTarget*	pTarget = GET_SINGLE(CRenderManager)->CreateRenderTarget("CharacterTarget",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		Vector4::Zero,
		DXGI_FORMAT_D24_UNORM_S8_UINT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(400.f, 0.f, 0.f),
		Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	CGameObject* pCameraObj = m_pScene->CreateCamera("CharacterTargetCamera",
		Vector3::Zero, PI / 3.f,
		DEVICE_RESOLUTION.iWidth / (float)DEVICE_RESOLUTION.iHeight,
		0.03f, 5000.f);

	m_pTargetCamera = pCameraObj->FindComponentFromType<CCamera>(CT_CAMERA);
	m_pTargetCameraTr = pCameraObj->GetTransform();

	SAFE_RELEASE(pCameraObj);

	return true;
}

void CCharacterPanel::Input(float fTime)
{
}

int CCharacterPanel::Update(float fTime)
{
	return 0;
}

int CCharacterPanel::LateUpdate(float fTime)
{
	
	return 0;
}

void CCharacterPanel::Collision(float fTime)
{
}

void CCharacterPanel::Render(float fTime)
{
	CRenderingTarget* pTarget = GET_SINGLE(CRenderManager)->FindRenderTarget("CharacterTarget");

	if (!m_bAddResourceView)
	{
		m_bAddResourceView = true;

		CRenderer2D*	pRenderer = FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial*	pMaterial = pRenderer->GetMaterial();

		pMaterial->AddMultiTexture(SAMPLER_LINEAR, pTarget->GetShaderResourceView(),
			11, 11);

		SAFE_RELEASE(pMaterial);

		pRenderer->CreateCBuffer("MultiTexture", 12, sizeof(MULTITEXTURECBUFFER), SCT_PIXEL);

		MULTITEXTURECBUFFER	tBuffer = {};
		tBuffer.vAreaStart = Vector2(0.1f, 0.f);
		tBuffer.vAreaEnd = Vector2(0.4f, 0.5f);

		pRenderer->UpdateCBuffer("MultiTexture", 12, sizeof(MULTITEXTURECBUFFER),
			SCT_PIXEL, &tBuffer);

		pRenderer->SetShader(UI_MULTITEXTURE_SHADER);

		SAFE_RELEASE(pRenderer);
	}

	// 타겟의 위치를 통해 카메라 위치 조정
	m_pTargetCameraTr->SetWorldPos(m_pTargetTr->GetWorldPos() +
		m_pTargetTr->GetWorldAxis(AXIS_Z) * 2.f + Vector3(0.f, 1.f, 0.f));

	m_pTargetCameraTr->SetWorldAxis(m_pTargetTr->GetWorldAxis(AXIS_X) * -1.f,
		AXIS_X);
	m_pTargetCameraTr->SetWorldAxis(m_pTargetTr->GetWorldAxis(AXIS_Y),
		AXIS_Y);
	m_pTargetCameraTr->SetWorldAxis(m_pTargetTr->GetWorldAxis(AXIS_Z) * -1.f,
		AXIS_Z);

	m_pTargetCamera->LateUpdate(fTime);

	m_pScene->ChangeMainCamera("CharacterTargetCamera");

	pTarget->ClearTarget();
	pTarget->SetTarget();

	CRenderer*	pTargetRenderer = m_pTarget->FindComponentFromType<CRenderer>(CT_RENDERER);

	pTargetRenderer->Render(fTime);

	SAFE_RELEASE(pTargetRenderer);

	pTarget->ResetTarget();

	SAFE_RELEASE(pTarget);

	m_pScene->ChangeMainCamera("MainCamera");

	SAFE_RELEASE(pTarget);
}

CCharacterPanel * CCharacterPanel::Clone()
{
	return new CCharacterPanel(*this);;
}
