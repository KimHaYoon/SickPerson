#pragma once

#include "Component/Script.h"
#include "Component/Animation.h"

Engine_USING

class CPlayer :
	public CScript
{
public:
	CPlayer();
	CPlayer(const CPlayer& player);
	~CPlayer();

private:
	CAnimation*		m_pAnimation;
	float			m_fMoveSpeed;
	float			m_fRotSpeed;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CPlayer* Clone();
};