#pragma once

#include "Engine.h"

class  CCore
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
	bool Init( HINSTANCE hInst, const wchar_t * pTitle, const  wchar_t * pClass, int iIconID,
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
	ATOM WindowRegisterClass( const wchar_t* pClass, int iIconID );
	BOOL InitWindow( const wchar_t* pClass, const wchar_t* pTitle, UINT iWidth, UINT iHeight );

public:
	static LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
	DECLARE_SINGLE( CCore )
};