#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"
#include "../Core/Input.h"
#include "../GameObject/GameObject.h"
#include "../Component/Transform.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() :
	m_pCurScene(NULL),
	m_pNextScene(NULL)
{
}

CSceneManager::~CSceneManager()
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyObjList.end();

	for (iter = m_DontDestroyObjList.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter).pObj);
		SAFE_RELEASE((*iter).pTransform);
	}

	iterEnd = m_DontDestroyPrototypeList.end();
	for (iter = m_DontDestroyPrototypeList.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter).pObj);
	}

	SAFE_RELEASE(m_pNextScene);
	SAFE_RELEASE(m_pCurScene);
}

CScene * CSceneManager::CreateScene(const string & strTag)
{
	CScene*	pScene = new CScene;

	pScene->SetTag(strTag);

	if (!pScene->Init())
	{
		SAFE_RELEASE(pScene);
		return NULL;
	}

	pScene->AddRef();

	return pScene;
}

CScene * CSceneManager::GetCurrentScene() const
{
	m_pCurScene->AddRef();
	return m_pCurScene;
}

CScene * CSceneManager::CreateNextScene(const string & strTag)
{
	m_pNextScene = CreateScene(strTag);

	return m_pNextScene;
}

void CSceneManager::AddDontDestroyObj(CGameObject * pObj,
	const string & strLayerTag, int iZOrder)
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyObjList.end();

	for (iter = m_DontDestroyObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).pObj == pObj)
			return;
	}

	DONTDESTROYOBJ	tObj;
	pObj->AddRef();
	tObj.pObj = pObj;
	tObj.pTransform = pObj->GetTransform();
	tObj.strLayerTag = strLayerTag;
	tObj.iZOrder = iZOrder;
	tObj.bStart = false;

	m_DontDestroyObjList.push_back(tObj);
}

void CSceneManager::DeleteDontDestroyObj(CGameObject * pObj)
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyObjList.end();

	for (iter = m_DontDestroyObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).pObj == pObj)
		{
			SAFE_RELEASE((*iter).pObj);
			SAFE_RELEASE((*iter).pTransform);
			m_DontDestroyObjList.erase(iter);
			return;
		}
	}
}

CGameObject * CSceneManager::FindDontDestroyObj(const string & strTag)
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyObjList.end();

	for (iter = m_DontDestroyObjList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).pObj->GetTag() == strTag)
		{
			(*iter).pObj->AddRef();
			return (*iter).pObj;
		}
	}

	return NULL;
}

void CSceneManager::AddDontDestroyPrototype(CGameObject * pObj,
	const string & strTag)
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyPrototypeList.end();

	for (iter = m_DontDestroyPrototypeList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).pObj == pObj)
			return;
	}

	DONTDESTROYOBJ	tObj;
	pObj->AddRef();
	tObj.pObj = pObj;
	tObj.strLayerTag = strTag;

	m_DontDestroyPrototypeList.push_back(tObj);
}

void CSceneManager::DeleteDontDestroyPrototype(CGameObject * pObj)
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyPrototypeList.end();

	for (iter = m_DontDestroyPrototypeList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).pObj == pObj)
		{
			SAFE_RELEASE((*iter).pObj);
			m_DontDestroyPrototypeList.erase(iter);
			return;
		}
	}
}

CGameObject * CSceneManager::FindDontDestroyPrototype(const string & strTag)
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyPrototypeList.end();

	for (iter = m_DontDestroyPrototypeList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).pObj->GetTag() == strTag)
		{
			(*iter).pObj->AddRef();
			return (*iter).pObj;
		}
	}

	return NULL;
}

void CSceneManager::Start()
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyObjList.end();

	for (iter = m_DontDestroyObjList.begin(); iter != iterEnd; ++iter)
	{
		(*iter).bStart = true;
	}
}

bool CSceneManager::Init()
{
	m_pCurScene = CreateScene("DefaultScene");
	m_pCurScene->Release();

	return true;
}

void CSceneManager::Input(float fTime)
{
	m_pCurScene->Input(fTime);
}

SCENE_CHANGE CSceneManager::Update(float fTime)
{
	m_pCurScene->Update(fTime);

	return ChangeScene();
}

SCENE_CHANGE CSceneManager::LateUpdate(float fTime)
{
	m_pCurScene->LateUpdate(fTime);

	return ChangeScene();
}

void CSceneManager::Collision(float fTime)
{
	m_pCurScene->Collision(fTime);
}

void CSceneManager::Render(float fTime)
{
	m_pCurScene->Render(fTime);
}

SCENE_CHANGE CSceneManager::ChangeScene()
{
	if (m_pNextScene)
	{
		// 기존 장면을 지운다.
		SAFE_RELEASE(m_pCurScene);

		AddDontDestroyObj();

		AddDontDestroyPrototype();

		m_pCurScene = m_pNextScene;
		m_pNextScene = NULL;

		return SC_CHANGE;
	}

	return SC_NONE;
}

void CSceneManager::AddDontDestroyObj()
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyObjList.end();

	for (iter = m_DontDestroyObjList.begin(); iter != iterEnd; ++iter)
	{
		if (!(*iter).bStart)
		{
			(*iter).bStart = true;
			continue;
		}

		CLayer*	pLayer = m_pNextScene->GetLayer((*iter).strLayerTag);

		if (!pLayer)
			pLayer = m_pNextScene->CreateLayer((*iter).strLayerTag, (*iter).iZOrder);

		pLayer->AddObject((*iter).pObj);

		CGameObject::AddObjList((*iter).pObj);

		SAFE_RELEASE(pLayer);
	}
}

void CSceneManager::AddDontDestroyPrototype()
{
	list<DONTDESTROYOBJ>::iterator	iter;
	list<DONTDESTROYOBJ>::iterator	iterEnd = m_DontDestroyPrototypeList.end();

	for (iter = m_DontDestroyPrototypeList.begin(); iter != iterEnd; ++iter)
	{
		(*iter).pObj->SetScene(m_pNextScene);

		CGameObject::AddPrototype((*iter).strLayerTag, (*iter).pObj);
	}
}
