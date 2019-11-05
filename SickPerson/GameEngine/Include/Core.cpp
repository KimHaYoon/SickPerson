#include "Core.h"
#include "Device.h"
#include "Core/Timer.h"
#include "Core/TimerManager.h"
#include "Scene/SceneManager.h"
#include "Core/Input.h"
#include "Core/CollisionManager.h"
#include "Core/PathManager.h"
#include "Core/Scheduler.h"
#include "Core/NavigationManager.h"
#include "Resources/ResourcesManager.h"
#include "Rendering/RenderManager.h"
#include "Resources/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderManager.h"
#include "GameObject/GameObject.h"

Engine_USING

DEFINITION_SINGLE(CCore)

bool CCore::m_bLoop = true;

CCore::CCore()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(147);

#ifdef _DEBUG
	//AllocConsole();
#endif
}

CCore::~CCore()
{
	DESTROY_SINGLE(CScheduler);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CNavigationManager);
	DESTROY_SINGLE(CRenderManager);
	DESTROY_SINGLE(CResourcesManager);
	DESTROY_SINGLE(CInput);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CTimerManager);
	DESTROY_SINGLE(CPathManager);
	DESTROY_SINGLE(CDevice);

#ifdef _DEBUG
	//FreeConsole();
#endif // _DEBUG
}

HWND CCore::GetWindowHandle() const
{
	return m_hWnd;
}

bool CCore::Init(HINSTANCE hInst, TCHAR * pTitle, TCHAR * pClass, int iIconID,
	UINT iWidth, UINT iHeight, bool bWindowMode, bool bOnMouseRenderer)
{
	m_hInst = hInst;

	WindowRegisterClass(pClass, iIconID);

	InitWindow(pClass, pTitle, iWidth, iHeight);

	return Init(hInst, m_hWnd, iWidth, iHeight, bWindowMode, bOnMouseRenderer);
}

bool CCore::Init(HINSTANCE hInst, HWND hWnd, UINT iWidth,
	UINT iHeight, bool bWindowMode, bool bOnMouseRenderer)
{
	m_tResolution.iWidth = iWidth;
	m_tResolution.iHeight = iHeight;
	m_hInst = hInst;
	m_hWnd = hWnd;

	if (!GET_SINGLE(CDevice)->Init(m_hWnd, iWidth, iHeight, bWindowMode))
		return false;

	if (!GET_SINGLE(CScheduler)->Init())
		return false;

	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	if (!GET_SINGLE(CResourcesManager)->Init())
		return false;

	if (!GET_SINGLE(CRenderManager)->Init())
		return false;

	if (!GET_SINGLE(CInput)->Init(m_hWnd, bOnMouseRenderer))
		return false;

	if (!GET_SINGLE(CTimerManager)->Init())
		return false;

	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	return true;
}

int CCore::Run()
{
	GET_SINGLE(CSceneManager)->Start();
	MSG msg;

	while (m_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우 데드타임일 때는 else  구문으로 들어온다.
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

int CCore::RunTool()
{
	Logic();

	return 0;
}

void CCore::Logic()
{
	CTimer*	pTimer = GET_SINGLE(CTimerManager)->FindTimer("MainThread");

	pTimer->Update();

	float fTime = pTimer->GetDeltaTime();

	SAFE_RELEASE(pTimer);

	GET_SINGLE(CScheduler)->Update(fTime);

	Input(fTime);
	if (Update(fTime) == SC_CHANGE)
	{
		GET_SINGLE(CInput)->ClearWheel();
		return;
	}
	if (LateUpdate(fTime) == SC_CHANGE)
	{
		GET_SINGLE(CInput)->ClearWheel();
		return;
	}
	Collision(fTime);
	Render(fTime);
}

void CCore::Input(float fTime)
{
	GET_SINGLE(CInput)->Update(fTime);
	CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
	pMouseObj->Input(fTime);
	SAFE_RELEASE(pMouseObj);
	GET_SINGLE(CSceneManager)->Input(fTime);
}

int CCore::Update(float fTime)
{
	CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
	pMouseObj->Update(fTime);
	SAFE_RELEASE(pMouseObj);
	return GET_SINGLE(CSceneManager)->Update(fTime);
}

int CCore::LateUpdate(float fTime)
{
	CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
	pMouseObj->LateUpdate(fTime);
	SAFE_RELEASE(pMouseObj);
	return GET_SINGLE(CSceneManager)->LateUpdate(fTime);
}

void CCore::Collision(float fTime)
{
	GET_SINGLE(CSceneManager)->Collision(fTime);

	GET_SINGLE(CCollisionManager)->Collision(fTime);

	// 절두체 컬링
}

void CCore::Render(float fTime)
{
	GET_SINGLE(CDevice)->ClearTarget();

	GET_SINGLE(CSceneManager)->Render(fTime);

	GET_SINGLE(CRenderManager)->Render(fTime);

	CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
	pMouseObj->Render(fTime);
	SAFE_RELEASE(pMouseObj);

	GET_SINGLE(CDevice)->Present();

	GET_SINGLE(CInput)->ClearWheel();
}

ATOM CCore::WindowRegisterClass(TCHAR * pClass, int iIconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CCore::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(iIconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_MY170825);
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iIconID));

	return RegisterClassExW(&wcex);
}

BOOL CCore::InitWindow(TCHAR * pClass, TCHAR * pTitle, UINT iWidth, UINT iHeight)
{
	HWND hWnd = CreateWindowW(pClass, pTitle, WS_OVERLAPPEDWINDOW,
		300, 0, iWidth, iHeight, nullptr, nullptr, m_hInst, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	// 윈도우의 클라이언트 영역을 원하는 크기로 설정하는 방법
	RECT	rc = { 0, 0, iWidth, iHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(hWnd, HWND_TOPMOST, 300, 0, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	m_hWnd = hWnd;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:	// 키를 누를때 들어오는 메세지이다.
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	case WM_MOUSEWHEEL:
		GET_SINGLE(CInput)->SetWheel(HIWORD(wParam));
		break;
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
