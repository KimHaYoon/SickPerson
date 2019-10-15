#include "BlendState.h"
#include "../Device.h"

Engine_USING

CBlendState::CBlendState()
{
	m_eType = RST_BLEND;
}

CBlendState::~CBlendState()
{
}

bool CBlendState::AddDesc( bool bEnable, D3D12_BLEND eSrc, D3D12_BLEND eDest,
	D3D12_BLEND_OP eOp, D3D12_BLEND eSrcAlpha, D3D12_BLEND eDestAlpha,
	D3D12_BLEND_OP eAlphaOp, UINT8 iWriteMask )
{
	if ( m_vecDesc.size() == 8 )
		return false;

	D3D12_RENDER_TARGET_BLEND_DESC	tDesc = {};

	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = eSrc;
	tDesc.DestBlend = eDest;
	tDesc.BlendOp = eOp;
	tDesc.SrcBlendAlpha = eSrcAlpha;
	tDesc.DestBlendAlpha = eDestAlpha;
	tDesc.BlendOpAlpha = eAlphaOp;
	tDesc.RenderTargetWriteMask = iWriteMask;

	m_vecDesc.push_back( tDesc );

	return true;
}

void CBlendState::AddDesc( const D3D12_RENDER_TARGET_BLEND_DESC & tDesc )
{
	if ( m_vecDesc.size() == 8 )
		return;

	m_vecDesc.push_back( tDesc );
}

bool CBlendState::CreateBlendState( const string& strKey, bool bAlphaCoverage, bool bIndependent )
{
	m_strKey = strKey;
	
	m_tDesc.AlphaToCoverageEnable = bAlphaCoverage;
	m_tDesc.IndependentBlendEnable = bIndependent;

	for ( size_t i = 0; i < m_vecDesc.size(); ++i )
	{
		m_tDesc.RenderTarget[i] = m_vecDesc[i];
	}

	memset( m_fBlendFactor, 0, sizeof( float ) * 4 );
	m_iSampleMask = 0xffffffff;

	return true;
}

D3D12_BLEND_DESC CBlendState::GetBlendDesc() const
{
	return m_tDesc;
}

//void CBlendState::SetState()
//{
//	CONTEXT->OMGetBlendState( ( ID3D12BlendState** )&m_pOldState, m_fOldBlendFactor,
//		&m_iOldSampleMask );
//	CONTEXT->OMSetBlendState( ( ID3D12BlendState* )m_pState, m_fBlendFactor, m_iSampleMask );
//}
//
//void CBlendState::ResetState()
//{
//	CONTEXT->OMSetBlendState( ( ID3D12BlendState* )m_pOldState, m_fOldBlendFactor, m_iOldSampleMask );
//	SAFE_RELEASE( m_pOldState );
//}
