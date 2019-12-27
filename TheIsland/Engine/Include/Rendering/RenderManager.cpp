#include "RenderManager.h"
#include "ShaderManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

GAME_USING

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
{
}

CRenderManager::~CRenderManager()
{
	Safe_Release_Map(m_mapRenderState);
	DESTROY_SINGLE(CShaderManager);
}

bool CRenderManager::Init()
{
	if (!GET_SINGLE(CShaderManager)->Init())
		return false;

	AddBlendTargetDesc();

	CBlendState*	pState = CreateBlendState(ALPHA_BLEND, FALSE, FALSE);

	SAFE_RELEASE(pState);

	CDepthStencilState*	pDepthDisable = CreateDepthStencilState( DEPTH_DISABLE, false );

	SAFE_RELEASE(pDepthDisable);

	pDepthDisable = CreateDepthStencilState( DEPTH_LESS_EQUAL, true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL );

	SAFE_RELEASE( pDepthDisable );

	CRasterizerState*	pRS = CreateRasterizerState( WIRE_FRAME, D3D11_FILL_WIREFRAME );

	SAFE_RELEASE(pRS);

	pRS = CreateRasterizerState( CULLING_NONE, D3D11_FILL_SOLID, D3D11_CULL_NONE );

	SAFE_RELEASE(pRS);

	return true;
}

bool CRenderManager::AddBlendTargetDesc(bool bEnable, D3D11_BLEND eSrc, 
	D3D11_BLEND eDest, D3D11_BLEND_OP eOp, D3D11_BLEND eSrcAlpha, 
	D3D11_BLEND eDestAlpha, D3D11_BLEND_OP eAlphaOp, UINT8 iWriteMask)
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

CBlendState * CRenderManager::CreateBlendState(const string & strKey, 
	bool bAlphaCoverage, bool bIndependent)
{
	CBlendState*	pState = (CBlendState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CBlendState;

	for (size_t i = 0; i < m_vecDesc.size(); ++i)
	{
		pState->AddDesc(m_vecDesc[i]);
	}

	m_vecDesc.clear();

	if (!pState->CreateBlendState(strKey, bAlphaCoverage, bIndependent))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->SetTag(strKey);

	pState->AddRef();

	m_mapRenderState.insert(make_pair(strKey, pState));
	
	return pState;
}

CDepthStencilState * CRenderManager::CreateDepthStencilState(
	const string & strKey, bool bDepth,
	D3D11_DEPTH_WRITE_MASK eDepthWriteMask,
	D3D11_COMPARISON_FUNC eDepthFunc, bool bStencil,
	UINT8 iReadMask, UINT8 iWriteMask,
	D3D11_DEPTH_STENCILOP_DESC tFrontDesc,
	D3D11_DEPTH_STENCILOP_DESC tBackDesc)
{
	CDepthStencilState*	pState = (CDepthStencilState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDepthStencilState;

	if (!pState->CreateDepthStencilState(strKey, bDepth, eDepthWriteMask,
		eDepthFunc, bStencil, iReadMask, iWriteMask,
		tFrontDesc, tBackDesc))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->SetTag(strKey);

	pState->AddRef();

	m_mapRenderState.insert(make_pair(strKey, pState));

	return pState;
}

CRasterizerState * CRenderManager::CreateRasterizerState(
	const string & strKey, D3D11_FILL_MODE eFill,
	D3D11_CULL_MODE eCull, bool bClockWise, int iDepthBias, 
	float fDepthBiasClamp, float fSlopeScale, bool bDepthClipEnable,
	bool bScissorEnable, bool bMultisampleEnable, 
	bool bAntialisedLineEnable)
{
	CRasterizerState*	pState = (CRasterizerState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CRasterizerState;

	if (!pState->CreateRasterizerState(strKey, eFill, eCull,
		bClockWise, iDepthBias, fDepthBiasClamp, fSlopeScale,
		bDepthClipEnable, bScissorEnable, bMultisampleEnable,
		bAntialisedLineEnable))
	{
		SAFE_RELEASE(pState);
		return NULL;
	}

	pState->SetTag(strKey);

	pState->AddRef();

	m_mapRenderState.insert(make_pair(strKey, pState));

	return pState;
}

CRenderState * CRenderManager::FindRenderState(const string & strKey)
{
	unordered_map<string, class CRenderState*>::iterator	iter = m_mapRenderState.find(strKey);

	if (iter == m_mapRenderState.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}
