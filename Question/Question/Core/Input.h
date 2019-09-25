#pragma once

#include "../Engine.h"

Engine_BEGIN

typedef struct Engine_DLL _tagKeyInfo
{
	string	strName;
	bool	bDown;
	bool	bPush;
	bool	bUp;
	vector<DWORD>	vecKey;
}KEYINFO, *PKEYINFO;

class Engine_DLL CInput
{
private:
	unordered_map<string, PKEYINFO>		m_mapKey;
	PKEYINFO	m_pCreate;
	HWND		m_hWnd;
	POINT		m_ptMousePos;
	Vector2		m_vWorldMousePos;
	POINT		m_ptMouseMove;
	class CGameObject*	m_pMouseObj;
	class CTransform*	m_pMouseTr;
	bool		m_bMouseClip;
	bool		m_bOnMouseRenderer;
	short		m_sWheel;
	bool		m_bFix;

public:
	POINT GetMousePos()	const;
	Vector2 GetWorldMousePos()	const;
	POINT GetMouseMove()	const;
	class CGameObject* GetMouseObj()	const;
	bool GetFix()	const;
	void SetFix(bool bFix);
	void SetMouseClip(bool bClip);
	short GetWheel()	const;
	void SetWheel(short sWheel);
	void ClearWheel();

public:
	bool Init(HWND hWnd, bool bOnMouseRenderer = true);
	void Update(float fTime);

public:
	// 가장 마지막 인자가 들어올 종료조건 함수를 만들어준다.
	template <typename T>
	void CreateKey(const T& data)
	{
		const char* pName = typeid(T).name();
		// 현재 들어온 타입이 어떤 타입인지를 알아본다.
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
		// 현재 들어온 타입이 어떤 타입인지를 알아본다.
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

		// 여기로 오게되면 모든 인자에 대해서 재귀함수 호출이 끝났다는 것이다.
		m_pCreate = NULL;
	}

	void Update(float fTime);
	PKEYINFO FindKey(const string& strKey)	const;

public:
	bool KeyDown(const string& strKey)	const;
	bool KeyPush(const string& strKey)	const;
	bool KeyUp(const string& strKey)	const;
	DECLARE_SINGLE(CInput)
};

Engine_END