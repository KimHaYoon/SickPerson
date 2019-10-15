#include "RenderManager.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "RenderingTarget.h"
#include "../Device.h"
#include "../Core/Input.h"
#include "../Component/Renderer.h"
#include "../Component/Renderer2D.h"
#include "../Component/UI.h"
#include "../Component/Light.h"
#include "../Component/Camera.h"
#include "../Component/UI.h"
#include "../Component/Transform.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObject.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Sampler.h"

Engine_USING

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() :
	m_pLightAccShader(NULL),
	m_pLightBlendShader(NULL),
	m_pDeferredShader(NULL),
	m_pPointSmp(NULL),
	m_pDepthDisable(NULL),
	m_pAccBlend(NULL)
{
}

CRenderManager::~CRenderManager()
{
	SAFE_RELEASE(m_pDeferredShader);
	SAFE_RELEASE(m_pLightBlendShader);
	SAFE_RELEASE(m_pDepthDisable);
	SAFE_RELEASE(m_pAccBlend);
	SAFE_RELEASE(m_pPointSmp);
	SAFE_RELEASE(m_pLightAccShader);
	Safe_Delete_Map(m_mapMRT);
	Safe_Release_Map(m_mapTarget);

	for (int i = 0; i < RGT_END; ++i)
	{
		SAFE_DELETE_ARRAY(m_tRenderGroup[i].pRenderObj);
	}

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

	AddBlendTargetDesc(true, D3D12_BLEND_ONE, D3D12_BLEND_ONE);
	pState = CreateBlendState(ONE_BLEND, FALSE, FALSE);

	SAFE_RELEASE(pState);

	CDepthStencilState*	pDepthDisable = CreateDepthStencilState(
		DEPTH_DISABLE, false);

	SAFE_RELEASE(pDepthDisable);

	pDepthDisable = CreateDepthStencilState(
		DEPTH_LESS_EQUAL, true, D3D12_DEPTH_WRITE_MASK_ALL,
		D3D12_COMPARISON_FUNC_LESS_EQUAL);

	SAFE_RELEASE(pDepthDisable);

	CRasterizerState*	pRS = CreateRasterizerState(WIRE_FRAME,
		D3D12_FILL_MODE_WIREFRAME);

	SAFE_RELEASE(pRS);

	pRS = CreateRasterizerState(CULLING_NONE, D3D12_FILL_MODE_SOLID,
		D3D12_CULL_MODE_NONE);

	SAFE_RELEASE(pRS);

	// 렌더링 타겟 생성
	CRenderingTarget*	pTarget = CreateRenderTarget("Albedo",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3::Zero, Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Normal",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 100.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Depth",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT, Vector4(1.f, 1.f, 1.f, 0.f));

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 200.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Specular",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 300.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Tangent",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 400.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("Binormal",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(0.f, 500.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	AddRenderTargetToMRT("GBuffer", "Albedo");
	AddRenderTargetToMRT("GBuffer", "Normal");
	AddRenderTargetToMRT("GBuffer", "Depth");
	AddRenderTargetToMRT("GBuffer", "Specular");
	AddRenderTargetToMRT("GBuffer", "Tangent");
	AddRenderTargetToMRT("GBuffer", "Binormal");
	//SetDepthToMRT("GBuffer", "Albedo");

	AddRenderTargetToMRT("Decal", "Albedo");
	AddRenderTargetToMRT("Decal", "Normal");
	AddRenderTargetToMRT("Decal", "Specular");

	pTarget = CreateRenderTarget("LightDif",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		Vector4::Zero, DXGI_FORMAT_D24_UNORM_S8_UINT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(100.f, 0.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	pTarget = CreateRenderTarget("LightSpc",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(100.f, 100.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	AddRenderTargetToMRT("LightAcc", "LightDif");
	AddRenderTargetToMRT("LightAcc", "LightSpc");
	SetDepthToMRT("LightAcc", "LightDif");

	pTarget = CreateRenderTarget("LightBlend",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		Vector4::Zero, DXGI_FORMAT_D24_UNORM_S8_UINT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(200.f, 0.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	// 조명 누적 버퍼용 셰이더
	m_pLightAccShader = GET_SINGLE(CShaderManager)->FindShader(LIGHT_ACC_SHADER);
	m_pPointSmp = GET_SINGLE(CResourcesManager)->FindSampler(SAMPLER_POINT);
	m_pDepthDisable = FindRenderState(DEPTH_DISABLE);
	m_pAccBlend = FindRenderState(ONE_BLEND);

	m_pLightBlendShader = GET_SINGLE(CShaderManager)->FindShader(LIGHT_BLEND_SHADER);
	m_pDeferredShader = GET_SINGLE(CShaderManager)->FindShader(DEFERRED_SHADER);

	return true;
}

bool CRenderManager::AddBlendTargetDesc(bool bEnable, D3D12_BLEND eSrc,
	D3D12_BLEND eDest, D3D12_BLEND_OP eOp, D3D12_BLEND eSrcAlpha,
	D3D12_BLEND eDestAlpha, D3D12_BLEND_OP eAlphaOp, D3D12_LOGIC_OP eLogicOP, UINT8 iWriteMask)
{
	if (m_vecDesc.size() == 8)
		return false;

	D3D12_RENDER_TARGET_BLEND_DESC	tDesc = {};

	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = eSrc;
	tDesc.DestBlend = eDest;
	tDesc.BlendOp = eOp;
	tDesc.SrcBlendAlpha = eSrcAlpha;
	tDesc.DestBlendAlpha = eDestAlpha;
	tDesc.BlendOpAlpha = eAlphaOp;
	tDesc.LogicOp = eLogicOP;
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
	D3D12_DEPTH_WRITE_MASK eDepthWriteMask,
	D3D12_COMPARISON_FUNC eDepthFunc, bool bStencil,
	UINT8 iReadMask, UINT8 iWriteMask,
	D3D12_DEPTH_STENCILOP_DESC tFrontDesc,
	D3D12_DEPTH_STENCILOP_DESC tBackDesc)
{
	CDepthStencilState*	pState = (CDepthStencilState*)FindRenderState(strKey);

	if (pState)
		return pState;

	pState = new CDepthStencilState;

	if ( !pState->CreateDepthStencilState( strKey, bDepth, eDepthWriteMask,
		eDepthFunc, bStencil, iReadMask, iWriteMask,
		tFrontDesc, tBackDesc ) )
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
	const string & strKey, D3D12_FILL_MODE eFill,
	D3D12_CULL_MODE eCull, bool bClockWise, int iDepthBias,
	float fDepthBiasClamp, float fSlopeScale, bool bDepthClipEnable,
	bool bMultisampleEnable,
	bool bAntialisedLineEnable)
{
	CRasterizerState*	pState = (CRasterizerState*)FindRenderState(strKey);

	if (pState)
		return pState;

	if (!pState->CreateRasterizerState(strKey, eFill, eCull,
		bClockWise, iDepthBias, fDepthBiasClamp, fSlopeScale,
		bDepthClipEnable, bMultisampleEnable,
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

CRenderingTarget * CRenderManager::CreateRenderTarget(const string & strKey,
	unsigned int iW, unsigned int iH, DXGI_FORMAT eFmt,
	const Vector4& vClearColor, DXGI_FORMAT eDepthFmt)
{
	CRenderingTarget*	pTarget = FindRenderTarget(strKey);

	if (pTarget)
		return pTarget;

	pTarget = new CRenderingTarget;

	if (!pTarget->CreateTarget(iW, iH, eFmt, vClearColor, eDepthFmt))
	{
		SAFE_RELEASE(pTarget);
		return NULL;
	}

	pTarget->AddRef();
	m_mapTarget.insert(make_pair(strKey, pTarget));

	return pTarget;
}

CRenderingTarget * CRenderManager::FindRenderTarget(const string & strKey)
{
	unordered_map<string, CRenderingTarget*>::iterator	iter = m_mapTarget.find(strKey);

	if (iter == m_mapTarget.end())
		return NULL;

	iter->second->AddRef();

	return iter->second;
}

void CRenderManager::AddRenderTargetToMRT(const string & strMRTKey,
	const string & strTargetKey)
{
	PMRT	pMRT = FindMRT(strMRTKey);

	CRenderingTarget*	pTarget = FindRenderTarget(strTargetKey);

	if (!pTarget)
		return;

	if (!pMRT)
	{
		pMRT = new MRT;

		m_mapMRT.insert(make_pair(strMRTKey, pMRT));
	}

	if (pMRT->vecTargets.size() == 8)
		return;

	pMRT->vecTargets.push_back(pTarget);

	SAFE_RELEASE(pTarget);
}

void CRenderManager::SetDepthToMRT(const string & strMRTKey,
	const string & strDepthKey)
{
	PMRT	pMRT = FindMRT(strMRTKey);

	CRenderingTarget*	pTarget = FindRenderTarget(strDepthKey);

	if (!pTarget)
		return;

	else if (!pTarget->GetDepthStencilView())
	{
		SAFE_RELEASE(pTarget);
		return;
	}

	if (!pMRT)
	{
		pMRT = new MRT;

		m_mapMRT.insert(make_pair(strMRTKey, pMRT));
	}

	pMRT->pDepth = pTarget->GetDepthStencilView();

	SAFE_RELEASE(pTarget);
}

void CRenderManager::ClearMRT(const string & strKey)
{
	PMRT	pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	for (size_t i = 0; i < pMRT->vecTargets.size(); ++i)
	{
		pMRT->vecTargets[i]->ClearTarget();
	}
}

void CRenderManager::SetMRT(const string & strKey)
{
	PMRT	pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	vector<ID3D12DescriptorHeap*>	vecTargets;
	for (size_t i = 0; i < pMRT->vecTargets.size(); ++i)
	{
		vecTargets.push_back(pMRT->vecTargets[i]->GetRenderTargetView());
	}

	CMLIST->OMGetRenderTargets(vecTargets.size(), pMRT->pOldTarget, &pMRT->pOldDepth);

	ID3D12DescriptorHeap*	pDepth = pMRT->pOldDepth;

	if (pMRT->pDepth)
		pDepth = pMRT->pDepth;

	CMLIST->OMSetRenderTargets(vecTargets.size(), &vecTargets[0], pDepth);
}

void CRenderManager::ResetMRT(const string & strKey)
{
	PMRT	pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	CMLIST->OMSetRenderTargets(pMRT->vecTargets.size(), pMRT->pOldTarget, pMRT->pOldDepth);
	SAFE_RELEASE(pMRT->pOldDepth);

	for (size_t i = 0; i < pMRT->vecTargets.size(); ++i)
	{
		SAFE_RELEASE(pMRT->pOldTarget[i]);
	}
}

PMRT CRenderManager::FindMRT(const string & strKey)
{
	unordered_map<string, PMRT>::iterator	iter = m_mapMRT.find(strKey);

	if (iter == m_mapMRT.end())
		return NULL;

	return iter->second;
}

void CRenderManager::AddRenderObject(CGameObject * pObj)
{
	if (pObj->GetCulling())
		return;

	// UI인지 아닌지 구분
	if (pObj->CheckComponentFromType(CT_UI))
	{
		if (m_tRenderGroup[RGT_UI].iSize == m_tRenderGroup[RGT_UI].iCapasity)
		{
			m_tRenderGroup[RGT_UI].iCapasity *= 1.5f;
			CGameObject**	pList = new CGameObject*[m_tRenderGroup[RGT_UI].iCapasity];
			memcpy(pList, m_tRenderGroup[RGT_UI].pRenderObj,
				sizeof(CGameObject*) * m_tRenderGroup[RGT_UI].iSize);

			delete[]	m_tRenderGroup[RGT_UI].pRenderObj;
			m_tRenderGroup[RGT_UI].pRenderObj = pList;
		}

		m_tRenderGroup[RGT_UI].pRenderObj[m_tRenderGroup[RGT_UI].iSize] =
			pObj;
		++m_tRenderGroup[RGT_UI].iSize;
	}

	else
	{
		RENDER_GROUP_TYPE	rgt;

		CRenderer*	pRenderer = pObj->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer)
		{
			if (pRenderer->GetAlphaEnable())
				rgt = RGT_ALPHA;

			else if (pObj->CheckComponentFromType(CT_LANDSCAPE))
				rgt = RGT_LANDSCAPE;

			else
				rgt = RGT_DEFAULT;

			if (m_tRenderGroup[rgt].iSize == m_tRenderGroup[rgt].iCapasity)
			{
				m_tRenderGroup[rgt].iCapasity *= 1.5f;
				CGameObject**	pList = new CGameObject*[m_tRenderGroup[rgt].iCapasity];
				memcpy(pList, m_tRenderGroup[rgt].pRenderObj,
					sizeof(CGameObject*) * m_tRenderGroup[rgt].iSize);

				delete[]	m_tRenderGroup[rgt].pRenderObj;
				m_tRenderGroup[rgt].pRenderObj = pList;
			}

			m_tRenderGroup[rgt].pRenderObj[m_tRenderGroup[rgt].iSize] =
				pObj;
			++m_tRenderGroup[rgt].iSize;

			SAFE_RELEASE(pRenderer);
		}
	}
}

void CRenderManager::Render(float fTime)
{
	// 출력할 오브젝트 정렬
	for (int i = RGT_LANDSCAPE; i <= RGT_DEFAULT; ++i)
	{
		if (m_tRenderGroup[i].iSize >= 2)
		{
			sort(&m_tRenderGroup[i].pRenderObj[0],
				&m_tRenderGroup[i].pRenderObj[m_tRenderGroup[i].iSize],
				CRenderManager::SortZ);
		}
	}

	if (m_tRenderGroup[RGT_ALPHA].iSize >= 2)
	{
		sort(&m_tRenderGroup[RGT_ALPHA].pRenderObj[0],
			&m_tRenderGroup[RGT_ALPHA].pRenderObj[m_tRenderGroup[RGT_ALPHA].iSize],
			CRenderManager::SortAlpha);
	}

	if (m_tRenderGroup[RGT_UI].iSize >= 2)
	{
		sort(&m_tRenderGroup[RGT_UI].pRenderObj[0],
			&m_tRenderGroup[RGT_UI].pRenderObj[m_tRenderGroup[RGT_UI].iSize],
			CRenderManager::SortUI);
	}

	// GBuffer를 만든다.
	RenderGBuffer(fTime);

	// 조명 누적버퍼를 만든다.
	RenderLightAcc(fTime);

	// 조명 누적버퍼를 이용하여 Albedo에 조명을 처리한다.
	RenderLightBlend(fTime);

	// 최종 조명처리된 타겟을 화면에 출력한다.
	RenderDeferredTarget(fTime);

	//CRenderingTarget*	pTarget = FindRenderTarget("Albedo");

	//pTarget->SetTarget();
	m_pPointSmp->SetSampler(11, SCT_PIXEL);

	CRenderingTarget*	pDepthTarget = FindRenderTarget("Depth");

	pDepthTarget->SetShader(13);

	for (int i = RGT_ALPHA; i < RGT_END; ++i)
	{
		for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
		{
			m_tRenderGroup[i].pRenderObj[j]->Render(fTime);
		}

		m_tRenderGroup[i].iSize = 0;
	}

	pDepthTarget->ResetShader(13);

	SAFE_RELEASE(pDepthTarget);

	//pTarget->ResetTarget();

	//if (GetAsyncKeyState(VK_F9) & 0x8000)
	//{
	//	PMRT	pMRT = FindMRT("GBuffer");

	//	const wchar_t*	pFile[4] = { L"Albedo.png", L"Normal.png", L"Depth.png", L"Specular.png" };
	//	for (size_t i = 0; i < pMRT->vecTargets.size(); ++i)
	//	{
	//		pMRT->vecTargets[i]->SaveFile(pFile[i]);
	//	}
	//}

	// 디버깅용 타겟을 출력한다.
	unordered_map<string, CRenderingTarget*>::iterator	iter;
	unordered_map<string, CRenderingTarget*>::iterator	iterEnd = m_mapTarget.end();

	for (iter = m_mapTarget.begin(); iter != iterEnd; ++iter)
	{
		if (!iter->second->GetDebugEnable())
			continue;

		iter->second->DebugRender(fTime);
	}

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	CGameObject*	pMouseObj = GET_SINGLE(CInput)->GetMouseObj();

	pMouseObj->SetScene(pScene);

	pMouseObj->Render(fTime);

	SAFE_RELEASE(pMouseObj);

	SAFE_RELEASE(pScene);
}

void CRenderManager::RenderDecal(float fTime)
{
	SetMRT("Decal");

	CRenderingTarget*	pDepthTarget = FindRenderTarget("Depth");
	CRenderingTarget*	pTangentTarget = FindRenderTarget("Tangent");
	CRenderingTarget*	pBinormalTarget = FindRenderTarget("Binormal");

	m_pPointSmp->SetSampler(11, SCT_PIXEL);

	pDepthTarget->SetShader(13);
	pTangentTarget->SetShader(15);
	pBinormalTarget->SetShader(16);

	for (int i = 0; i < m_tRenderGroup[RGT_DECAL].iSize; ++i)
	{
		m_tRenderGroup[RGT_DECAL].pRenderObj[i]->Render(fTime);
	}

	m_tRenderGroup[RGT_DECAL].iSize = 0;

	pDepthTarget->ResetShader(13);
	pTangentTarget->ResetShader(15);
	pBinormalTarget->ResetShader(16);

	SAFE_RELEASE(pDepthTarget);
	SAFE_RELEASE(pTangentTarget);
	SAFE_RELEASE(pBinormalTarget);

	ResetMRT("Decal");
}

void CRenderManager::RenderGBuffer(float fTime)
{
	ClearMRT("GBuffer");
	SetMRT("GBuffer");

	for (int i = RGT_LANDSCAPE; i <= RGT_DEFAULT; ++i)
	{
		for (int j = 0; j < m_tRenderGroup[i].iSize; ++j)
		{
			m_tRenderGroup[i].pRenderObj[j]->Render(fTime);
		}

		m_tRenderGroup[i].iSize = 0;
	}

	ResetMRT("GBuffer");
}

void CRenderManager::RenderLightAcc(float fTime)
{
	ClearMRT("LightAcc");
	SetMRT("LightAcc");

	m_pDepthDisable->SetState();
	m_pAccBlend->SetState();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	const list<CGameObject*>*	pLightList = pScene->GetLightList();

	list<CGameObject*>::const_iterator	iter;
	list<CGameObject*>::const_iterator	iterEnd = pLightList->end();

	m_pPointSmp->SetSampler(11, SCT_PIXEL);

	// GBuffer MRT 를 얻어온다.
	PMRT	pGBuffer = FindMRT("GBuffer");

	pGBuffer->vecTargets[1]->SetShader(12);
	pGBuffer->vecTargets[2]->SetShader(13);
	pGBuffer->vecTargets[3]->SetShader(14);

	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;

	CCamera*	pCamera = pScene->GetMainCamera();

	matView = pCamera->GetViewMatrix().mat;
	matProj = pCamera->GetProjMatrix().mat;

	SAFE_RELEASE(pCamera);

	tBuffer.matView = matView;
	tBuffer.matProj = matProj;
	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProj;
	tBuffer.matVP = matView * matProj;
	tBuffer.matInvProj = XMMatrixInverse(&XMMatrixDeterminant(matProj),
		matProj);

	tBuffer.matView = XMMatrixTranspose(tBuffer.matView.mat);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj.mat);
	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV.mat);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP.mat);
	tBuffer.matWP = XMMatrixTranspose(tBuffer.matWP.mat);
	tBuffer.matVP = XMMatrixTranspose(tBuffer.matVP.mat);
	tBuffer.matInvProj = XMMatrixTranspose(tBuffer.matInvProj.mat);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tBuffer, SCT_PIXEL);

	m_pLightAccShader->SetShader();

	for (iter = pLightList->begin(); iter != iterEnd; ++iter)
	{
		if (!(*iter)->GetEnable())
			continue;

		CLight*	pLight = (*iter)->FindComponentFromType<CLight>(CT_LIGHT);

		if (!pLight->GetEnable())
		{
			SAFE_RELEASE(pLight);
			continue;
		}

		pLight->SetShader();

		SAFE_RELEASE(pLight);
		UINT	iOffset = 0;
		CMLIST->IASetInputLayout(NULL);
		CMLIST->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
		CMLIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		CMLIST->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
		CMLIST->Draw(4, 0);
	}

	SAFE_RELEASE(pScene);

	pGBuffer->vecTargets[1]->ResetShader(12);
	pGBuffer->vecTargets[2]->ResetShader(13);
	pGBuffer->vecTargets[3]->ResetShader(14);

	m_pDepthDisable->ResetState();
	m_pAccBlend->ResetState();

	ResetMRT("LightAcc");
}

void CRenderManager::RenderLightBlend(float fTime)
{
	CRenderingTarget*	pTarget = FindRenderTarget("LightBlend");

	pTarget->SetTarget();
	pTarget->ClearTarget();

	CRenderingTarget*	pAlbedo = FindRenderTarget("Albedo");
	CRenderingTarget*	pLightDif = FindRenderTarget("LightDif");
	CRenderingTarget*	pLightSpc = FindRenderTarget("LightSpc");

	pAlbedo->SetShader(11);
	pLightDif->SetShader(15);
	pLightSpc->SetShader(16);

	m_pLightBlendShader->SetShader();

	UINT	iOffset = 0;
	CMLIST->IASetInputLayout(NULL);
	CMLIST->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
	CMLIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CMLIST->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
	CMLIST->Draw(4, 0);

	pAlbedo->ResetShader(11);
	pLightDif->ResetShader(15);
	pLightSpc->ResetShader(16);

	pTarget->ResetTarget();

	SAFE_RELEASE(pAlbedo);
	SAFE_RELEASE(pLightDif);
	SAFE_RELEASE(pLightSpc);
	SAFE_RELEASE(pTarget);
}

void CRenderManager::RenderDeferredTarget(float fTime)
{
	m_pDepthDisable->SetState();

	CRenderingTarget*	pDeferred = FindRenderTarget("LightBlend");

	pDeferred->SetShader(17);

	m_pDeferredShader->SetShader();

	UINT	iOffset = 0;
	CMLIST->IASetInputLayout(NULL);
	CMLIST->IASetVertexBuffers(0, 0, NULL, 0, &iOffset);
	CMLIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CMLIST->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
	CMLIST->Draw(4, 0);

	pDeferred->ResetShader(17);

	SAFE_RELEASE(pDeferred);

	m_pDepthDisable->ResetState();
}

bool CRenderManager::SortZ(CGameObject * pSrc, CGameObject * pDest)
{
	CTransform* pSrcTr = pSrc->GetTransform();
	CTransform* pDestTr = pDest->GetTransform();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CCamera* pCamera = pScene->GetMainCamera();

	SAFE_RELEASE(pScene);

	Matrix matView = pCamera->GetViewMatrix();

	Vector3 vScrPos = pSrcTr->GetWorldPos();
	Vector3 vDestPos = pDestTr->GetWorldPos();

	vScrPos = vScrPos.TransformCoord(matView.mat);
	vDestPos = vDestPos.TransformCoord(matView.mat);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pSrcTr);
	SAFE_RELEASE(pDestTr);

	return vScrPos.z < vDestPos.z;
}

bool CRenderManager::SortAlpha(CGameObject * pSrc, CGameObject * pDest)
{
	CTransform* pSrcTr = pSrc->GetTransform();
	CTransform* pDestTr = pDest->GetTransform();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CCamera* pCamera = pScene->GetMainCamera();

	SAFE_RELEASE(pScene);

	Matrix matView = pCamera->GetViewMatrix();

	Vector3 vScrPos = pSrcTr->GetWorldPos();
	Vector3 vDestPos = pDestTr->GetWorldPos();

	vScrPos = vScrPos.TransformCoord(matView.mat);
	vDestPos = vDestPos.TransformCoord(matView.mat);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pSrcTr);
	SAFE_RELEASE(pDestTr);

	return vScrPos.z > vDestPos.z;
}

bool CRenderManager::SortUI(CGameObject * pSrc, CGameObject * pDest)
{
	CUI*	pSrcUI = pSrc->FindComponentFromType<CUI>(CT_UI);
	CUI*	pDestUI = pDest->FindComponentFromType<CUI>(CT_UI);

	int iSrcZ = pSrcUI->GetZOrder();
	int iDestZ = pDestUI->GetZOrder();

	SAFE_RELEASE(pSrcUI);
	SAFE_RELEASE(pDestUI);

	return iSrcZ > iDestZ;
}
