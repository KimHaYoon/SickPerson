#pragma once
#include "Light.h"

GAME_BEGIN

class GAME_DLL CLightDir :
	public CLight
{
protected:
	friend class CGameObject;

protected:
	CLightDir();
	CLightDir(const CLightDir& light);
	~CLightDir();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLightDir* Clone();
};

GAME_END
