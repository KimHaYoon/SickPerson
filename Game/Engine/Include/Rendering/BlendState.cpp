#include "BlendState.h"

Engine_USING

CBlendState::CBlendState()
{
}


CBlendState::~CBlendState()
{
}

bool CBlendState::AddDesc( bool bEnable, D3D12_BLEND eSrc, D3D12_BLEND eDest, D3D12_BLEND_OP eOp, D3D12_BLEND eSrcAlpha, D3D12_BLEND eDestAlpha, D3D12_BLEND_OP eAlphaOp, UINT8 iWriteMask )
{
	return false;
}

void CBlendState::AddDesc( const D3D12_RENDER_TARGET_BLEND_DESC & tDesc )
{
}

bool CBlendState::CreateBlendState( const string & strKey, bool bAlphaCoverage, bool bIndependent )
{
	return false;
}

void CBlendState::SetState()
{
}

void CBlendState::ResetState()
{
}
