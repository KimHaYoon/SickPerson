#include "Mouse.h"
#include "ColliderPoint.h"
#include "ColliderRay.h"
#include "Camera.h"
#include "Transform.h"
#include "Animation2D.h"
#include "Renderer2D.h"
#include "Material.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../GameObject/GameObject.h"

Engine_USING

CMouse::CMouse()
{
	SetTag("Mouse");
	SetTypeName("CMouse");
	SetTypeID<CMouse>();
	m_eType = CT_MOUSE;
	m_bMouseShow = FALSE;
}

CMouse::CMouse(const CMouse & mouse)	:
	CComponent(mouse)
{
	m_bMouseShow = FALSE;
}

CMouse::~CMouse()
{
}

bool CMouse::Init()
{
	ShowCursor(FALSE);

	m_pTransform->SetWorldScale(5.f, 5.f, 1.f);

	CRenderer2D*	pRenderer = m_pGameObject->AddComponent<CRenderer2D>("MouseRenderer");

	pRenderer->SetMesh("RectOrthoLTTex");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("VertexTex");
	pRenderer->SetRenderState(ALPHA_BLEND);

	CMaterial*	pMaterial	= pRenderer->CreateMaterial(SAMPLER_LINEAR,
		"MouseDefault", 0, 0, L"mouse.png");

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	// ����Ʈ �浹ü ����
	CColliderPoint*	pColl = m_pGameObject->AddComponent<CColliderPoint>("MousePoint");

	pColl->SetPos(m_pTransform->GetWorldPos().x,
		m_pTransform->GetWorldPos().y);

	SAFE_RELEASE(pColl);

	// Ray �浹ü ����
	CColliderRay*	pCollRay = m_pGameObject->AddComponent<CColliderRay>("MouseRay");

	SAFE_RELEASE(pCollRay);

	return true;
}

void CMouse::Input(float fTime)
{
}

int CMouse::Update(float fTime)
{
	// ȭ�� ���� �������� üũ�Ѵ�.
	Vector3	vPos = m_pTransform->GetWorldPos();
	if (vPos.x < 0 || vPos.x > DEVICE_RESOLUTION.iWidth ||
		vPos.y < 0 || vPos.y > DEVICE_RESOLUTION.iHeight)
	{
		if (!m_bMouseShow)
		{
			m_bMouseShow = TRUE;
			ShowCursor(TRUE);
		}
	}

	else
	{
		if (m_bMouseShow)
		{
			m_bMouseShow = FALSE;
			ShowCursor(FALSE);
		}
	}

	// ���콺 Ray�� �����.
	// ī�޶� ���´�.	
	CCamera*	pCamera = m_pScene->GetMainCamera();

	Matrix	matProj = pCamera->GetProjMatrix();
	Matrix	matView = pCamera->GetViewMatrix();

	SAFE_RELEASE(pCamera);

	D3D12_VIEWPORT	tVP = {};

	UINT	iVPCount = 1;
	CMLIST->RSGetViewports(&iVPCount, &tVP);

	float	fHalfW = tVP.Width * 0.5f;
	float	fHalfH = tVP.Height * 0.5f;

	Vector3	vRayDir;

	// ����������� Ray�� �����ش�.
	vRayDir.x = (vPos.x / fHalfW - 1.f) / matProj.m[0][0];
	vRayDir.y = (-(vPos.y / fHalfH) + 1.f) / matProj.m[1][1];
	vRayDir.z = 1.f;

	vRayDir = vRayDir.Normalize();

	// ����������� ��ȯ�Ѵ�.
	// ���� ������� �����ش�.
	matView	= XMMatrixInverse(&XMMatrixDeterminant(matView.mat),
		matView.mat);
	vRayDir = vRayDir.TransformNormal(matView.mat);

	Vector3	vRayPos;

	vRayPos = vRayPos.TransformCoord(matView.mat);

	// Ray �浹ü�� ���´�.
	CColliderRay*	pRay = FindComponentFromTag<CColliderRay>("MouseRay");

	pRay->SetPos(vRayPos);
	pRay->SetDir(vRayDir);

	SAFE_RELEASE(pRay);

	return 0;
}

int CMouse::LateUpdate(float fTime)
{
	return 0;
}

void CMouse::Collision(float fTime)
{
}

void CMouse::Render(float fTime)
{
}

CMouse * CMouse::Clone()
{
	return new CMouse(*this);
}
