#include "Input.h"
#include "../Device.h"
//#include "../GameObject/GameObject.h"
//#include "../Component/Renderer2D.h"
//#include "../Component/Material.h"
//#include "../Component/Animation2D.h"
//#include "../Component/Mouse.h"
//#include "../Component/Transform.h"
//#include "../Scene/Scene.h"
//#include "../Scene/SceneManager.h"

Engine_USING

DEFINITION_SINGLE(CInput)

CInput::CInput() :
	m_pCreate(NULL),
	m_pMouseObj(NULL),
	m_pMouseTr(NULL),
	m_bMouseClip(false),
	m_sWheel(0),
	m_bFix(false)
{
}

CInput::~CInput()
{
	CGameObject::EraseObj(m_pMouseObj);
	SAFE_RELEASE(m_pMouseTr);
	SAFE_RELEASE(m_pMouseObj);
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();
	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapKey.clear();
}

POINT CInput::GetMousePos() const
{
	return m_ptMousePos;
}

Vector2 CInput::GetWorldMousePos() const
{
	return m_vWorldMousePos;
}

POINT CInput::GetMouseMove() const
{
	return m_ptMouseMove;
}

CGameObject * CInput::GetMouseObj() const
{
	m_pMouseObj->AddRef();
	return m_pMouseObj;
}

bool CInput::GetFix() const
{
	return m_bFix;
}

void CInput::SetFix(bool bFix)
{
	m_bFix = bFix;
}

void CInput::SetMouseClip(bool bClip)
{
	m_bMouseClip = bClip;
}

short CInput::GetWheel() const
{
	return m_sWheel;
}

void CInput::SetWheel(short sWheel)
{
	m_sWheel = sWheel / 120;

	if (m_sWheel > 1)
		m_sWheel = 1;

	else if (m_sWheel < -1)
		m_sWheel = -1;
}

void CInput::ClearWheel()
{
	m_sWheel = 0;
}

bool CInput::Init(HWND hWnd, bool bOnMouseRenderer)
{
	m_hWnd = hWnd;
	m_bOnMouseRenderer = bOnMouseRenderer;

	CreateKey("MoveFront", 'W');
	CreateKey("MoveBack", 'S');
	CreateKey("RotInvY", 'A');
	CreateKey('D', "RotY");
	CreateKey(VK_CONTROL, '1', "Skill1");
	CreateKey(VK_CONTROL, "Ctrl");
	CreateKey(VK_LBUTTON, "MouseLButton");
	CreateKey(VK_RBUTTON, "MouseRButton");

	// 아래 함수는 스크린좌표로 나온다.
	GetCursorPos(&m_ptMousePos);
	// 스크린 좌표를 클라이언트 좌표로 바꾼다.
	ScreenToClient(m_hWnd, &m_ptMousePos);

	Vector2	vRS = GET_SINGLE(CDevice)->GetWindowDeviceResolution();

	m_ptMousePos.x *= vRS.x;
	m_ptMousePos.y *= vRS.y;

	m_pMouseObj = CGameObject::CreateObject("MouseObj");

	CMouse*	pMouse = m_pMouseObj->AddComponent<CMouse>("Mouse");

	SAFE_RELEASE(pMouse);

	m_pMouseTr = m_pMouseObj->GetTransform();

	return true;
}

void CInput::Update(float fTime)
{
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int	iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
				++iPushCount;
		}

		// 벡터에 있는 키를 모두 눌렀을 경우
		if (iPushCount == iter->second->vecKey.size())
		{
			if (!iter->second->bDown && !iter->second->bPush)
			{
				iter->second->bPush = true;
				iter->second->bDown = true;
			}

			else
			{
				iter->second->bDown = false;
			}
		}

		// 이전 프레임에 이 키를 누르고 있었을 경우
		else if (iter->second->bDown || iter->second->bPush)
		{
			iter->second->bUp = true;
			iter->second->bDown = false;
			iter->second->bPush = false;
		}

		else if (iter->second->bUp)
		{
			iter->second->bUp = false;
		}
	}

	POINT	ptMousePos;
	// 아래 함수는 스크린좌표로 나온다.
	GetCursorPos(&ptMousePos);
	// 스크린 좌표를 클라이언트 좌표로 바꾼다.
	ScreenToClient(m_hWnd, &ptMousePos);

	Vector2	vRS = GET_SINGLE(CDevice)->GetWindowDeviceResolution();

	ptMousePos.x *= vRS.x;
	ptMousePos.y *= vRS.y;

	m_ptMouseMove.x = ptMousePos.x - m_ptMousePos.x;
	m_ptMouseMove.y = ptMousePos.y - m_ptMousePos.y;

	m_ptMousePos = ptMousePos;

	//m_pMouseTr->SetWorldPos(ptMousePos.x, ptMousePos.y, 0.f);
}

PKEYINFO CInput::FindKey(const string & strKey)	const
{
	unordered_map<string, PKEYINFO>::const_iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}

bool CInput::KeyDown(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bDown;
}

bool CInput::KeyPush(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bPush;
}

bool CInput::KeyUp(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bUp;
}
