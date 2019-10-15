#pragma once
#include "../Obj.h"

class  CSceneScript :
	public CObj
{
protected:
	friend class CScene;

protected:
	CSceneScript();
	virtual ~CSceneScript();

protected:
	class CScene*	m_pScene;

public:
	void SetScene(class CScene* pScene);

public:
	virtual bool Init() = 0;
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
};