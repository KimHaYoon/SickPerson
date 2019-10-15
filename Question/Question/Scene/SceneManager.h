#pragma once

#include "../Engine.h"


typedef struct  _tagDontDestroyObj
{
	class CGameObject*	pObj;
	class CTransform*	pTransform;
	string		strLayerTag;
	int			iZOrder;
	bool		bStart;
}DONTDESTROYOBJ, *PDONTDESTROYOBJ;

class  CSceneManager
{

private:
	class CScene*	m_pCurScene;
	class CScene*	m_pNextScene;
	list<DONTDESTROYOBJ>	m_DontDestroyObjList;
	list<DONTDESTROYOBJ>	m_DontDestroyPrototypeList;

public:
	class CScene* CreateScene(const string& strTag);
	class CScene* GetCurrentScene()	const;
	class CScene* CreateNextScene(const string& strTag);

public:
	void AddDontDestroyObj(class CGameObject* pObj, const string& strLayerTag,
		int iZOrder);
	void DeleteDontDestroyObj(class CGameObject* pObj);
	class CGameObject* FindDontDestroyObj(const string& strTag);
	void AddDontDestroyPrototype(class CGameObject* pObj, const string& strTag);
	void DeleteDontDestroyPrototype(class CGameObject* pObj);
	class CGameObject* FindDontDestroyPrototype(const string& strTag);

	void Start();

public:
	bool Init();
	void Input(float fTime);
	SCENE_CHANGE Update(float fTime);
	SCENE_CHANGE LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);

private:
	SCENE_CHANGE ChangeScene();
	void AddDontDestroyObj();
	void AddDontDestroyPrototype();

	DECLARE_SINGLE(CSceneManager)
};