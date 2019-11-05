#pragma once
#include "UI.h"

Engine_BEGIN

class Engine_DLL CUIPanel :
	public CUI
{
protected:
	friend class CGameObject;

protected:
	CUIPanel();
	CUIPanel(const CUIPanel& panel);
	~CUIPanel();

private:
	bool	m_bEnableTitle;
	RECTINFO	m_tRelativeTitleArea;
	RECTINFO	m_tTitleArea;
	bool		m_bClick;
	Vector2		m_vTitleDist;

public:
	void EnableTitle(bool bTitle);
	void SetTitleArea(float l, float t, float r, float b);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CUIPanel* Clone();
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};

Engine_END
