#include "RenderingTarget.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "../Device.h"
#include "../Core/PathManager.h"
#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Component/Camera.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"

CRenderingTarget::CRenderingTarget() :
	m_pTargetView(NULL),
	m_pTargetSRV(NULL),
	m_pOldDepth(NULL),
	m_pOldTargetView(NULL),
	m_pDepthTex(NULL),
	m_pDepthView(NULL),
	m_pMesh(NULL),
	m_pShader(NULL),
	m_pLayout(NULL),
	m_bDebug(false)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);

	for ( int i = 0; i < 2; ++i )
		m_pTargetTex[i] = NULL;
}

CRenderingTarget::~CRenderingTarget()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pOldTargetView);
	SAFE_RELEASE(m_pOldDepth);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pTargetSRV);
	for ( int i = 0; i < 2; ++i )
		SAFE_RELEASE( m_pTargetTex[i] );
}

ID3D12DescriptorHeap* CRenderingTarget::GetRenderTargetView() const
{
	return m_pTargetView;
}

ID3D12DescriptorHeap * CRenderingTarget::GetDepthStencilView() const
{
	return m_pDepthView;
}

ID3D12DescriptorHeap * CRenderingTarget::GetShaderResourceView() const
{
	return m_pTargetSRV;
}

bool CRenderingTarget::GetDebugEnable() const
{
	return m_bDebug;
}

void CRenderingTarget::SetDebugEnable(bool bDebug)
{
	m_bDebug = bDebug;

	if (bDebug)
	{
		if (!m_pMesh)
			m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("RectOrthoLTTex");

		if (!m_pLayout)
			m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexTex");
	}
}

bool CRenderingTarget::CreateTarget(unsigned int iW, unsigned int iH,
	DXGI_FORMAT eFmt, const Vector4& vClearColor,
	DXGI_FORMAT eDepthFmt)
{
	D3D12_RESOURCE_DESC tDesc = {};
	tDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	tDesc.Alignment = 0;
	tDesc.Width = iW;
	tDesc.Height = iH;
	tDesc.MipLevels = 1;
	tDesc.DepthOrArraySize = 1;
	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	tDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	for (int i = 0; i < 4; ++i)
	{
		m_fClearColor[i] = vClearColor[i];
	}

	// 렌더 타겟 서술자 힙을 생성하고 원소의 크기를 저장
	D3D12_DESCRIPTOR_HEAP_DESC		tDescriptorDesc = {};
	tDescriptorDesc.NumDescriptors = 2;																																					// 서술자의 개수 = 스왑체인 버퍼의 개수
	tDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	tDescriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	tDescriptorDesc.NodeMask = 0;
	if ( FAILED( DEVICE->CreateDescriptorHeap( &tDescriptorDesc, __uuidof( ID3D12DescriptorHeap ), ( void** )&m_pTargetView ) ) )			// 렌더 타겟 서술자 힙을 생성
		return false;
	m_iRTVSize = DEVICE->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );			// 렌더 타겟 서술자 힙의 원소의 크기를 저장

	auto pRTVHeap = m_pTargetView->GetCPUDescriptorHandleForHeapStart();
	for ( int i = 0; i < 2; ++i )
	{
		if(FAILED( SWAPCHAIN->GetBuffer( i, __uuidof( ID3D12Resource ), ( void** )m_pTargetTex[i] )))
			return false;

		DEVICE->CreateRenderTargetView( m_pTargetTex[i], NULL, pRTVHeap );
		pRTVHeap.ptr += m_iRTVSize;

		DEVICE->CreateShaderResourceView( m_pTargetTex[i], NULL, m_pTargetSRV->GetCPUDescriptorHandleForHeapStart() );
	}


	if (eDepthFmt != DXGI_FORMAT_UNKNOWN)
	{
		memset(&tDesc, 0, sizeof(tDesc));
		tDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		tDesc.Alignment = 0;
		tDesc.Width = iW;
		tDesc.Height = iH;
		tDesc.MipLevels = 1;
		tDesc.DepthOrArraySize = 1;
		tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		tDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE	tClearValue = {};
		tClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tClearValue.DepthStencil.Depth = 1.f;
		tClearValue.DepthStencil.Stencil = 0.f;

		D3D12_HEAP_PROPERTIES tHeapProperties = {};
		tHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
		tHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		tHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		tHeapProperties.CreationNodeMask = 1;
		tHeapProperties.VisibleNodeMask = 1;
		

		// 깊이-스텐실 서술자 힙 생성하고 원소의 크기를 저장
		tDescriptorDesc.NumDescriptors = 1;
		tDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		if ( FAILED( DEVICE->CreateDescriptorHeap( &tDescriptorDesc, __uuidof( ID3D12DescriptorHeap ), ( void** )&m_pDepthView ) ) )
			return false;
		m_iDSVSize = DEVICE->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_DSV );			// 깊이-스텐실 서술자 힙의 원소의 크기를 저장


		if ( FAILED( DEVICE->CreateCommittedResource( &tHeapProperties, D3D12_HEAP_FLAG_NONE,
			&tDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &tClearValue, __uuidof( ID3D12Resource ), ( void** )&m_pTargetTex) ) )
			return false;

		// 생성한 깊이버퍼를 이용해서 DepthStencilView를 만들어준다.
		auto pDSView = m_pDepthView->GetCPUDescriptorHandleForHeapStart();
		DEVICE->CreateDepthStencilView( m_pDepthTex, NULL, pDSView );
	}

	return true;
}

void CRenderingTarget::SetDebugInfo(const Vector3 & vPos, const Vector3 & vScale)
{
	m_vPos = vPos;
	m_vScale = vScale;
}

void CRenderingTarget::SetTarget()
{
	if (m_pOldTargetView || m_pOldDepth)
		return;

	//float	fColor[4] = {};
	//ClearTarget(fColor);

	/*CMDLIST->OMGetRenderTargets(1, &m_pOldTargetView, &m_pOldDepth);

	ID3D12DepthStencilView*	pDepthView = m_pOldDepth;

	if (m_pDepthView)
		pDepthView = m_pDepthView;

	CMDLIST->OMSetRenderTargets(1, &m_pTargetView, pDepthView);*/
	auto pRTVHeap = m_pTargetView->GetCPUDescriptorHandleForHeapStart();
	int iSwapChainIdx = SWAPCHAIN->GetCurrentBackBufferIndex();
	pRTVHeap.ptr += (  iSwapChainIdx * m_iRTVSize );
	CMDLIST->ClearRenderTargetView( pRTVHeap, m_fClearColor, 0, NULL );
	auto pDSVHeap = m_pDepthView->GetCPUDescriptorHandleForHeapStart();
	CMDLIST->ClearDepthStencilView( pDSVHeap,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, NULL );

	CMDLIST->OMSetRenderTargets( 1, &pRTVHeap, TRUE, &pDSVHeap );

}

void CRenderingTarget::ResetTarget()
{
	if (!m_pOldTargetView)
		return;

	auto pRTVHeap = m_pOldTargetView->GetCPUDescriptorHandleForHeapStart();
	int iSwapChainIdx = SWAPCHAIN->GetCurrentBackBufferIndex();
	pRTVHeap.ptr += ( iSwapChainIdx * m_iRTVSize );
	auto pDSVHeap = m_pOldDepth->GetCPUDescriptorHandleForHeapStart();
	
	CMDLIST->OMSetRenderTargets( 1, &pRTVHeap, TRUE, &pDSVHeap );
	
	SAFE_RELEASE(m_pOldDepth);
	SAFE_RELEASE(m_pOldTargetView);
}

void CRenderingTarget::SaveFile(const wchar_t * pFileName,
	const string & strPathKey)
{
	const wchar_t* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	wstring	strPath;

	if (pPath)
		strPath = pPath;

	strPath += pFileName;

	ScratchImage	img;
	CaptureTexture(DEVICE, CMDLIST, m_pTargetTex, img);
	SaveToWICFile(img.GetImages(), img.GetImageCount(),
		WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), strPath.c_str());
}

void CRenderingTarget::SaveFileFromFullPath(const wchar_t * pFullPath)
{
	ScratchImage	img;
	CaptureTexture(DEVICE, CMDLIST, m_pTargetTex, img);
	SaveToWICFile(img.GetImages(), img.GetImageCount(),
		WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), pFullPath);
}

void CRenderingTarget::ClearTarget()
{
	auto pRTVHeap = m_pTargetView->GetCPUDescriptorHandleForHeapStart();
	int iSwapChainIdx = SWAPCHAIN->GetCurrentBackBufferIndex();
	pRTVHeap.ptr += ( iSwapChainIdx * m_iRTVSize );
	CMDLIST->ClearRenderTargetView( pRTVHeap, m_fClearColor, 0, NULL );

	if ( m_pDepthView )
	{
		auto pDSVHeap = m_pDepthView->GetCPUDescriptorHandleForHeapStart();
		CMDLIST->ClearDepthStencilView( pDSVHeap, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, NULL );
	}
}

void CRenderingTarget::DebugRender(float fTime)
{
	TRANSFORMCBUFFER	tBuffer = {};

	XMMATRIX	matView, matProj;

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CCamera*	pCamera = pScene->GetUICamera();

	SAFE_RELEASE(pScene);

	matView = pCamera->GetViewMatrix().mat;
	matProj = pCamera->GetProjMatrix().mat;

	SAFE_RELEASE(pCamera);

	Matrix	matScale, matPos;
	matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	matPos = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	tBuffer.matWorld = matScale * matPos;
	tBuffer.matView = matView;
	tBuffer.matProj = matProj;
	tBuffer.matWV = tBuffer.matWorld * tBuffer.matView;
	tBuffer.matWVP = tBuffer.matWV * tBuffer.matProj;
	tBuffer.matWP = tBuffer.matWorld * tBuffer.matProj;
	tBuffer.matVP = matView * matProj;
	tBuffer.vPivot = Vector3::Zero;
	tBuffer.vLength = m_pMesh->GetLength();

	tBuffer.matWorld = XMMatrixTranspose(tBuffer.matWorld.mat);
	tBuffer.matView = XMMatrixTranspose(tBuffer.matView.mat);
	tBuffer.matProj = XMMatrixTranspose(tBuffer.matProj.mat);
	tBuffer.matWV = XMMatrixTranspose(tBuffer.matWV.mat);
	tBuffer.matWVP = XMMatrixTranspose(tBuffer.matWVP.mat);
	tBuffer.matWP = XMMatrixTranspose(tBuffer.matWP.mat);
	tBuffer.matVP = XMMatrixTranspose(tBuffer.matVP.mat);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&tBuffer, SCT_VERTEX | SCT_PIXEL | SCT_GEOMETRY);

	ANIMATION2DBUFFER	tAniBuffer = {};
	tAniBuffer.iType = A2D_NONE;
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Animation2D",
		&tAniBuffer, SCT_VERTEX | SCT_PIXEL);

	m_pShader->SetShader();
	CMDLIST->IASetInputLayout(m_pLayout);
	CMDLIST->PSSetShaderResources(0, 1, &m_pTargetSRV);

	m_pMesh->Render(fTime);
}

void CRenderingTarget::SetShader(int iRegister)
{
	CMDLIST->PSSetShaderResources(iRegister, 1, &m_pTargetSRV);
}

void CRenderingTarget::ResetShader(int iRegister)
{
	ID3D12ShaderResourceView*	pView = NULL;
	CMDLIST->PSSetShaderResources(iRegister, 1, &pView);
}
