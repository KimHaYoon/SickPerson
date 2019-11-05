#include "UI.h"

Engine_USING

CUI::CUI()
{
	SetTag("UI");
	SetTypeName("CUI");
	SetTypeID<CUI>();
	m_eType = CT_UI;
	m_iZOrder = 0;
}


CUI::CUI(const CUI & ui)	:
	CComponent(ui)
{
	m_iZOrder = ui.m_iZOrder;
}

CUI::~CUI()
{
}

void CUI::SetZOrder(int iZOrder)
{
	m_iZOrder = iZOrder;
}

int CUI::GetZOrder() const
{
	return m_iZOrder;
}

void CUI::Input(float fTime)
{
}

int CUI::Update(float fTime)
{
	return 0;
}

int CUI::LateUpdate(float fTime)
{
	return 0;
}

void CUI::Collision(float fTime)
{
}

void CUI::Render(float fTime)
{
}
