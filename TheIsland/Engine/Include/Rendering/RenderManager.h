#pragma once

#include "../Engine.h"

GAME_BEGIN

class GAME_DLL CRenderManager
{
private:
	unordered_map<string, class CRenderState*>	m_mapRenderState;
	vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecDesc;

public:
	bool Init();
	bool AddBlendTargetDesc(bool bEnable = true, D3D11_BLEND eSrc = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDest = D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP eOp = D3D11_BLEND_OP_ADD, D3D11_BLEND eSrcAlpha = D3D11_BLEND_ONE,
		D3D11_BLEND eDestAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP eAlphaOp = D3D11_BLEND_OP_ADD,
		UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	class CBlendState* CreateBlendState(const string& strKey, 
		bool bAlphaCoverage, bool bIndependent);

	class CDepthStencilState* CreateDepthStencilState(const string& strKey,
		bool bDepth = true,
		D3D11_DEPTH_WRITE_MASK eDepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS, bool bStencil = false,
		UINT8 iReadMask = 0, UINT8 iWriteMask = 0,
		D3D11_DEPTH_STENCILOP_DESC tFrontDesc = {},
		D3D11_DEPTH_STENCILOP_DESC tBackDesc = {});

	class CRasterizerState* CreateRasterizerState(const string& strKey,
		D3D11_FILL_MODE eFill = D3D11_FILL_SOLID,
		D3D11_CULL_MODE eCull = D3D11_CULL_BACK,
		bool bClockWise = false, int iDepthBias = 0, float fDepthBiasClamp = 0.f,
		float fSlopeScale = 0.f, bool bDepthClipEnable = true,
		bool bScissorEnable = false, bool bMultisampleEnable = false,
		bool bAntialisedLineEnable = false);

	class CRenderState* FindRenderState(const string& strKey);

	DECLARE_SINGLE(CRenderManager)
};

GAME_END
