#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CScript :
	public CComponent
{
protected:
	friend class CGameObject;

protected:
	CScript();
	CScript(const CScript& script);
	virtual ~CScript();

public:
	virtual bool Init() = 0;
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CScript* Clone() = 0;
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};

Engine_END