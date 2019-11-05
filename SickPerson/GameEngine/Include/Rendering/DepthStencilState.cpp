#include "DepthStencilState.h"
#include "../Device.h"

Engine_USING

CDepthStencilState::CDepthStencilState()
{
	m_eType = RST_DEPTH;
}

CDepthStencilState::~CDepthStencilState()
{
}

bool CDepthStencilState::CreateDepthStencilState(const string& strKey, bool bDepth,
	D3D11_DEPTH_WRITE_MASK eDepthWriteMask,
	D3D11_COMPARISON_FUNC eDepthFunc, bool bStencil,
	UINT8 iReadMask, UINT8 iWriteMask,
	D3D11_DEPTH_STENCILOP_DESC tFrontDesc,
	D3D11_DEPTH_STENCILOP_DESC tBackDesc)
{
	m_strKey = strKey;

	D3D11_DEPTH_STENCIL_DESC	tDesc = {};

	tDesc.DepthEnable = bDepth;
	tDesc.DepthWriteMask = eDepthWriteMask;
	tDesc.DepthFunc = eDepthFunc;
	tDesc.StencilEnable = bStencil;
	tDesc.StencilReadMask = iReadMask;
	tDesc.StencilWriteMask = iWriteMask;
	tDesc.FrontFace = tFrontDesc;
	tDesc.BackFace = tBackDesc;

	if (FAILED(DEVICE->CreateDepthStencilState(&tDesc,
		(ID3D11DepthStencilState**)&m_pState)))
		return false;

	m_iStencilRef = 0xff;

	return true;
}

void CDepthStencilState::SetState()
{
	CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_pOldState,
		&m_iOldStencilRef);
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pState,
		m_iStencilRef);
}

void CDepthStencilState::ResetState()
{
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pOldState,
		m_iOldStencilRef);
	SAFE_RELEASE(m_pOldState);
}
