#pragma once
#include "../Obj.h"

class  CGameObject :
	public CObj
{
private:
	friend class CScene;

private:
	static list<CGameObject*>	m_ObjList;
	static list<CGameObject*>	m_FindObjectList;
	static unordered_map<string, CGameObject*>	m_mapPrototype;

public:
	static CGameObject* CreateObject(const string& strTag, class CLayer* pLayer = NULL);
	static CGameObject* CreatePrototype(const string& strKey, class CScene* pScene = NULL);
	static CGameObject* CreateClone(const string& strKey, class CLayer* pLayer = NULL);
	static CGameObject* CreateObjectDontDestroy(const string& strTag,
		class CLayer* pLayer);
	static CGameObject* CreatePrototypeDontDestroy(const string& strKey,
		class CScene* pScene);
	static void ChangePrototypeScene(const string& strKey, class CScene* pScene);
	static void AddPrototype(const string& strTag, CGameObject* pPrototype);
	static void AddObjList(CGameObject* pObj);
	static void ChangeLayer(const string& strTag, class CLayer* pLayer);
	static void DontDestroy(const string& strTag, bool bDontDesetroy = true);
	static void EraseObj(CGameObject* pObj);
	static void EraseObj(const string& strTag);
	static void EraseObj();
	static void ErasePrototype(class CScene* pScene);
	static void ErasePrototype(CGameObject* pObj);
	static void ErasePrototype(const string& strKey);
	static void ErasePrototype();
	static CGameObject* FindObject(const string& strTag);
	static CGameObject* FindObject(COMPONENT_TYPE eType);
	static const list<CGameObject*>* FindObjects(const string& strTag);

private:
	static CGameObject* FindPrototype(const string& strKey);

private:
	CGameObject();
	CGameObject(const CGameObject& obj);
	~CGameObject();

private:
	class CScene*	m_pScene;
	class CLayer*	m_pLayer;
	class CTransform*	m_pTransform;
	list<class CComponent*>	m_ComList;
	list<class CComponent*>	m_FindList;
	list<CGameObject*>		m_ChildList;
	CGameObject*			m_pParent;
	bool					m_bDontDestroy;
	bool	m_bCulling;

public:
	void SetCulling(bool bCulling);
	bool GetCulling()	const;
	void DontDestroyOnLoad(bool bDontDestroy = true);
	bool IsDontDestroy()	const;
	void AddChild(CGameObject* pChild);
	void DeleteChild(const string& strTag);
	void DeleteChild(CGameObject* pChild);
	void DeleteParent();
	CGameObject* GetParent()	const;
	void ChangeLayer(class CLayer* pLayer);

public:
	class CTransform* GetTransform()	const;

public:
	void SetScene(class CScene* pScene);
	void SetLayer(class CLayer* pLayer);

public:
	bool Init();
	void Input(float fTime);
	int Update(float fTime);
	int LateUpdate(float fTime);
	void Collision(float fTime);
	void Render(float fTime);
	CGameObject* Clone();

public:
	void UpdateTransformHierarchy();

public:
	template <typename T>
	T* FindComponentFromTag(const string& strTag)
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}

		return NULL;
	}

	template <typename T>
	T* FindComponentFromTypeName(const string& strTypeName)
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTypeName() == strTypeName)
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}

		return NULL;
	}

	template <typename T>
	T* FindComponentFromType(COMPONENT_TYPE eType)
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetComponentType() == eType)
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}

		return NULL;
	}

	template <typename T>
	T* FindComponentFromTypeID()
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTypeID() == &typeid(T))
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}

		return NULL;
	}

public:
	const list<CComponent*>* FindComponentsFromTag(const string& strTag);
	const list<CComponent*>* FindComponentsFromTypeName(
		const string& strTypeName);
	const list<CComponent*>* FindComponentsFromType(COMPONENT_TYPE eType);

	template <typename T>
	const list<CComponent*>* FindComponentsFromTypeID()
	{
		Safe_Release_VecList(m_FindList);
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTypeID() == &typeid(T))
			{
				(*iter)->AddRef();
				m_FindList.push_back(*iter);
			}
		}

		return &m_FindList;
	}

public:
	bool CheckComponentFromTag(const string& strTag);
	bool CheckComponentFromTypeName(const string& strTypeName);
	bool CheckComponentFromType(COMPONENT_TYPE eType);

	template <typename T>
	bool CheckComponentFromTypeID()
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_ComList.end();

		for (iter = m_ComList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTypeID() == &typeid(T))
			{
				return true;
			}
		}

		return false;
	}

public:
	CComponent* AddComponent(CComponent* pComponent);

	template <typename T>
	T* AddComponent(const string& strTag)
	{
		T*	pCom = NULL;

		if (m_bDontDestroy)
		{
			pCom = FindComponentFromTag<T>(strTag);

			if (pCom)
				return pCom;
		}

		pCom = new T;

		pCom->SetTag(strTag);
		pCom->SetScene(m_pScene);
		pCom->SetLayer(m_pLayer);
		pCom->SetGameObject(this);
		pCom->SetTransform(m_pTransform);

		if (!pCom->Init())
		{
			SAFE_RELEASE(pCom);
			return NULL;
		}

		return (T*)AddComponent(pCom);
	}
};