#pragma once
#include "RenderState.h"

class CRasterizerState :
	public CRenderState
{
private:
	friend class CRenderManager;

private:
	CRasterizerState();
	~CRasterizerState();

private:
	D3D12_RASTERIZER_DESC	m_tDesc;

public:
	bool CreateRasterizerState( const string& strKey, D3D12_FILL_MODE eFill = D3D12_FILL_MODE_SOLID,
		D3D12_CULL_MODE eCull = D3D12_CULL_MODE_BACK,
		bool bClockWise = false, int iDepthBias = 0, float fDepthBiasClamp = 0.f,
		float fSlopeScale = 0.f, bool bDepthClipEnable = true,
		bool bMultisampleEnable = false,
		bool bAntialisedLineEnable = false );

public:
	D3D12_RASTERIZER_DESC	GetRasterizerDesc();

//public:
//	virtual void SetState();
//	virtual void ResetState();
};