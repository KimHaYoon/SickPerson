#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState()
{
	m_eType = RST_DEPTH;
}

CDepthStencilState::~CDepthStencilState()
{
}

bool CDepthStencilState::CreateDepthStencilState(const string& strKey, bool bDepth,
	D3D12_DEPTH_WRITE_MASK eDepthWriteMask,
	D3D12_COMPARISON_FUNC eDepthFunc, bool bStencil,
	UINT8 iReadMask, UINT8 iWriteMask,
	D3D12_DEPTH_STENCILOP_DESC tFrontDesc,
	D3D12_DEPTH_STENCILOP_DESC tBackDesc)
{
	m_strKey = strKey;

	m_tDesc.DepthEnable = bDepth;
	m_tDesc.DepthWriteMask = eDepthWriteMask;
	m_tDesc.DepthFunc = eDepthFunc;
	m_tDesc.StencilEnable = bStencil;
	m_tDesc.StencilReadMask = iReadMask;
	m_tDesc.StencilWriteMask = iWriteMask;
	m_tDesc.FrontFace = tFrontDesc;
	m_tDesc.BackFace = tBackDesc;

	m_iStencilRef = 0xff;

	return true;
}

D3D12_DEPTH_STENCIL_DESC CDepthStencilState::GetDepthStencilDesc() const
{
	return m_tDesc;
}

//void CDepthStencilState::SetState()
//{
//	CONTEXT->OMGetDepthStencilState((ID3D12DepthStencilState**)&m_pOldState,
//		&m_iOldStencilRef);
//	CONTEXT->OMSetDepthStencilState((ID3D12DepthStencilState*)m_pState,
//		m_iStencilRef);
//}
//
//void CDepthStencilState::ResetState()
//{
//	CONTEXT->OMSetDepthStencilState((ID3D12DepthStencilState*)m_pOldState,
//		m_iOldStencilRef);
//	SAFE_RELEASE(m_pOldState);
//}
