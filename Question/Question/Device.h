#pragma once

#include "Engine.h"

class  CDevice
{
private:
	IDXGIFactory4*									m_pFactory;
	ID3D12Device*										m_pDevice;

	ID3D12GraphicsCommandList*		m_pCmdList;
	ID3D12CommandQueue*					m_pCmdQueue;
	ID3D12CommandAllocator*				m_pCmdAlloc;

	IDXGISwapChain3*								m_pSwapChain;
	UINT														m_iSwapChainIdx;

	ID3D12Resource*									m_pRTVBuffer[2];
	ID3D12DescriptorHeap*						m_pRTView;
	UINT														m_iRTVSize;

	ID3D12Resource*									m_pDepthBuffer;
	ID3D12DescriptorHeap*						m_pDSView;
	UINT														m_iDSVSize;

	float														m_fClearColor[4];
	RESOLUTION											m_tRS;
	HWND														m_hWnd;

	ID3D12Debug*										m_pDebug;

	D3D12_RESOURCE_BARRIER				m_tResourceBarrier;

	ID3D12Fence*	m_pFence;
	UINT64				m_iFenceValues[2];
	HANDLE				m_hFenceEvent;

public:
	ID3D12Device* GetDevice()	const;
	ID3D12GraphicsCommandList* GetCmdList()	const;
	IDXGISwapChain3* GetSwapChain()	const;
	RESOLUTION GetResolution()	const;

	ID3D12DescriptorHeap* GetRTVHeap() const;
	ID3D12Fence*	GetFence() const;

public:
	bool Init(HWND hWnd, UINT iWidth, UINT iHeight,
		bool bWindowMode);
	void ClearTarget();
	void Present();
	void CmdReset();
	void WaitForGpuComplete();
	void MoveToNextFrame();

public:
	Vector2 GetWindowDeviceResolution()	const;
	Vector2 GetWindowResolution()	const;

	DECLARE_SINGLE(CDevice)
};

