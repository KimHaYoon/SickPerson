#pragma once

#include "Engine.h"

Engine_BEGIN

class Engine_DLL CDevice
{
private:
	ID3D12Device*					m_pDevice;				// Direct3D 디바이스 인터페이스
	IDXGIFactory4*				m_pFactory;			// DXGI 팩토리 인터페이스

	IDXGISwapChain3*			m_pSwapChain;		// 스왑 체인 인터페이스
	UINT									m_iSwapChainIdx;	// 스왑 체인 인덱스
	UINT									m_iSwapChainSize;

	ID3D12Resource**			m_pRTView;				// 렌더 타겟 뷰
	ID3D12DescriptorHeap*	m_pRTVHeap;			// 렌더 타켓 서술자 힙 인터페이스 포인터
	UINT									m_iRTVSize;				// 렌더 타겟 서술자 원소의 크기

	ID3D12Resource*				m_pDSView;				// 깊이 스텐실 뷰
	ID3D12DescriptorHeap*	m_pDSVHeap;			// 깊이 스텐실 힙 인터페이스 포인터
	UINT									m_iDSVSize;				// 깊이 스텐실 서술자 원소의 크기

	ID3D12CommandQueue*				m_pCommandQueue;				// 명령큐
	ID3D12CommandAllocator*			m_pCommandAllocator;			// 명령 할당자
	ID3D12GraphicsCommandList*	m_pCommandList;						// 명령 리스트 인터페이스 포인터

	ID3D12PipelineState*			m_pPipelineState;		// 그래픽스 파이프라인 상태 객체에 대한 인터페이스 포인터

	ID3D12Fence*	m_pFence;					// 펜스 인터페이스 포인터
	UINT64				m_iFenceValue;			// 펜스 값
	HANDLE				m_hFenceEvent;			// 이벤트 핸들
	
	float					m_fClearColor[4];
	RESOLUTION		m_tRS;
	HWND					m_hWnd;

	ID3D12Debug* m_pDebugController;

	bool					m_bMsaa4xEnable;
	UINT					m_iMsaa4xQualityLevels;

	D3D12_VIEWPORT		m_tViewport;

public:
	ID3D12Device* GetDevice()	const;
	//ID3D12DeviceContext* GetContext()	const;
	IDXGISwapChain3* GetSwapChain()	const;
	RESOLUTION GetResolution()	const;

public:
	bool Init(HWND hWnd, UINT iWidth, UINT iHeight, bool bWindowMode);
	//void ClearTarget();
	void Present();
	void WaitForGpuComplete();

public:
	Vector2 GetWindowDeviceResolution()	const;
	Vector2 GetWindowResolution()	const;

	DECLARE_SINGLE(CDevice)
};

Engine_END