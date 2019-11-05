#pragma once
#include "UI.h"

Engine_BEGIN

enum Engine_DLL BUTTON_STATE
{
	BS_NONE,
	BS_MOUSEON,
	BS_CLICK
};

class Engine_DLL CUIButton :
	public CUI
{
protected:
	friend class CGameObject;

protected:
	CUIButton();
	CUIButton(const CUIButton& button);
	~CUIButton();

protected:
	Vector4		m_vNormalCol;
	Vector4		m_vMouseOnCol;
	Vector4		m_vClickCol;
	BUTTONCBUFFER	m_tCBuffer;
	function<void(float)>	m_ButtonCallback;
	bool		m_bCallback;
	BUTTON_STATE	m_eState;

public:
	template <typename T>
	void SetCallback(T* pObj, void(T::*pFunc)(float))
	{
		m_ButtonCallback = bind(pFunc, pObj, placeholders::_1);
		m_bCallback = true;
	}

	void SetCallback(void(*pFunc)(float));

	void SetNormalColor(const Vector4& vColor);
	void SetMouseOnColor(const Vector4& vColor);
	void SetClickColor(const Vector4& vColor);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CUIButton* Clone();
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};

Engine_END
