#pragma once
#include "Light.h"

Engine_BEGIN

class Engine_DLL CLightSpot :
	public CLight
{
protected:
	friend class CGameObject;

protected:
	CLightSpot();
	CLightSpot(const CLightSpot& light);
	~CLightSpot();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLightSpot* Clone();
};

Engine_END
