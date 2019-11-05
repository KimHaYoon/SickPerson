#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CNavigation3D :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CNavigation3D();
	CNavigation3D(const CNavigation3D& nav);
	~CNavigation3D();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CNavigation3D* Clone();
};

Engine_END
