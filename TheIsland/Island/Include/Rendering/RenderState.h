#pragma once
#include "../Obj.h"

GAME_BEGIN

class GAME_DLL CRenderState :
	public CObj
{
protected:
	friend class CRenderManager;

protected:
	CRenderState();
	virtual ~CRenderState();

protected:
	ID3D11DeviceChild*	m_pState;
	ID3D11DeviceChild*	m_pOldState;
	RENDERSTATE_TYPE	m_eType;
	string				m_strKey;

public:
	RENDERSTATE_TYPE GetType()	const;
	string GetKey()	const;

public:
	virtual void SetState() = 0;
	virtual void ResetState() = 0;
};

GAME_END
