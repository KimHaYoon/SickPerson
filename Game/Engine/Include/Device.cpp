#include "Device.h"

Engine_USING

DEFINITION_SINGLE(CDevice)

CDevice::CDevice() :
	m_pDevice(NULL),
	m_pCmdList(NULL),
	m_pCmdQueue(NULL),
	m_pCmdAlloc(NULL),
	m_pSwapChain(NULL),
	m_pRTView(NULL),
	m_pDepthBuffer(NULL),
	m_pDSView(NULL),
	m_pDebug(NULL),
	m_pFence(NULL),
	m_hFenceEvent(NULL),
	m_pFactory(NULL)
{
	//memset(m_fClearColor, 1.f, sizeof(float) * 4);
	m_fClearColor[0] = 0.f;
	m_fClearColor[1] = 0.125f;
	m_fClearColor[2] = 0.3f;
	m_fClearColor[3] = 1.f;

	for ( int i = 0; i < 2; ++i )
	{
		m_pRTVBuffer[i] = NULL;
		m_iFenceValues[i] = 0;
	}

	memset( &m_tResourceBarrier, 0, sizeof( D3D12_RESOURCE_BARRIER ));
}

CDevice::~CDevice()
{
	CloseHandle( m_hFenceEvent );
	SAFE_RELEASE( m_pFence );

	SAFE_RELEASE( m_pDebug );
	SAFE_RELEASE(m_pDepthBuffer);
	SAFE_RELEASE(m_pDSView);

	SAFE_RELEASE(m_pRTView);
	m_pSwapChain->SetFullscreenState( FALSE, NULL );
	SAFE_RELEASE(m_pSwapChain);

	for ( int i = 0; i < 2; ++i )
		SAFE_RELEASE( m_pRTVBuffer[i] );
	SAFE_RELEASE( m_pCmdQueue );
	SAFE_RELEASE( m_pCmdAlloc );
	SAFE_RELEASE( m_pCmdList );
	SAFE_RELEASE( m_pDevice );
	SAFE_RELEASE( m_pFactory );
}

ID3D12Device * CDevice::GetDevice() const
{
	return m_pDevice;
}

ID3D12GraphicsCommandList * CDevice::GetCmdList() const
{
	return m_pCmdList;
}

IDXGISwapChain * CDevice::GetSwapChain() const
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
	HRESULT Debug = D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void **)&m_pDebug );
	m_pDebug->EnableDebugLayer();
#endif // _DEBUG

	CreateDXGIFactory1( __uuidof( IDXGIFactory4 ), ( void ** )&m_pFactory );		// ���丮 ����
	IDXGIAdapter1* pAdapter = NULL;
	D3D_FEATURE_LEVEL	eLevel = D3D_FEATURE_LEVEL_12_0;

	if ( FAILED( D3D12CreateDevice( NULL, eLevel, __uuidof( ID3D12Device ), ( void** )&m_pDevice ) ) )
	{
		m_pFactory->EnumWarpAdapter( __uuidof( IDXGIFactory4 ), ( void** )&pAdapter );
		D3D12CreateDevice( pAdapter, D3D_FEATURE_LEVEL_11_0, __uuidof( ID3D12Device ), ( void** )&m_pDevice );		// Ư�� ���� 12.0�� �����ϴ� �ϵ���� ����̽��� ������ �� ������ WARP ����̽� ����
	}
	m_pDevice->CreateFence( 0, D3D12_FENCE_FLAG_NONE, __uuidof( ID3D12Fence ), ( void ** )&m_pFence );
	//�潺�� �����ϰ� �潺 ���� 0���� �����Ѵ�.
	for ( int i = 0; i < 2; i++ )
		m_iFenceValues[i] = 0;

	//�潺�� ����ȭ�� ���� �̺�Ʈ ��ü�� �����Ѵ�(�̺�Ʈ ��ü�� �ʱⰪ�� FALSE�̴�).	�̺�Ʈ�� ����Ǹ�( Signal ) �̺�Ʈ�� ���� �ڵ������� FALSE�� �ǵ��� �����Ѵ�.
	m_hFenceEvent = ::CreateEvent( NULL, FALSE, FALSE, NULL );

	SAFE_RELEASE( pAdapter );

	D3D12_COMMAND_QUEUE_DESC tCmdQueueDesc;
	::ZeroMemory( &tCmdQueueDesc, sizeof( D3D12_COMMAND_QUEUE_DESC ) );
	tCmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	tCmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	//����(Direct) ��� ť�� �����Ѵ�.
	if ( FAILED( m_pDevice->CreateCommandQueue( &tCmdQueueDesc,
		_uuidof( ID3D12CommandQueue ), ( void ** )&m_pCmdQueue ) ) )
		return false;

	//����(Direct) ��� �Ҵ��ڸ� �����Ѵ�.
	if ( FAILED( m_pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT,
		__uuidof( ID3D12CommandAllocator ), ( void ** )&m_pCmdAlloc ) ) )
		return false;

	//����(Direct) ��� ����Ʈ�� �����Ѵ�.
	if ( FAILED( m_pDevice->CreateCommandList( 0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAlloc, NULL,
		__uuidof( ID3D12GraphicsCommandList ), ( void ** )&m_pCmdList ) ) )
		return false;

	//��� ����Ʈ�� �����Ǹ� ����(Open) �����̹Ƿ� ����(Closed) ���·� �����. 
	if ( FAILED( m_pCmdList->Close() ) )
		return false;

	DXGI_SWAP_CHAIN_DESC	tDesc = {};
	tDesc.BufferCount = 2;
	tDesc.BufferDesc.Width = iWidth;
	tDesc.BufferDesc.Height = iHeight;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	tDesc.OutputWindow = m_hWnd;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Windowed = bWindowMode;
	//��üȭ�� ��忡�� ����ȭ���� �ػ󵵸� ����ü��(�ĸ����)�� ũ�⿡ �°� �����Ѵ�. 
	tDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if ( FAILED( m_pFactory->CreateSwapChain( m_pCmdQueue, &tDesc, ( IDXGISwapChain** )&m_pSwapChain ) ) )
		return false;

	m_iSwapChainIdx = m_pSwapChain->GetCurrentBackBufferIndex();

	// ���� Ÿ�� �����ڿ� ����-���ٽ� ������ ���� �����ϰ� ������ ũ�⸦ ����
	D3D12_DESCRIPTOR_HEAP_DESC		tDescriptorDesc = {};
	tDescriptorDesc.NumDescriptors = 2;																																					// �������� ���� = ����ü�� ������ ����
	tDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	tDescriptorDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	tDescriptorDesc.NodeMask = 0;
	if(FAILED(m_pDevice->CreateDescriptorHeap( &tDescriptorDesc, __uuidof( ID3D12DescriptorHeap ), ( void** )&m_pRTView )))			// ���� Ÿ�� ������ ���� ����																																															
		return false;
	m_iRTVSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );			// ���� Ÿ�� ������ ���� ������ ũ�⸦ ����
	tDescriptorDesc.NumDescriptors = 1;
	tDescriptorDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	if ( FAILED( m_pDevice->CreateDescriptorHeap( &tDescriptorDesc, __uuidof( ID3D12DescriptorHeap ), ( void** )&m_pDSView ) ) )
		return false;
	m_iDSVSize = m_pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_DSV );			// ����-���ٽ� ������ ���� ������ ũ�⸦ ����

	// ����۸� ���´�.
	auto pRTVHeap = m_pRTView->GetCPUDescriptorHandleForHeapStart();
	for ( int i = 0; i < 2; ++i )
	{
		m_pSwapChain->GetBuffer( i, __uuidof( ID3D12Resource ), ( void** )&m_pRTVBuffer[i] );

		// ���� ����۸� �̿��ؼ� ������ Ÿ�� �並 ������ش�.
		m_pDevice->CreateRenderTargetView( m_pRTVBuffer[i], NULL, pRTVHeap );
		pRTVHeap.ptr += m_iRTVSize;
	}

	// ���̹��ۿ� �ؽ��ĸ� �����.
	D3D12_RESOURCE_DESC 	tDepthDesc = {};
	tDepthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	tDepthDesc.Alignment = 0;
	tDepthDesc.Width = iWidth;
	tDepthDesc.Height = iHeight;
	tDepthDesc.MipLevels = 1;
	tDepthDesc.DepthOrArraySize = 1;
	tDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDepthDesc.SampleDesc.Count = 1;
	tDepthDesc.SampleDesc.Quality = 0;
	tDepthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	tDepthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

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

	if (FAILED(m_pDevice->CreateCommittedResource(&tHeapProperties, D3D12_HEAP_FLAG_NONE,
		&tDepthDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &tClearValue, __uuidof( ID3D12Resource ), (void** )&m_pDepthBuffer)))
		return false;

	// ������ ���̹��۸� �̿��ؼ� DepthStencilView�� ������ش�.
	auto pDSView = m_pDSView->GetCPUDescriptorHandleForHeapStart();
	m_pDevice->CreateDepthStencilView( m_pDepthBuffer, NULL, pDSView );

	// ������ ������ ����Ÿ�ٺ�� ���̽��ٽǺ並 ������ ������������
	// ��º��ձ� �ܰ迡 �����ش�.
	//pRTVHeap = m_pRTView->GetCPUDescriptorHandleForHeapStart();
	//pDSView = m_pDSView->GetCPUDescriptorHandleForHeapStart();
	//m_pCmdList->OMSetRenderTargets(1, &pRTVHeap, TRUE, &pDSView );

	// ����Ʈ ����. 
	//D3D12_VIEWPORT	tVP = {};

	//tVP.Width = iWidth;
	//tVP.Height = iHeight;
	//tVP.MaxDepth = 1.f;

	//m_pCmdList->	RSSetViewports(1, &tVP);
	
	return true;
}

void CDevice::ClearTarget()
{
	auto pRTVHeap = m_pRTView->GetCPUDescriptorHandleForHeapStart();
	pRTVHeap.ptr += ( m_iSwapChainIdx * m_iRTVSize );
	m_pCmdList->ClearRenderTargetView( pRTVHeap, m_fClearColor, 0, NULL);
	auto pDSVHeap= m_pDSView->GetCPUDescriptorHandleForHeapStart();
	m_pCmdList->ClearDepthStencilView( pDSVHeap,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0 , NULL);

	m_pCmdList->OMSetRenderTargets( 1, &pRTVHeap, TRUE, &pDSVHeap );
}

void CDevice::Present()
{
	m_tResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_tResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	m_tResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pCmdList->ResourceBarrier( 1, &m_tResourceBarrier );

	m_pCmdList->Close();

	ID3D12CommandList *ppd3dCommandLists[] = { m_pCmdList };
	m_pCmdQueue->ExecuteCommandLists( 1, ppd3dCommandLists );

	WaitForGpuComplete();

	m_pSwapChain->Present(0, 0);

	m_iSwapChainIdx = m_pSwapChain->GetCurrentBackBufferIndex();

	MoveToNextFrame();
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

void CDevice::CmdReset()
{
	HRESULT hResult  = m_pCmdAlloc->Reset();
	hResult = m_pCmdList->Reset( m_pCmdAlloc, NULL );

	/*���� ���� Ÿ�ٿ� ���� ������Ʈ�� �����⸦ ��ٸ���. ������Ʈ�� ������ ���� Ÿ�� ������ ���´� ������Ʈ ���� (D3D12_RESOURCE_STATE_PRESENT)���� 
	���� Ÿ�� ����(D3D12_RESOURCE_STATE_RENDER_TARGET)�� �ٲ� ���̴�.*/
	m_tResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_tResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_tResourceBarrier.Transition.pResource = m_pRTVBuffer[m_iSwapChainIdx];
	m_tResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_tResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_tResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_pCmdList->ResourceBarrier( 1, &m_tResourceBarrier );

	D3D12_VIEWPORT	tVP = {};

	tVP.Width = m_tRS.iWidth;
	tVP.Height = m_tRS.iHeight;
	tVP.MaxDepth = 1.f;

	m_pCmdList->RSSetViewports( 1, &tVP );

	D3D12_RECT m_RCcissorRect = { 0,0,m_tRS.iWidth, m_tRS.iHeight };
	m_pCmdList->RSSetScissorRects( 1, &m_RCcissorRect );

	//������ ���� Ÿ�ٿ� �ش��ϴ� �������� CPU �ּ�(�ڵ�)�� ����Ѵ�.
	D3D12_CPU_DESCRIPTOR_HANDLE pRTVHeap = m_pRTView->GetCPUDescriptorHandleForHeapStart();
	pRTVHeap.ptr += ( m_iSwapChainIdx * m_iRTVSize );
}

void CDevice::WaitForGpuComplete()
{
	UINT64 nFenceValue = ++m_iFenceValues[m_iSwapChainIdx];
	HRESULT hResult = m_pCmdQueue->Signal( m_pFence, nFenceValue );
	//GPU�� �潺�� ���� �����ϴ� ����� ��� ť�� �߰��Ѵ�. 
	if ( m_pFence->GetCompletedValue() < nFenceValue )
	{
		//�潺�� ���� ���� ������ ������ ������ �潺�� ���� ���� ������ ���� �� ������ ��ٸ���. 
		hResult = m_pFence->SetEventOnCompletion( nFenceValue, m_hFenceEvent );
		::WaitForSingleObject( m_hFenceEvent, INFINITE );
	}
}

void CDevice::MoveToNextFrame()
{
	m_iSwapChainIdx = m_pSwapChain->GetCurrentBackBufferIndex();
	//m_nSwapChainBufferIndex = (m_nSwapChainBufferIndex + 1) % m_nSwapChainBuffers;

	UINT64 nFenceValue = ++m_iFenceValues[m_iSwapChainIdx];
	HRESULT hResult = m_pCmdQueue->Signal( m_pFence, nFenceValue );

	if ( m_pFence->GetCompletedValue() < nFenceValue )
	{
		hResult = m_pFence->SetEventOnCompletion( nFenceValue, m_hFenceEvent );
		::WaitForSingleObject( m_hFenceEvent, INFINITE );
	}
}