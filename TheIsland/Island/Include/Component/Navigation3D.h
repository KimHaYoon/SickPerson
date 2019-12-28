#pragma once
#include "Component.h"

GAME_BEGIN

class GAME_DLL CNavigation3D :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CNavigation3D();
	CNavigation3D(const CNavigation3D& nav);
	~CNavigation3D();

private:
	Vector3		m_vStart;
	Vector3		m_vEnd;
	bool		m_bFind;
	list<Vector3>	m_PathList;
	bool		m_bMove;
	Vector3		m_vTargetPos;

public:
	void FindNavigation(const Vector3& vStart, const Vector3& vEnd);
	void FindNavigation(const Vector3& vStart, const Vector3& vOrigin,
		const Vector3& vDir);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CNavigation3D* Clone();
};

GAME_END
