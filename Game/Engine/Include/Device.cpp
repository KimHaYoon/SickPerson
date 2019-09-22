#include "Device.h"

Engine_USING

DEFINITION_SINGLE(CDevice)

CDevice::CDevice() :
	m_pDevice(NULL),
	m_pFactory(NULL),
	m_pSwapChain(NULL),
	m_pRTVHeap( NULL ),
	m_pDSView( NULL ),
	m_pDSVHeap( NULL ),
	m_pCommandQueue(NULL),
	m_pCommandAllocator(NULL),
	m_pCommandList(NULL),
	m_pPipelineState(NULL),
	m_pFence(NULL),
	m_hFenceEvent(NULL)
{
	memset(m_fClearColor, 0, sizeof(float) * 4);
	//m_fClearColor[1] = 1.f;

	m_iRTVSize = 0;
	m_iDSVSize = 0;
	m_iFenceValue = 0;
	m_iMsaa4xQualityLevels = 0;
	m_iSwapChainSize = 2;
	m_bMsaa4xEnable = false;

	m_pRTView = new ID3D12Resource*[m_iSwapChainSize];

	for ( int i = 0; i < m_iSwapChainSize; ++i )
		m_pRTView[i] = NULL;
}

CDevice::~CDevice()
{
	SAFE_RELEASE( m_pDebugController );

	SAFE_RELEASE(m_pDSView);
	SAFE_RELEASE( m_pDSVHeap );

	for ( int i = 0; i < m_iSwapChainSize; ++i )
		SAFE_RELEASE( m_pRTView[i] );
	SAFE_RELEASE( m_pRTVHeap );

	SAFE_RELEASE( m_pCommandAllocator );
	SAFE_RELEASE( m_pCommandQueue );
	SAFE_RELEASE( m_pCommandList );

	SAFE_RELEASE( m_pPipelineState );

	CloseHandle( m_hFenceEvent );
	SAFE_RELEASE( m_pFence );

	m_pSwapChain->SetFullscreenState( FALSE, NULL );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pDevice );
	SAFE_RELEASE( m_pFactory );
}

ID3D12Device * CDevice::GetDevice() const
{
	return m_pDevice;
}

//ID3D11DeviceContext * CDevice::GetContext() const
//{
//	return m_pContext;
//}

IDXGISwapChain3 * CDevice::GetSwapChain() const
{
	return m_pSwapChain;
}

RESOLUTION CDevice::GetResolution() const
{
	return m_tRS;
}

bool CDevice::Init(HWND hWnd, UINT iWidth, UINT iHeight,
	bool bWindowMode)
{
	m_hWnd = hWnd;
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	UINT	iFlag = 0;
#ifdef _DEBUG
	D3D12GetDebugInterface( __uuidof( ID3D12Debug ), ( void ** )&m_pDebugController );
	m_pDebugController->EnableDebugLayer();

#endif // _DEBUG

	// Create Device
	CreateDXGIFactory1( __uuidof( IDXGIFactory4 ), ( void ** )&m_pFactory );		// ���丮 ����
	IDXGIAdapter1* pAdapter = NULL;
	for ( int i = 0; DXGI_ERROR_NOT_FOUND != m_pFactory->EnumAdapters1( i, &pAdapter ); ++i )
	{
		DXGI_ADAPTER_DESC1	tAdapterDesc;
		pAdapter->GetDesc1( &tAdapterDesc );
		if( tAdapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;
		if(SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_0,  __uuidof(ID3D12Device), (void**)&m_pDevice )))			// ��� �ϵ���� ����� ���Ͽ� Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽� ����
			break;
	}

	if ( !pAdapter )
	{
		m_pFactory->EnumWarpAdapter( __uuidof( IDXGIFactory4 ), ( void** )&pAdapter );
		D3D12CreateDevice( pAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof( ID3D12Device ), ( void** )&m_pDevice );		// Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� ������ �� ������ WARP ����̽� ����
	}

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS tQualityLevels;
	tQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tQualityLevels.SampleCount = 4;						// Msaa4x ���� ���ø�
	tQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	tQualityLevels.NumQualityLevels = 0;
	m_pDevice->CheckFeatureSupport( D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &tQualityLevels, sizeof( D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS ) );
	m_iMsaa4xQualityLevels = tQualityLevels.NumQualityLevels;			// ����̽��� �����ϴ� ���� ������ ǰ�� ������ Ȯ��
	m_bMsaa4xEnable = ( m_iMsaa4xQualityLevels > 1 ) ? true : false;		// ���� ������ ǰ�� ������ 1���� ũ�� ���� ���ø��� Ȱ��ȭ
	m_hFenceEvent = CreateEvent( NULL, FALSE, FALSE, NULL );			// �潺�� ����ȭ�� ���� �̺�Ʈ ��ü�� ����. �̺�Ʈ�� ����Ǹ� �̺�Ʈ�� ���� �ڵ������� FASLE�� �ǵ��� ����

	m_tViewport.TopLeftX = 0;
	m_tViewport.TopLeftY = 0;
	m_tViewport.Width = iWidth;
	m_tViewport.Height = iHeight;
	m_tViewport.MinDepth = 0.f;
	m_tViewport.MaxDepth = 1.f;

	if ( pAdapter )
		SAFE_RELEASE( pAdapter );

	// CreateCommandQueueAndList
	D3D12_COMMAND_QUEUE_DESC	tQueueDesc = {};
	tQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	tQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hResult = m_pDevice->CreateCommandQueue( &tQueueDesc, __uuidof( ID3D12CommandQueue ), ( void** )&m_pCommandQueue );			// ���� ��� ť�� ����
	hResult = m_pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof( ID3D12CommandAllocator ), ( void** )&m_pCommandAllocator );		// ���� ��� �Ҵ��ڸ� ����
	hResult = m_pDevice->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator, NULL, __uuidof( ID3D12GraphicsCommandList ), ( void** )&m_pCommandList );		// ���� ��� ����Ʈ�� ����
	hResult = m_pCommandList->Close();				// ��� ����Ʈ�� �����Ǹ� ���� �����̹Ƿ� ���� ���·� �����.
	
	// Create SwapChain
	DXGI_SWAP_CHAIN_DESC1	tDesc = {};

	tDesc.Width = iWidth;
	tDesc.Height = iHeight;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferCount = m_iSwapChainSize;
	tDesc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	tDesc.Flags = iFlag;
	
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC		tFullDesc = {};
	tFullDesc.RefreshRate.Numerator = 60;
	tFullDesc.RefreshRate.Denominator = 1;
	tFullDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tFullDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;			// ������������ �����ϸ�
	tFullDesc.Windowed = bWindowMode;
	
	if(FAILED(m_pFactory->CreateSwapChainForHwnd(m_pCommandQueue, m_hWnd, 
		&tDesc, &tFullDesc, NULL, (IDXGISwapChain1**)&m_pSwapChain)))
		return false;
	
	m_iSwapChainIdx = m_pSwapChain->GetCurrentBackBufferIndex();
	
	// CreateRtvAndDsvDescriptorHeaps
	D3D12_DESCRIPTOR_HEAP_DESC		tDescriptorDesc = {};
	tDescriptorDesc.NumDescriptors = m_iSwapChainSize;																																					// �������� ���� = ����ü�� ������ ����
	tDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	tDescriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	tDescriptorDesc.NodeMask = 0;																																																
	hResult = m_pDevice->CreateDescriptorHeap( &tDescriptorDesc, __uuidof( ID3D12DescriptorHeap ), ( void** )&m_pRTVHeap );			// ���� Ÿ�� ������ ���� ����																																															
	m_iRTVSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );			// ���� Ÿ�� ������ ���� ������ ũ�⸦ ����
	tDescriptorDesc.NumDescriptors = 1;
	tDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hResult = m_pDevice->CreateDescriptorHeap( &tDescriptorDesc, __uuidof( ID3D12DescriptorHeap ), ( void** )&m_pDSVHeap );
	m_iDSVSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_DSV );			// ����-���ٽ� ������ ���� ������ ũ�⸦ ����
		
	// CreateRenderTargetView
	D3D12_CPU_DESCRIPTOR_HANDLE tRtvCPUDescriptorHandle = m_pRTVHeap->GetCPUDescriptorHandleForHeapStart();
	for ( UINT i = 0; i < m_iSwapChainSize; ++i )
	{
		hResult = m_pSwapChain->GetBuffer( i, __uuidof( ID3D12Resource ), ( void	** )&m_pRTView[i] );
		m_pDevice->CreateRenderTargetView( m_pRTView[i], NULL,	tRtvCPUDescriptorHandle );
		tRtvCPUDescriptorHandle.ptr += m_iRTVSize;
	}

	// CreateDepthStencilView
	D3D12_RESOURCE_DESC tResourceDesc = {};
	tResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	tResourceDesc.Alignment = 0;
	tResourceDesc.Width = iWidth;
	tResourceDesc.Height = iHeight;
	tResourceDesc.DepthOrArraySize = 1;
	tResourceDesc.MipLevels = 1;
	tResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tResourceDesc.SampleDesc.Count = ( m_bMsaa4xEnable ) ? 4 : 1;
	tResourceDesc.SampleDesc.Quality = ( m_bMsaa4xEnable ) ? ( m_iMsaa4xQualityLevels - 1 ) : 0;
	tResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	tResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES tHeapProperties = {};
	tHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	tHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	tHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	tHeapProperties.CreationNodeMask = 1;
	tHeapProperties.VisibleNodeMask = 1;
	D3D12_CLEAR_VALUE d3dClearValue;
	d3dClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dClearValue.DepthStencil.Depth = 1.0f;
	d3dClearValue.DepthStencil.Stencil = 0;
	m_pDevice->CreateCommittedResource( &tHeapProperties, D3D12_HEAP_FLAG_NONE,	&tResourceDesc, 
		D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dClearValue,	__uuidof( ID3D12Resource ), ( void ** )&m_pDSView );			//����-���ٽ� ���۸� �����Ѵ�. 
	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pDSVHeap->GetCPUDescriptorHandleForHeapStart();
	m_pDevice->CreateDepthStencilView( m_pDSView, NULL, d3dDsvCPUDescriptorHandle );														//����-���ٽ� ���� �並 �����Ѵ�.

	return true;
}

//void CDevice::ClearTarget()
//{
//	m_pContext->ClearRenderTargetView(m_pRTView, m_fClearColor);
//	m_pContext->ClearDepthStencilView(m_pDSView,
//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
//}

void CDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}

void CDevice::WaitForGpuComplete()
{
	++m_iFenceValue;				// CPU�潺 �� ����

	const UINT64 iFence = m_iFenceValue;
	HRESULT hResult = m_pCommandQueue->Signal( m_pFence, iFence );			// GPU�� �潺�� ���� �����ϴ� ����� ��� ť�� �߰�

	if ( m_pFence->GetCompletedValue() < iFence )
	{
		hResult = m_pFence->SetEventOnCompletion( iFence, m_hFenceEvent );
		WaitForSingleObject( m_hFenceEvent, INFINITE );
	}
}

Vector2 CDevice::GetWindowDeviceResolution() const
{
	RECT	rc;
	GetClientRect(m_hWnd, &rc);
	Vector2	v;
	v.x = m_tRS.iWidth / (float)(rc.right - rc.left);
	v.y = m_tRS.iHeight / (float)(rc.bottom - rc.top);

	return v;
}

Vector2 CDevice::GetWindowResolution() const
{
	RECT	rc;
	GetClientRect(m_hWnd, &rc);

	return Vector2(rc.right - rc.left, rc.bottom - rc.top);
}
