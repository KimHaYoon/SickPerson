#include "UIPanel.h"
#include "Renderer2D.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "ColliderRect.h"
#include "../Core/Input.h"
#include "ColliderPoint.h"

Engine_USING

CUIPanel::CUIPanel()
{
	SetTag("UIPanel");
	SetTypeName("CUIPanel");
	SetTypeID<CUIPanel>();
	m_bEnableTitle = true;
	m_bClick = false;
}

CUIPanel::CUIPanel(const CUIPanel & panel)	:
	CUI(panel)
{
	m_bEnableTitle = panel.m_bEnableTitle;
	m_bClick = false;
	m_tRelativeTitleArea = panel.m_tRelativeTitleArea;
}

CUIPanel::~CUIPanel()
{
}

void CUIPanel::EnableTitle(bool bTitle)
{
	m_bEnableTitle = bTitle;
}

void CUIPanel::SetTitleArea(float l, float t, float r, float b)
{
	m_tRelativeTitleArea.l = l;
	m_tRelativeTitleArea.t = t;
	m_tRelativeTitleArea.r = r;
	m_tRelativeTitleArea.b = b;
}

bool CUIPanel::Init()
{
	m_pTransform->SetWorldScale(300.f, 200.f, 1.f);

	CRenderer2D*	pRenderer = m_pGameObject->AddComponent<CRenderer2D>("PanelRenderer");

	pRenderer->SetMesh("RectOrthoLTTex");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("VertexTex");
	pRenderer->SetRenderState(ALPHA_BLEND);

	CMaterial*	pMaterial = pRenderer->CreateMaterial(SAMPLER_LINEAR,
		"DefaultPanel", 0, 0, L"Panel.png");

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	CColliderRect*	pRC = m_pGameObject->AddComponent<CColliderRect>("PanelRC");

	pRC->SetCollisionGroup("UI");
	pRC->SetRect(0.f, 0.f, 300.f, 200.f);

	SAFE_RELEASE(pRC);

	m_bClick = false;

	EnableTitle(true);
	SetTitleArea(0.f, 0.f, 300.f, 30.f);

	return true;
}

void CUIPanel::Input(float fTime)
{
	CUI::Input(fTime);
}

int CUIPanel::Update(float fTime)
{
	CUI::Update(fTime);

	if (m_bClick)
	{
		if (KEYUP("MouseLButton"))
		{
			m_bClick = false;
		}

		else
		{
			POINT vMousePos = GET_SINGLE(CInput)->GetMousePos();
			
			Vector3	vPos = Vector3(m_vTitleDist.x + vMousePos.x,
				m_vTitleDist.y + vMousePos.y, 0.f);

			m_pTransform->SetWorldPos(vPos);
		}
	}

	return 0;
}

int CUIPanel::LateUpdate(float fTime)
{
	CUI::LateUpdate(fTime);

	if (m_bEnableTitle)
	{
		m_tTitleArea.l = m_tRelativeTitleArea.l + m_pTransform->GetWorldPos().x;
		m_tTitleArea.t = m_tRelativeTitleArea.t + m_pTransform->GetWorldPos().y;
		m_tTitleArea.r = m_tRelativeTitleArea.r + m_pTransform->GetWorldPos().x;
		m_tTitleArea.b = m_tRelativeTitleArea.b + m_pTransform->GetWorldPos().y;
	}

	return 0;
}

void CUIPanel::Collision(float fTime)
{
	CUI::Collision(fTime);
}

void CUIPanel::Render(float fTime)
{
	CUI::Render(fTime);
}

CUIPanel * CUIPanel::Clone()
{
	return new CUIPanel(*this);
}

void CUIPanel::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CUIPanel::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (m_bEnableTitle && pDest->GetTag() == "MousePoint")
	{
		if (KEYDOWN("MouseLButton") && !m_bClick)
		{
			Vector2	vPoint = ((CColliderPoint*)pDest)->GetPos();

			if (m_tTitleArea.l <= vPoint.x && vPoint.x <= m_tTitleArea.r &&
				m_tTitleArea.t <= vPoint.y && vPoint.y <= m_tTitleArea.b)
			{
				m_bClick = true;
				m_vTitleDist.x = m_pTransform->GetWorldPos().x -
					vPoint.x;
				m_vTitleDist.y = m_pTransform->GetWorldPos().y -
					vPoint.y;
			}
		}
	}
}

void CUIPanel::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
