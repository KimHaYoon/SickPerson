#pragma once
#include "../Obj.h"

class  CComponent :
	public CObj
{
protected:
	friend class CGameObject;

protected:
	CComponent();
	CComponent(const CComponent& com);
	virtual ~CComponent();

protected:
	class CGameObject*	m_pGameObject;
	class CLayer*		m_pLayer;
	class CScene*		m_pScene;
	class CTransform*	m_pTransform;
	COMPONENT_TYPE		m_eType;

public:
	COMPONENT_TYPE GetComponentType()	const;

public:
	void SetScene(class CScene* pScene);
	void SetLayer(class CLayer* pLayer);
	void SetGameObject(class CGameObject* pObj);
	void SetTransform(class CTransform* pTransform);

public:
	class CScene* GetScene()	const;
	class CLayer* GetLayer()	const;
	class CGameObject* GetGameObject()	const;
	class CTransform* GetTransform()	const;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CComponent* Clone() = 0;

public:
	template <typename T>
	T* FindComponentFromTag(const string& strTag)
	{
		return m_pGameObject->FindComponentFromTag<T>(strTag);
	}

	template <typename T>
	T* FindComponentFromTypeName(const string& strTypeName)
	{
		return m_pGameObject->FindComponentFromTypeName<T>(strTypeName);
	}

	template <typename T>
	T* FindComponentFromType(COMPONENT_TYPE eType)
	{
		return m_pGameObject->FindComponentFromType<T>(eType);
	}

	template <typename T>
	T* FindComponentFromTypeID()
	{
		return m_pGameObject->FindComponentFromTypeID<T>();
	}

public:
	const list<CComponent*>* FindComponentsFromTag(const string& strTag);
	const list<CComponent*>* FindComponentsFromTypeName(
		const string& strTypeName);
	const list<CComponent*>* FindComponentsFromType(COMPONENT_TYPE eType);

	template <typename T>
	const list<CComponent*>* FindComponentsFromTypeID()
	{
		return m_pGameObject->FindComponentsFromTypeID<T>();
	}
};