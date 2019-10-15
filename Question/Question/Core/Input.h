#pragma once

#include "../Engine.h"

typedef struct  _tagKeyInfo
{
	string	strName;
	bool	bDown;
	bool	bPush;
	bool	bUp;
	vector<DWORD>	vecKey;
}KEYINFO, *PKEYINFO;

class CInput
{
private:
	unordered_map<string, PKEYINFO>		m_mapKey;
	PKEYINFO	m_pCreate;
	HWND		m_hWnd;

public:
	bool Init(HWND hWnd, bool bOnMouseRenderer = true);
	void Update(float fTime);

public:
	// ���� ������ ���ڰ� ���� �������� �Լ��� ������ش�.
	template <typename T>
	void CreateKey(const T& data)
	{
		const char* pName = typeid(T).name();
		// ���� ���� Ÿ���� � Ÿ�������� �˾ƺ���.
		if (strcmp(pName, "int") == 0 ||
			strcmp(pName, "char") == 0)
		{
			m_pCreate->vecKey.push_back((DWORD)data);
		}

		else
		{
			m_pCreate->strName = data;
			m_mapKey.insert(make_pair(m_pCreate->strName, m_pCreate));
		}
	}

	template <typename T, typename ... Types>
	void CreateKey(const T& first, const Types& ...argv)
	{
		if (!m_pCreate)
		{
			m_pCreate = new KEYINFO;
			m_pCreate->bDown = false;
			m_pCreate->bPush = false;
			m_pCreate->bUp = false;
			m_pCreate->strName = "";
		}

		const char* pName = typeid(T).name();
		// ���� ���� Ÿ���� � Ÿ�������� �˾ƺ���.
		if (strcmp(pName, "int") == 0 ||
			strcmp(pName, "char") == 0)
		{
			m_pCreate->vecKey.push_back((DWORD)first);
		}

		else
		{
			m_pCreate->strName = first;
			m_mapKey.insert(make_pair(m_pCreate->strName, m_pCreate));
		}

		CreateKey(argv...);

		// ����� ���ԵǸ� ��� ���ڿ� ���ؼ� ����Լ� ȣ���� �����ٴ� ���̴�.
		m_pCreate = NULL;
	}

	PKEYINFO FindKey(const string& strKey)	const;

public:
	bool KeyDown(const string& strKey)	const;
	bool KeyPush(const string& strKey)	const;
	bool KeyUp(const string& strKey)	const;
	DECLARE_SINGLE(CInput)
};