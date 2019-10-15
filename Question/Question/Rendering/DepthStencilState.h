#pragma once
#include "RenderState.h"

class CDepthStencilState :
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
	D3D12_DEPTH_STENCIL_DESC	m_tDesc;

public:
	bool CreateDepthStencilState(const string& strKey, bool bDepth = true,
		D3D12_DEPTH_WRITE_MASK eDepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL,
		D3D12_COMPARISON_FUNC eDepthFunc = D3D12_COMPARISON_FUNC_LESS, bool bStencil = false,
		UINT8 iReadMask = 0, UINT8 iWriteMask = 0,
		D3D12_DEPTH_STENCILOP_DESC tFrontDesc = {},
		D3D12_DEPTH_STENCILOP_DESC tBackDesc = {});

public:
	D3D12_DEPTH_STENCIL_DESC	GetDepthStencilDesc()	const;
//public:
//	virtual void SetState();
//	virtual void ResetState();
};