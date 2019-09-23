#include "RasterizerState.h"
#include "../Device.h"

Engine_USING

CRasterizerState::CRasterizerState()
{
	m_eType = RST_RASTERIZER;
}

CRasterizerState::~CRasterizerState()
{
}

bool CRasterizerState::CreateRasterizerState(const string& strKey, D3D11_FILL_MODE eFill,
	D3D11_CULL_MODE eCull, bool bClockWise, int iDepthBias,
	float fDepthBiasClamp, float fSlopeScale, bool bDepthClipEnable,
	bool bScissorEnable, bool bMultisampleEnable,
	bool bAntialisedLineEnable)
{
	m_strKey = strKey;

	D3D11_RASTERIZER_DESC	tDesc = {};

	tDesc.FillMode = eFill;
	tDesc.CullMode = eCull;
	tDesc.FrontCounterClockwise = bClockWise;
	tDesc.DepthBias = iDepthBias;
	tDesc.DepthBiasClamp = fDepthBiasClamp;
	tDesc.SlopeScaledDepthBias = fSlopeScale;
	tDesc.DepthClipEnable = bDepthClipEnable;
	tDesc.ScissorEnable = bScissorEnable;
	tDesc.MultisampleEnable = bMultisampleEnable;
	tDesc.AntialiasedLineEnable = bAntialisedLineEnable;

	if (FAILED(DEVICE->CreateRasterizerState(&tDesc,
		(ID3D11RasterizerState**)&m_pState)))
		return false;

	return true;
}

void CRasterizerState::SetState()
{
	CONTEXT->RSGetState((ID3D11RasterizerState**)&m_pOldState);
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pState);
}

void CRasterizerState::ResetState()
{
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pOldState);
	SAFE_RELEASE(m_pOldState);
}
