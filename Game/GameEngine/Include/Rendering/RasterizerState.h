#pragma once
#include "RenderState.h"

Engine_BEGIN

class Engine_DLL CRasterizerState :
	public CRenderState
{
private:
	friend class CRenderManager;

private:
	CRasterizerState();
	~CRasterizerState();

public:
	bool CreateRasterizerState(const string& strKey, D3D11_FILL_MODE eFill = D3D11_FILL_SOLID,
		D3D11_CULL_MODE eCull = D3D11_CULL_BACK,
		bool bClockWise = false, int iDepthBias = 0, float fDepthBiasClamp = 0.f,
		float fSlopeScale = 0.f, bool bDepthClipEnable = true,
		bool bScissorEnable = false, bool bMultisampleEnable = false,
		bool bAntialisedLineEnable = false);

public:
	virtual void SetState();
	virtual void ResetState();
};

Engine_END