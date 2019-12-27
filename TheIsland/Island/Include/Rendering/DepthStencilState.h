#pragma once
#include "RenderState.h"

GAME_BEGIN

class GAME_DLL CDepthStencilState :
	public CRenderState
{
private:
	friend class CRenderManager;

private:
	CDepthStencilState();
	~CDepthStencilState();

private:
	UINT		m_iStencilRef;
	UINT		m_iOldStencilRef;

public:
	bool CreateDepthStencilState(const string& strKey, bool bDepth = true,
		D3D11_DEPTH_WRITE_MASK eDepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS, bool bStencil = false,
		UINT8 iReadMask = 0, UINT8 iWriteMask = 0,
		D3D11_DEPTH_STENCILOP_DESC tFrontDesc = {},
		D3D11_DEPTH_STENCILOP_DESC tBackDesc = {});

public:
	virtual void SetState();
	virtual void ResetState();
};

GAME_END
