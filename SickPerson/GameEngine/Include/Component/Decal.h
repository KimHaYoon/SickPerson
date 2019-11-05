#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CDecal :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CDecal();
	CDecal(const CDecal& decal);
	~CDecal();

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CDecal* Clone();
};

Engine_END