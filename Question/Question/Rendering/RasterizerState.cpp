#include "RasterizerState.h"
#include "../Device.h"

CRasterizerState::CRasterizerState()
{
	m_eType = RST_RASTERIZER;
}

CRasterizerState::~CRasterizerState()
{
}

bool CRasterizerState::CreateRasterizerState(const string& strKey, D3D12_FILL_MODE eFill,
	D3D12_CULL_MODE eCull, bool bClockWise, int iDepthBias,
	float fDepthBiasClamp, float fSlopeScale, bool bDepthClipEnable,
	bool bMultisampleEnable,
	bool bAntialisedLineEnable)
{
	m_strKey = strKey;

	m_tDesc.FillMode = eFill;
	m_tDesc.CullMode = eCull;
	m_tDesc.FrontCounterClockwise = bClockWise;
	m_tDesc.DepthBias = iDepthBias;
	m_tDesc.DepthBiasClamp = fDepthBiasClamp;
	m_tDesc.SlopeScaledDepthBias = fSlopeScale;
	m_tDesc.DepthClipEnable = bDepthClipEnable;
	m_tDesc.MultisampleEnable = bMultisampleEnable;
	m_tDesc.AntialiasedLineEnable = bAntialisedLineEnable;
	m_tDesc.ForcedSampleCount = 0;
	m_tDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	
	return true;
}

D3D12_RASTERIZER_DESC CRasterizerState::GetDesc()
{
	return m_tDesc;
}

//void CRasterizerState::SetState()
//{
//	CONTEXT->RSGetState((ID3D12RasterizerState**)&m_pOldState);
//	CONTEXT->RSSetState((ID3D12RasterizerState*)m_pState);
//}
//
//void CRasterizerState::ResetState()
//{
//	CONTEXT->RSSetState((ID3D12RasterizerState*)m_pOldState);
//	SAFE_RELEASE(m_pOldState);
//}
