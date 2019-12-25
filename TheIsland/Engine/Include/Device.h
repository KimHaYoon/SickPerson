#pragma once
#include "Engine.h"

GAME_BEGIN

class GAME_DLL CDevice
{
private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRTView;
	ID3D11Texture2D*		m_pDepthBuffer;
	ID3D11DepthStencilView*	m_pDSView;
	float	m_fClearColor[4];
	RESOLUTION				m_tRS;
	HWND					m_hWnd;

public:
	ID3D11Device* GetDevice()	const;
	ID3D11DeviceContext* GetContext()	const;
	IDXGISwapChain* GetSwapChain()	const;
	RESOLUTION GetResolution()	const;

public:
	bool Init( HWND hWnd, UINT iWidth, UINT iHeight,
		bool bWindowMode );
	void ClearTarget();
	void Present();

public:
	Vector2 GetWindowDeviceResolution()	const;
	Vector2 GetWindowResolution()	const;

	DECLARE_SINGLE( CDevice )
};

GAME_END