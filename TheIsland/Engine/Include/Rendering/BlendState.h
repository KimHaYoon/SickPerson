#pragma once
#include "RenderState.h"

GAME_BEGIN

class GAME_DLL CBlendState :
	public CRenderState
{
private:
	friend class CRenderManager;

private:
	CBlendState();
	~CBlendState();

private:
	vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecDesc;
	float	m_fBlendFactor[4];
	UINT	m_iSampleMask;
	float	m_fOldBlendFactor[4];
	UINT	m_iOldSampleMask;

public:
	bool AddDesc(bool bEnable = true, D3D11_BLEND eSrc = D3D11_BLEND_SRC_ALPHA, 
		D3D11_BLEND eDest = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eOp = D3D11_BLEND_OP_ADD, D3D11_BLEND eSrcAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND eDestAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eAlphaOp = D3D11_BLEND_OP_ADD, 
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	void AddDesc(const D3D11_RENDER_TARGET_BLEND_DESC& tDesc);
	bool CreateBlendState(const string& strKey, bool bAlphaCoverage, bool bIndependent);

public:
	virtual void SetState();
	virtual void ResetState();
};

GAME_END
