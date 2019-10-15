#pragma once
#include "../Engine.h"

Engine_BEGIN

typedef struct _tagRenderGroup
{
	class CGameObject**	pRenderObj;
	int		iSize;
	int		iCapasity;

	_tagRenderGroup() :
		iSize(0),
		iCapasity(1000)
	{
		pRenderObj = new CGameObject*[iCapasity];
	}
}RENDERGROUP, *PRENDERGROUP;

enum RENDER_GROUP_TYPE
{
	RGT_LANDSCAPE,
	RGT_DEFAULT,
	RGT_DECAL,
	RGT_ALPHA,
	RGT_UI,
	RGT_END
};

typedef struct _tagMultiRenderTarget
{
	vector<class CRenderingTarget*>	vecTargets;
	ID3D12DescriptorHeap*			pDepth;
	ID3D12DescriptorHeap*		pOldTarget[8];
	ID3D12DescriptorHeap*			pOldDepth;

	_tagMultiRenderTarget() :
		pDepth(NULL),
		pOldDepth(NULL)
	{
		memset(pOldTarget, 0, sizeof( ID3D12DescriptorHeap*) * 8);
	}
}MRT, *PMRT;

class Engine_DLL CRenderManager
{
private:
	unordered_map<string, class CRenderState*>		m_mapRenderState;
	vector<D3D12_RENDER_TARGET_BLEND_DESC>			m_vecDesc;
	RENDERGROUP	m_tRenderGroup[RGT_END];
	unordered_map<string, class CRenderingTarget*>	m_mapTarget;
	unordered_map<string, PMRT>	m_mapMRT;
	class CShader*	m_pLightAccShader;
	class CShader*	m_pLightBlendShader;
	class CShader*	m_pDeferredShader;
	class CSampler*	m_pPointSmp;
	class CRenderState*	m_pDepthDisable;
	class CRenderState*	m_pAccBlend;

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

	class CRenderingTarget* CreateRenderTarget(const string& strKey,
		unsigned int iW, unsigned int iH, DXGI_FORMAT eFmt,
		const Vector4& vClearColor = Vector4::Zero,
		DXGI_FORMAT eDepthFmt = DXGI_FORMAT_UNKNOWN);

	class CRenderingTarget* FindRenderTarget(const string& strKey);
	void AddRenderTargetToMRT(const string& strMRTKey,
		const string& strTargetKey);
	void SetDepthToMRT(const string& strMRTKey,
		const string& strDepthKey);
	void ClearMRT(const string& strKey);
	void SetMRT(const string& strKey);
	void ResetMRT(const string& strKey);

	PMRT FindMRT(const string& strKey);

public:
	void AddRenderObject(class CGameObject* pObj);
	void Render(float fTime);

private:
	void RenderDecal(float fTime);
	void RenderGBuffer(float fTime);
	void RenderLightAcc(float fTime);
	void RenderLightBlend(float fTime);
	void RenderDeferredTarget(float fTime);

public:
	static bool SortZ(class CGameObject* pSrc, class CGameObject* pDest);
	static bool SortAlpha(class CGameObject* pSrc, class CGameObject* pDest);
	static bool SortUI(class CGameObject* pSrc, class CGameObject* pDest);

	DECLARE_SINGLE(CRenderManager)
};

Engine_END