#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CMouse :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CMouse();
	CMouse(const CMouse& mouse);
	~CMouse();

private:
	BOOL	m_bMouseShow;
	Vector3	m_vRayPos;
	Vector3	m_vRayDir;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CMouse* Clone();
};

Engine_END
