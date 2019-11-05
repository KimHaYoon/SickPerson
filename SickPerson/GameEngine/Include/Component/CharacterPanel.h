#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CCharacterPanel :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CCharacterPanel();
	CCharacterPanel(const CCharacterPanel& panel);
	~CCharacterPanel();

private:
	class CGameObject*	m_pTarget;
	class CTransform*	m_pTargetTr;
	class CCamera*		m_pTargetCamera;
	class CTransform*	m_pTargetCameraTr;
	bool				m_bAddResourceView;

public:
	void SetTarget(class CGameObject*	pTarget);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CCharacterPanel* Clone();
};

Engine_END