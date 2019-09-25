#pragma once

#include "Engine.h"

class  CObj
{
protected:
	CObj();
	virtual ~CObj();

protected:
	int					m_iRefCount;
	string				m_strTag;
	string				m_strTypeName;
	const type_info*	m_pTypeID;
	bool				m_bEnable;
	bool				m_bAlive;

public:
	string GetTag()	const;
	string GetTypeName()	const;
	const type_info* GetTypeID()	const;
	bool GetEnable()	const;
	bool GetAlive()	const;

public:
	void SetTag(const string& strTag);
	void SetTypeName(const string& strTypeName);
	template <typename T>
	void SetTypeID()
	{
		m_pTypeID = &typeid(T);
	}

	void Enable(bool bEnable);
	void Die();

public:
	int Release();
	void AddRef();
};
