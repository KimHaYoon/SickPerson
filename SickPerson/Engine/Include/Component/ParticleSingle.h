#pragma once
#include "Component.h"

Engine_BEGIN

class Engine_DLL CParticleSingle :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CParticleSingle();
	CParticleSingle(const CParticleSingle& particle);
	~CParticleSingle();

private:
	class CRenderer*	m_pRenderer;
	Vector2				m_vSize;

public:
	void SetSize(const Vector2& vSize);
	void SetSize(float x, float y);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CParticleSingle* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

Engine_END
