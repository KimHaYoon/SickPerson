#pragma once
#include "../Obj.h"

Engine_BEGIN

class Engine_DLL CRenderState : public CObj
{
protected:
	friend class CRenderManager;

protected:
	CRenderState();
	~CRenderState();

protected:
	ID3D12DeviceChild*		m_pState;
	ID3D12DeviceChild*		m_pOldState;
	RENDERSTATE_TYPE	m_eType;
	string								m_strKey;

public:
	RENDERSTATE_TYPE	GetType()	const;
	string								GetKey()	const;

public:
	virtual	void		SetState() = 0;
	virtual	void		ResetState() = 0;
};

Engine_END