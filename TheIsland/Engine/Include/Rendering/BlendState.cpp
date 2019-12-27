#include "BlendState.h"
#include "../Device.h"

GAME_USING

CBlendState::CBlendState()
{
	m_eType = RST_BLEND;
}

CBlendState::~CBlendState()
{
}

bool CBlendState::AddDesc(bool bEnable, D3D11_BLEND eSrc, D3D11_BLEND eDest, 
	D3D11_BLEND_OP eOp, D3D11_BLEND eSrcAlpha, D3D11_BLEND eDestAlpha, 
	D3D11_BLEND_OP eAlphaOp, UINT8 iWriteMask)
{
	if (m_vecDesc.size() == 8)
		return false;

	D3D11_RENDER_TARGET_BLEND_DESC	tDesc = {};

	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = eSrc;
	tDesc.DestBlend = eDest;
	tDesc.BlendOp = eOp;
	tDesc.SrcBlendAlpha = eSrcAlpha;
	tDesc.DestBlendAlpha = eDestAlpha;
	tDesc.BlendOpAlpha = eAlphaOp;
	tDesc.RenderTargetWriteMask = iWriteMask;

	m_vecDesc.push_back(tDesc);

	return true;
}

void CBlendState::AddDesc(const D3D11_RENDER_TARGET_BLEND_DESC & tDesc)
{
	if (m_vecDesc.size() == 8)
		return;

	m_vecDesc.push_back(tDesc);
}

bool CBlendState::CreateBlendState(const string& strKey, bool bAlphaCoverage, bool bIndependent)
{
	m_strKey = strKey;

	D3D11_BLEND_DESC	tDesc = {};

	tDesc.AlphaToCoverageEnable = bAlphaCoverage;
	tDesc.IndependentBlendEnable = bIndependent;

	for (size_t i = 0; i < m_vecDesc.size(); ++i)
	{
		tDesc.RenderTarget[i] = m_vecDesc[i];
	}

	if (FAILED(DEVICE->CreateBlendState(&tDesc, (ID3D11BlendState**)&m_pState)))
		return false;

	memset(m_fBlendFactor, 0, sizeof(float) * 4);
	m_iSampleMask = 0xffffffff;

	return true;
}

void CBlendState::SetState()
{
	CONTEXT->OMGetBlendState((ID3D11BlendState**)&m_pOldState, m_fOldBlendFactor,
		&m_iOldSampleMask);
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState, m_fBlendFactor, m_iSampleMask);
}

void CBlendState::ResetState()
{
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pOldState, m_fOldBlendFactor, m_iOldSampleMask);
	SAFE_RELEASE(m_pOldState);
}
