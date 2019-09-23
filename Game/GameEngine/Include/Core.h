#pragma once

#include "Engine.h"

Engine_BEGIN

class Engine_DLL CCore
{
private:
	static bool	m_bLoop;

private:
	HINSTANCE		m_hInst;
	HWND			m_hWnd;
	RESOLUTION		m_tResolution;

public:
	HWND GetWindowHandle()	const;

public:
	bool Init( HINSTANCE hInst, const TCHAR * pTitle, const  TCHAR * pClass, int iIconID,
		UINT iWidth, UINT iHeight, bool bWindowMode, bool bOnMouseRenderer = true );
	bool Init( HINSTANCE hInst, HWND hWnd, UINT iWidth,
		UINT iHeight, bool bWindowMode, bool bOnMouseRenderer = true );
	int Run();
	int RunTool();

private:
	void Logic();
	void Input( float fTime );
	int Update( float fTime );
	int LateUpdate( float fTime );
	void Collision( float fTime );
	void Render( float fTime );

private:
	ATOM WindowRegisterClass( const TCHAR* pClass, int iIconID );
	BOOL InitWindow( const TCHAR* pClass, const TCHAR* pTitle, UINT iWidth, UINT iHeight );

public:
	static LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

	DECLARE_SINGLE( CCore )
};

Engine_END