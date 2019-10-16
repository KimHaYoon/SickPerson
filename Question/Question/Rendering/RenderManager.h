#pragma once
#include "../Engine.h"

class CRenderManager
{
private:
	unordered_map<string, class CRenderState*>		m_mapRenderState;
	vector<D3D12_RENDER_TARGET_BLEND_DESC>			m_vecDesc;

public:
	bool Init();

	bool AddBlendTargetDesc(bool bEnable = true, D3D12_BLEND eSrc = D3D12_BLEND_SRC_ALPHA,
		D3D12_BLEND eDest = D3D12_BLEND_INV_SRC_ALPHA,
		D3D12_BLEND_OP eOp = D3D12_BLEND_OP_ADD, D3D12_BLEND eSrcAlpha = D3D12_BLEND_ONE,
		D3D12_BLEND eDestAlpha = D3D12_BLEND_ZERO,
		D3D12_BLEND_OP eAlphaOp = D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP eLogicOP = D3D12_LOGIC_OP_NOOP,
		UINT8 iWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL);

	class CBlendState* CreateBlendState(const string& strKey,
		bool bAlphaCoverage, bool bIndependent);

	class CDepthStencilState* CreateDepthStencilState( const string& strKey, bool bDepth = true,
		D3D12_DEPTH_WRITE_MASK eDepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL,
		D3D12_COMPARISON_FUNC eDepthFunc = D3D12_COMPARISON_FUNC_LESS, bool bStencil = false,
		UINT8 iReadMask = 0, UINT8 iWriteMask = 0,
		D3D12_DEPTH_STENCILOP_DESC tFrontDesc = {},
		D3D12_DEPTH_STENCILOP_DESC tBackDesc = {} );

	class CRasterizerState* CreateRasterizerState(const string& strKey,
		D3D12_FILL_MODE eFill = D3D12_FILL_MODE_SOLID,
		D3D12_CULL_MODE eCull = D3D12_CULL_MODE_BACK,
		bool bClockWise = false, int iDepthBias = 0, float fDepthBiasClamp = 0.f,
		float fSlopeScale = 0.f, bool bDepthClipEnable = true,
		bool bMultisampleEnable = false,
		bool bAntialisedLineEnable = false);

	class CRenderState* FindRenderState(const string& strKey);

	DECLARE_SINGLE(CRenderManager)
};