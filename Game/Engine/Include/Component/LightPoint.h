#pragma once
#include "Light.h"

Engine_BEGIN

class Engine_DLL CLightPoint :
	public CLight
{
private:
	friend class CGameObject;

private:
	CLightPoint();
	CLightPoint(const CLightPoint& light);
	~CLightPoint();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLightPoint* Clone();
};

Engine_END
