#include "UIButton.h"
#include "Renderer2D.h"
#include "ColliderRect.h"
#include "Material.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"

Engine_USING

CUIButton::CUIButton()
{
	SetTag("UIButton");
	SetTypeName("CUIButton");
	SetTypeID<CUIButton>();
}

CUIButton::CUIButton(const CUIButton & button)	:
	CUI(button)
{
	m_vNormalCol = button.m_vNormalCol;
	m_vMouseOnCol = button.m_vMouseOnCol;
	m_vClickCol = button.m_vClickCol;
	m_bCallback = false;
	m_eState = BS_NONE;
	m_tCBuffer.vColor = m_vMouseOnCol;
	m_tCBuffer.fLight = 1.f;
}

CUIButton::~CUIButton()
{
}

void CUIButton::SetCallback(void(*pFunc)(float))
{
	m_ButtonCallback = bind(pFunc, placeholders::_1);
	m_bCallback = true;
}

void CUIButton::SetNormalColor(const Vector4 & vColor)
{
	m_vNormalCol = vColor;
}

void CUIButton::SetMouseOnColor(const Vector4 & vColor)
{
	m_vMouseOnCol = vColor;
}

void CUIButton::SetClickColor(const Vector4 & vColor)
{
	m_vClickCol = vColor;
}

bool CUIButton::Init()
{
	m_pTransform->SetWorldScale(100.f, 100.f, 1.f);

	CRenderer2D*	pRenderer = m_pGameObject->AddComponent<CRenderer2D>("Renderer");

	pRenderer->SetShader(UI_COLOR_SHADER);
	pRenderer->SetInputLayout("VertexTex");
	pRenderer->SetMesh("RectOrthoLTTex");
	pRenderer->SetRenderState(ALPHA_BLEND);

	pRenderer->CreateCBuffer("Button", 11, sizeof(BUTTONCBUFFER),
		SCT_VERTEX | SCT_PIXEL);

	CMaterial* pMaterial = pRenderer->CreateMaterial(SAMPLER_LINEAR,
		"BigGoblin", 10, 0, L"Player.png");

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	CColliderRect*	pRC = m_pGameObject->AddComponent<CColliderRect>("Button");
	
	pRC->SetRect(0.f, 0.f, 100.f, 100.f);
	pRC->SetViewType(VT_UI);

	SAFE_RELEASE(pRC);

	m_vNormalCol = Vector4(0.8f, 0.8f, 0.8f, 1.f);
	m_vMouseOnCol = Vector4::Yellow;
	m_vClickCol = Vector4::Red;

	m_bCallback = false;
	m_eState = BS_NONE;


	m_tCBuffer.vColor = m_vMouseOnCol;
	m_tCBuffer.fLight = 1.f;

	return true;
}

void CUIButton::Input(float fTime)
{
	CUI::Input(fTime);
}

int CUIButton::Update(float fTime)
{
	CUI::Update(fTime);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_eState == BS_MOUSEON)
		{
			m_eState = BS_CLICK;
			m_tCBuffer.vColor = m_vClickCol;
		}
	}

	else
	{
		if (m_eState == BS_CLICK)
		{
			m_eState = BS_MOUSEON;
			m_tCBuffer.vColor = m_vMouseOnCol;

			if (m_bCallback)
				m_ButtonCallback(fTime);
		}
	}

	return 0;
}

int CUIButton::LateUpdate(float fTime)
{
	CUI::LateUpdate(fTime);

	m_tCBuffer.fLight = 1.f;

	CRenderer2D*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

	pRenderer->UpdateCBuffer("Button", 11, sizeof(BUTTONCBUFFER),
		SCT_VERTEX | SCT_PIXEL, &m_tCBuffer);

	SAFE_RELEASE(pRenderer);

	return 0;
}

void CUIButton::Collision(float fTime)
{
	CUI::Collision(fTime);
}

void CUIButton::Render(float fTime)
{
	CUI::Render(fTime);
}

CUIButton * CUIButton::Clone()
{
	return new CUIButton(*this);
}

void CUIButton::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MousePoint")
	{
		m_eState = BS_MOUSEON;
		m_tCBuffer.vColor = m_vMouseOnCol;
		CRenderer2D*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

		pRenderer->UpdateCBuffer("Button", 11, sizeof(BUTTONCBUFFER),
			SCT_VERTEX | SCT_PIXEL, &m_tCBuffer);

		SAFE_RELEASE(pRenderer);
	}
}

void CUIButton::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CUIButton::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "MousePoint")
	{
		m_eState = BS_NONE;
		m_tCBuffer.vColor = m_vNormalCol;
		CRenderer2D*	pRenderer = m_pGameObject->FindComponentFromTypeID<CRenderer2D>();

		pRenderer->UpdateCBuffer("Button", 11, sizeof(BUTTONCBUFFER),
			SCT_VERTEX | SCT_PIXEL, &m_tCBuffer);

		SAFE_RELEASE(pRenderer);
	}
}
